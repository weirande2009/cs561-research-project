#include "cs561/all_files_enumerator.h"

#include "cs561/cs561_log.h"



namespace ROCKSDB_NAMESPACE {

AllFilesEnumerator::AllFilesEnumerator() : 
    last_version(std::vector<std::size_t>(2, std::numeric_limits<size_t>::max())),
    collector(),
    log_level(1){
        
}

AllFilesEnumerator::~AllFilesEnumerator() {
    
}

int AllFilesEnumerator::EnumerateAll(std::vector<rocksdb::Fsize>& temp, int level) {
    size_t index = std::numeric_limits<size_t>::max();
    // compute hash of the current version
    static std::hash<std::vector<ROCKSDB_NAMESPACE::Fsize>> hasher;
    std::size_t hash_value = hasher(temp);
    // if temp is empty or the version of temp remains unchanged, just return
    if(temp.empty() || hash_value == last_version[level]){
        return -1;
    }
    last_version[level] = hash_value;
    // if this is a new version, record the file information of this version to disk
    if(!collector.GetVersionForest().IsVersionExist(level, hash_value)){
        CS561Log::LogVersion(temp, hash_value);
    }
    // log
    if(log_level == 1){
        CS561Log::Log("Level: " + std::to_string(level) + ", Version: " + std::to_string(hash_value) + ", File index: " + std::to_string(index));
    }
    // select a file according to the history
    index = collector.GetVersionForest().GetCompactionFile(level, hash_value, temp.size());
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
    return static_cast<int>(index);
}

void AllFilesEnumerator::CollectCompactionInfo(
        std::vector<FileMetaData*>* files, 
        const std::vector<uint64_t>& file_overlapping_ratio, 
        int num_level, 
        int level, 
        int index){
    // compute hash of the current version
    static std::hash<std::vector<ROCKSDB_NAMESPACE::Fsize>> hasher;
    std::vector<Fsize> temp(files[level].size());
    for (size_t i = 0; i < files[level].size(); i++) {
      temp[i].index = i;
      temp[i].file = files[level][i];
    }
    std::size_t hash_value = hasher(temp);
    // collect this seletion
    collector.CollectCompactionInfo(level, files, file_overlapping_ratio, num_level, hash_value, index);
    // log
    if(log_level == 2){
        CS561Log::LogRegularCompactionInfo(collector.GetCompactionsInfo().back(), collector.GetCompactionsInfo().size());
    }
}

PickingHistoryCollector& AllFilesEnumerator::GetCollector(){
    return collector;
}

void AllFilesEnumerator::Terminate(){
    // Before termination, dump version forests to file
    collector.GetVersionForest().DumpToFile();
    // record WA and minimum in this run
    // collector.DumpWAResult();
    // Log
    // CS561Log::Log("Terminate program due to early stop");
    // Terminate program
    exit(1);
}

void AllFilesEnumerator::Pruning(){
    // Check whether current WA already exceeds global min
    if (!collector.CheckContinue()) {
        // Set the current version to be fully enumerated (not need to explore further)
        collector.GetVersionForest().GetLevelVersionTree(1).SetCurrentVersionFullyEnumerated();
        // set the flag of the current node
        CS561Log::Log("Terminate reason: current WA already exceeds the minimum");
        Terminate();
    }
}

int AllFilesEnumerator::GetLogLevel(){
    return log_level;
}

void AllFilesEnumerator::SetLogLevel(int log_level_){
    log_level = log_level_;
}

bool AllFilesEnumerator::CheckFinishEnumeration(){
    return false;
}


} // namespace ROCKSDB_NAMESPACE