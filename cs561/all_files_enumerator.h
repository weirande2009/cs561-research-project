#pragma once

#include "db/version_set.h"
#include "cs561/picking_history_collector.h"

#include <iostream>

namespace ROCKSDB_NAMESPACE {

class PickingHistoryCollector;

class AllFilesEnumerator{
private:
    // record the last version of of each level, if the version remains
    // unchanged, there is no compaction happening in this level, so
    // we don't need to do anything to this level
    std::vector<std::size_t> last_version;

    // history collector
    PickingHistoryCollector collector;

    // log level
    // there are two log level (default 1):
    // 1- only log the hash value of the version and the chosen file index
    // 2- log the concrete information of the version and the chosen file index
    // all other level will be treated as no log
    int log_level;

    // manual list: contains the file choice of each compaction
    std::vector<size_t> manual_list;

    // the compaction counter, record the number of compactions
    int compaction_counter;
    
    AllFilesEnumerator();
    ~AllFilesEnumerator();

public:
    enum CompactionStrategy{
        CRoundRobin,
        CMinOverlappingRatio,
        CEnumerateAll,
        CManual,
    };

    // Compaction strategy
    CompactionStrategy strategy;

    static AllFilesEnumerator& GetInstance() {
        static AllFilesEnumerator instance;
        return instance;
    }

    /**
     * according to the current file version, choose a new file as the first file
     * @param temp current version
     * @param level the level of the version
     * 
     * @return the index of the chosen file
    */
    int EnumerateAll(std::vector<Fsize>& temp, int level);

    /**
     * Collect the compaction information
     * @param files current version of all levels
     * @param file_overlapping_ratio the overlapping ratio of each file in the level
     * @param num_level the number of levels
     * @param level the level of the triggered compaction
     * @param index the index of the chosen file
    */
    void CollectCompactionInfo(std::vector<FileMetaData*>* files, 
                               const std::vector<uint64_t>& file_overlapping_ratio, 
                               int num_level, 
                               int level, 
                               int index);

    /**
     * Get the next file choice when replaying compaction
    */
    int NextChoiceForManual();

    /**
     * Set the manual list
     * @param ml the manual list
    */
    void SetManualList(const std::vector<size_t>& ml);

    /**
     * get the object of picking history collector
    */
    PickingHistoryCollector& GetCollector();

    /**
     * Terminate the program and do some collecting work
    */
    void Terminate();

    /**
     * Check whether current WA is larger than global min and terminate if so.
    */
    void Pruning();

    /**
     * Get the log level
    */
   int GetLogLevel();

   /**
    * Set the log level
    * @param log_level_ the new log level
   */
    void SetLogLevel(int log_level_);

    /**
     * Check whether all possible selections have been enumerated
    */
   bool CheckFinishEnumeration();

};

}  // namespace ROCKSDB_NAMESPACE