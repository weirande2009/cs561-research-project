#include "cs561/AllFilesEnumerator.h"

AllFilesEnumerator::AllFilesEnumerator() {

}

AllFilesEnumerator::~AllFilesEnumerator() {

}

void AllFilesEnumerator::EnumerateAll(std::vector<rocksdb::Fsize>& temp) {
    int index = -1;
    // find the first file that is not in the history
    for(int i=0; i<temp.size(); i++){
        if(!collector.IsInHistory(temp, i)){
            index = i;
            break;
        }
    }
    // add the newly picked file to history
    if(index != -1){
        collector.AddPickingFile(temp, index);
        // swap the chosen file to the first position
        rocksdb::Fsize tmp = temp[index];
        temp[index] = temp[0];
        temp[0] = tmp;
    }
}
