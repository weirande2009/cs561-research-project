#include "rocksdb/cs561/all_files_enumerator.h"

namespace ROCKSDB_NAMESPACE {

AllFilesEnumerator::AllFilesEnumerator() {

}

AllFilesEnumerator::~AllFilesEnumerator() {

}

void AllFilesEnumerator::EnumerateAll(std::vector<rocksdb::Fsize>& temp, int level) {
    int index = -1;
    // compute hash of the current version
    static std::hash<std::vector<ROCKSDB_NAMESPACE::Fsize>> hasher;
    std::size_t h = hasher(temp);
    // find the first file that is not in the history
    for(std::size_t i=0; i<temp.size(); i++){
        if(!collector.IsInHistory(h, i)){
            index = i;
            break;
        }
    }
    // add the newly picked file to history
    if(index != -1){
        collector.AddPickingFile(h, index);
        // swap the chosen file to the first position
        rocksdb::Fsize tmp = temp[index];
        temp[index] = temp[0];
        temp[0] = tmp;
    }
}

PickingHistoryCollector& AllFilesEnumerator::GetCollector(){
    return collector;
}

} // namespace ROCKSDB_NAMESPACE