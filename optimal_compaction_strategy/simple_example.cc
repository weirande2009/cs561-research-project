#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include "rocksdb/db.h"
#include "rocksdb/slice.h"
#include "rocksdb/cache.h"
#include "rocksdb/options.h"
#include "rocksdb/advanced_options.h"
#include "rocksdb/table.h"
#include "cs561/all_files_enumerator.h"

using namespace rocksdb;
std::string kDBPath = "/mnt/ramdisk/cs561_project1";

void backgroundJobMayComplete(DB* db) {
    uint64_t pending_compact;
    uint64_t pending_compact_bytes;
    uint64_t running_compact;
    uint64_t pending_flush;
    uint64_t running_flush;

    bool success = false; 
    while (pending_compact || pending_compact_bytes || running_compact
      || pending_flush || running_flush || !success) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        success = db->GetIntProperty("rocksdb.compaction-pending", &pending_compact)
        && db->GetIntProperty("rocksdb.estimate-pending-compaction-bytes", 
            &pending_compact_bytes)
        && db->GetIntProperty("rocksdb.num-running-compactions", &running_compact)
        && db->GetIntProperty("rocksdb.mem-table-flush-pending", &pending_flush)
        && db->GetIntProperty("rocksdb.num-running-flushes", &running_flush);
    }
}

inline void showProgress(const uint64_t& workload_size, const uint64_t& counter) {

    if (counter / (workload_size / 100) >= 1) {
        for (int i = 0; i < 104; i++) {
            std::cout << "\b";
            fflush(stdout);
        }
    }
    for (int i = 0; i < counter / (workload_size / 100); i++) {
        std::cout << "=";
        fflush(stdout);
    }
    std::cout << std::setfill(' ') << std::setw(101 - counter / (workload_size / 100));
    std::cout << counter * 100 / workload_size << "%";
    fflush(stdout);

    if (counter == workload_size) {
        std::cout << "\n";
        return;
    }
}

void runWorkload(Options& op, WriteOptions& write_op, ReadOptions& read_op, std::string compaciton_strategy, uint64_t total_written_bytes, const std::string& experiment_path) {
    DB* db;

    op.create_if_missing = true;
    op.write_buffer_size = 8 * 1024 * 1024;
    op.target_file_size_base = 8 * 1024 * 1024;
    op.level0_file_num_compaction_trigger = 4;
    op.max_bytes_for_level_multiplier = 10;
    op.max_bytes_for_level_base = 32 * 1024 * 1024;

    // set the compaction strategy
    if(compaciton_strategy == "kRoundRobin"){
        op.compaction_pri = kRoundRobin;
    }
    else if(compaciton_strategy == "kMinOverlappingRatio"){
        op.compaction_pri = kMinOverlappingRatio;
    }
    else if(compaciton_strategy == "kEnumerateAll"){
        op.compaction_pri = kEnumerateAll;
    }
    AllFilesEnumerator::GetInstance().SetLogLevel(2);
    // set the total bytes to be inserted to database
    // uint64_t total_bytes = 128000000;
    uint64_t total_bytes = total_written_bytes;

    if(op.compaction_pri == kEnumerateAll){
        AllFilesEnumerator::GetInstance().strategy = AllFilesEnumerator::CompactionStrategy::CEnumerateAll;
    } else if (op.compaction_pri == kMinOverlappingRatio){
        AllFilesEnumerator::GetInstance().strategy = AllFilesEnumerator::CompactionStrategy::CMinOverlappingRatio;
    } else if (op.compaction_pri == kRoundRobin){
        AllFilesEnumerator::GetInstance().strategy = AllFilesEnumerator::CompactionStrategy::CRoundRobin;
    } 

    {
        op.memtable_factory = std::shared_ptr<VectorRepFactory>(new VectorRepFactory);
        op.allow_concurrent_memtable_write = false;
    }

    {
        // op.memtable_factory = std::shared_ptr<SkipListFactory>(new SkipListFactory);
    }

    {
        //op.memtable_factory = std::shared_ptr<MemTableRepFactory>(NewHashSkipListRepFactory());
        //op.allow_concurrent_memtable_write = false;
    }

    {
        //op.memtable_factory = std::shared_ptr<MemTableRepFactory>(NewHashLinkListRepFactory());
        //op.allow_concurrent_memtable_write = false;
    }


    //BlockBasedTableOptions table_options;
    //table_options.block_cache = NewLRUCache(8*1048576);
    //op.table_factory.reset(NewBlockBasedTableFactory(table_options));

    Status s = DB::Open(op, kDBPath, &db);
    if (!s.ok()) std::cerr << s.ToString() << std::endl;
    assert(s.ok());

    // opening workload file for the first time
    std::string workload_path = experiment_path + "/workload.txt";
    std::ifstream workload_file;
    workload_file.open(workload_path);
    assert(workload_file);
    // doing a first pass to get the workload size
    uint32_t entry_size = 8;
    uint64_t workload_size = 0;
    uint64_t insert_update_size = 9000000;
    uint64_t inserted_bytes = 0;
    std::string line;
    while (std::getline(workload_file, line))
        ++workload_size;
    workload_file.close();

    workload_file.open(workload_path);
    assert(workload_file);

//    // Clearing the system cache
//    std::cout << "Clearing system cache ..." << std::endl;
//    int clean_flag = system("sudo sh -c 'echo 3 >/proc/sys/vm/drop_caches'");
//    if (clean_flag) {
//        std::cerr << "Cannot clean the system cache" << std::endl;
//        exit(0);
//    }

    AllFilesEnumerator::GetInstance().GetCollector().UpdateLeftBytes(total_bytes);

    Iterator* it = db->NewIterator(read_op); // for range reads
    uint64_t counter = 0; // for progress bar

    while (!workload_file.eof()) {
        char instruction;
        std::string value, key, start_key, end_key;
        workload_file >> instruction;
        switch (instruction)
        {
        case 'I': // insert
        case 'U': // update
            workload_file >> key >> value;
            inserted_bytes += key.length() + value.length();
            // Put key-value
            s = db->Put(write_op, key, value);
            if (!s.ok()) std::cerr << s.ToString() << std::endl;
            assert(s.ok());
            counter++;
            break;

        case 'Q': // probe: point query
            workload_file >> key;
            s = db->Get(read_op, key, &value);
            //if (!s.ok()) std::cerr << s.ToString() << "key = " << key << std::endl;
            // assert(s.ok());
            counter++;
            break;

        case 'S': // scan: range query
            workload_file >> start_key >> end_key;
            it->Refresh();
            assert(it->status().ok());
            for (it->Seek(start_key); it->Valid(); it->Next()) {
                //std::cout << "found key = " << it->key().ToString() << std::endl;
                if (it->key().ToString() == end_key) {
                    break;
                }
            }
            if (!it->status().ok()) {
                std::cerr << it->status().ToString() << std::endl;
            }
            counter++;
            break;
        case 'D':
            workload_file >> key;
            s = db->Delete(write_op, key);
            if (!s.ok()) std::cerr << s.ToString() << std::endl;
            assert(s.ok());
            break;
        default:
            return;
            // std::cerr << "ERROR: Case match NOT found !!" << std::endl;
            break;
        }

        if (total_bytes >= inserted_bytes)
            AllFilesEnumerator::GetInstance().GetCollector().UpdateLeftBytes(total_bytes-inserted_bytes);

        if (workload_size < 100) workload_size = 100;
        if (counter % (workload_size / 100) == 0) {
            // showProgress(workload_size, counter);
        }
    }

    workload_file.close();

    FlushOptions flush_options;
    db->Flush(flush_options);
    backgroundJobMayComplete(db);

    s = db->Close();
    if (!s.ok()) std::cerr << s.ToString() << std::endl;
    assert(s.ok());
    delete db;
    return;
}

int main(int argc, char* argv[]) {
    if(argc != 5){
        std::cout << "There should three parameters: " << std::endl;
        std::cout << "1. Compaction strategy (kRoundRobin, kMinOverlappingRatio, kEnumerateAll)" << std::endl;
        std::cout << "2. Total written bytes in the workload" << std::endl;
        std::cout << "3. The database path" << std::endl;
        std::cout << "4. The experiment path" << std::endl;
        return -1;
    }
    // parse compaction strategy
    std::string compaction_strategy = argv[1];
    uint64_t total_written_bytes = std::stoi(argv[2]);
    kDBPath = argv[3];
    std::cout << argv[4] << std::endl;
    CS561Log::SetLogRootPath(argv[4]);
    
    CS561Log::Log("==================================================");
    // std::cout << "Program information: " << std::endl;
    CS561Log::Log("Program information: ");
    // std::cout << "Compaction strategy: " << compaction_strategy << std::endl;
    CS561Log::Log("Compaction strategy: " + compaction_strategy);
    // std::cout << "Total written bytes: " << total_written_bytes << std::endl;
    CS561Log::Log("Total written bytes: " + std::to_string(total_written_bytes));
    // std::cout << "Database path: " << argv[3] << std::endl;
    CS561Log::Log("Database path: " + std::string(argv[3]));
    // std::cout << "Experiment path: " << argv[4] << std::endl;
    CS561Log::Log("Experiment path: " + std::string(argv[4]));

    auto start_time = std::chrono::system_clock::now();
    AllFilesEnumerator::GetInstance();
    Options options;
    WriteOptions write_op;
    ReadOptions read_op;
    runWorkload(options, write_op, read_op, compaction_strategy, total_written_bytes, argv[4]);

    auto end_time = std::chrono::system_clock::now();
    auto durationInMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    std::cout << "Total running time: " << durationInMicroseconds.count() << std::endl;
    CS561Log::Log("Total running time: " + std::to_string(durationInMicroseconds.count()) + "us");
    return 0;
}
