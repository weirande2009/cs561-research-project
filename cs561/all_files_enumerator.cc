#include "cs561/all_files_enumerator.h"

#include "cs561/cs561_log.h"



namespace ROCKSDB_NAMESPACE {

AllFilesEnumerator::AllFilesEnumerator() : 
    last_version(std::vector<std::size_t>(2, std::numeric_limits<size_t>::max())),
    collector(),
    activated(false) {
    std::cout << "Initialize AllFilesEnumerator" << std::endl;
    CS561Log::Log("==================================================");
}

AllFilesEnumerator::~AllFilesEnumerator() {
    
}

void AllFilesEnumerator::EnumerateAll(std::vector<rocksdb::Fsize>& temp, int level) {
    size_t index = std::numeric_limits<size_t>::max();
    // compute hash of the current version
    static std::hash<std::vector<ROCKSDB_NAMESPACE::Fsize>> hasher;
    std::size_t hash_value = hasher(temp);
    // if temp is empty or the version of temp remains unchanged, just return
    if(temp.empty() || hash_value == last_version[level]){
        return;
    }
    last_version[level] = hash_value;
    // select a file according to the history
    index = collector.GetVersionForests().GetCompactionFile(level, hash_value, temp.size());
    // log
    CS561Log::Log("Level: " + std::to_string(level) + ", Version: " + std::to_string(hash_value) + ", File index: " + std::to_string(index));
    // check whether we find a file to compact
    if(index != std::numeric_limits<size_t>::max()){
        // swap the chosen file to the first position
        rocksdb::Fsize tmp = temp[index];
        temp[index] = temp[0];
        temp[0] = tmp;
    }
    else{
        // we need to terminate the program because we don't need to continue
        Terminate();
    }
}

PickingHistoryCollector& AllFilesEnumerator::GetCollector(){
    return collector;
}

void AllFilesEnumerator::Terminate(){
    // Before termination, dump version forests to file
    collector.GetVersionForests().DumpToFile();
    // record WA and minimum in this run
    collector.DumpWAResult();
    // Log
    CS561Log::Log("Terminate program due to early stop");
    // Terminate program
    exit(1);
}

void AllFilesEnumerator::Pruning(){
    // Check whether current WA already exceeds global min
    if (!collector.CheckContinue()) {
        // Set the current version to be fully enumerated (not need to explore further)
        collector.GetVersionForests().GetLevelVersionForest(1).SetCurrentVersionFullyEnumerated();
        // set the flag of the current node
        CS561Log::Log("Current WA already exceeds the global minimum");
        Terminate();
    }
}

bool AllFilesEnumerator::Activated(){
    return activated;
}

void AllFilesEnumerator::SetActivated(bool b){
    activated = b;
}


} // namespace ROCKSDB_NAMESPACE