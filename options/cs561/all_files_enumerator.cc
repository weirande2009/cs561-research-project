#include "rocksdb/cs561/all_files_enumerator.h"

#include "rocksdb/cs561/cs561_log.h"

namespace ROCKSDB_NAMESPACE {

AllFilesEnumerator::AllFilesEnumerator() : collector(){

}

AllFilesEnumerator::~AllFilesEnumerator() {

}

void AllFilesEnumerator::EnumerateAll(std::vector<rocksdb::Fsize>& temp, int level) {
    size_t index = std::numeric_limits<size_t>::max();
    // compute hash of the current version
    static std::hash<std::vector<ROCKSDB_NAMESPACE::Fsize>> hasher;
    std::size_t hash_value = hasher(temp);
    // select a file according to the history
    index = collector.GetVersionForests().GetCompactionFile(level, hash_value, temp.size());
    // log
    CS561Log::Log("Version: " + std::to_string(hash_value) + ", File index: " + std::to_string(index));
    // check whether we find a file to compact
    if(index != std::numeric_limits<size_t>::max()){
        // swap the chosen file to the first position
        rocksdb::Fsize tmp = temp[index];
        temp[index] = temp[0];
        temp[0] = tmp;
    }
    else{
        // we need to terminate the program because we don't need to continue
    }
}

PickingHistoryCollector& AllFilesEnumerator::GetCollector(){
    return collector;
}

} // namespace ROCKSDB_NAMESPACE