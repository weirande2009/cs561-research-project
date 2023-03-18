#pragma once

#include "db/version_set.h"
#include "cs561/PickingHistoryCollector.h"

class AllFilesEnumerator{

private:
    PickingHistoryCollector collector;


public:
    AllFilesEnumerator();
    ~AllFilesEnumerator();

    // according to the current file version, choose a new file as the first file
    // @param temp: the current file version
    void EnumerateAll(std::vector<rocksdb::Fsize>& temp);

};