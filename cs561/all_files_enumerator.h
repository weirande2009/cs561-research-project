#pragma once

#include "db/version_set.h"
#include "cs561/picking_history_collector.h"

#include <iostream>

namespace ROCKSDB_NAMESPACE {

struct Fsize;
class PickingHistoryCollector;

class AllFilesEnumerator{
private:
    PickingHistoryCollector collector;
    AllFilesEnumerator();
    ~AllFilesEnumerator();

public:
    static AllFilesEnumerator& GetInstance() {
        static AllFilesEnumerator instance;
        return instance;
    }

    /**
     * Record the current compaction according the version and files chosen to compact
     * @param input_files contains the information of files chosen to compact in each level
     * @param version_storage_info contains the current version of files for each level
    */
    // void RecordCompaction(const std::vector<CompactionInputFiles>& input_files, const rocksdb::VersionStorageInfo& version_storage_info);

    /**
     * according to the current file version, choose a new file as the first file
     * @param temp current version
     * @param level the level of the version
    */
    void EnumerateAll(std::vector<Fsize>& temp, int level);

    PickingHistoryCollector& GetCollector();
};

}  // namespace ROCKSDB_NAMESPACE