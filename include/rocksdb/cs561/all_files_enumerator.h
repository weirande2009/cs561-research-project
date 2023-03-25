#pragma once

#include "db/version_edit.h"
#include "rocksdb/cs561/picking_history_collector.h"

namespace ROCKSDB_NAMESPACE {

struct Fsize;
class PickingHistoryCollector;

class AllFilesEnumerator{
private:
    PickingHistoryCollector collector;
    AllFilesEnumerator();

    static AllFilesEnumerator instance;

public:
    static AllFilesEnumerator& GetInstance() {
        return instance;
    }
    ~AllFilesEnumerator();

    // according to the current file version, choose a new file as the first file
    // @param temp: the current file version
    void EnumerateAll(std::vector<Fsize>& temp, int level);

    PickingHistoryCollector& GetCollector();

};

}  // namespace ROCKSDB_NAMESPACE