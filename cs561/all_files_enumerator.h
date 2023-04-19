#pragma once

#include "db/version_set.h"
#include "cs561/picking_history_collector.h"

#include <iostream>

namespace ROCKSDB_NAMESPACE {

struct Fsize;
class PickingHistoryCollector;

class AllFilesEnumerator{
private:
    // record the last version of of each level, if the version remains
    // unchanged, there is no compaction happening in this level, so
    // we don't need to do anything to this level
    std::vector<std::size_t> last_version;

    // history collector
    PickingHistoryCollector collector;

    // whether this enumerator is activated
    bool activated;
    
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

    /**
     * get the object of picking history collector
    */
    PickingHistoryCollector& GetCollector();

    /**
     * terminate the program and do some collecting work
    */
    void Terminate();

    /**
     * Check whether current WA is larger than global min and terminate if so.
    */
    void Pruning();

    bool Activated();

    void SetActivated(bool b);

};

}  // namespace ROCKSDB_NAMESPACE