#pragma once

#include "db/version_edit.h"
#include "rocksdb/cs561/PickingHistoryCollector.h"

namespace ROCKSDB_NAMESPACE {

struct Fsize;
class PickingHistoryCollector;

class AllFilesEnumerator{
private:
    PickingHistoryCollector collector;

public:
    AllFilesEnumerator();
    ~AllFilesEnumerator();

    // according to the current file version, choose a new file as the first file
    // @param temp: the current file version
    void EnumerateAll(std::vector<Fsize>& temp);

};

}  // namespace ROCKSDB_NAMESPACE