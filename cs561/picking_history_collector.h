#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <set>

//#include "version_set.h"
#include "db/version_edit.h"

#include "cs561/file_serializer.h"
#include "cs561/version_forests.h"
#include "cs561/cs561_log.h"

namespace ROCKSDB_NAMESPACE {

struct Fsize;

class PickingHistoryCollector{

private:
    // serialized FileMetaData(s) -> chose indexes
//    std::unordered_map<std::string, std::set<int>> m_history;
    // hashed vector -> chose indexes
    std::unordered_map<size_t, std::set<int>> m_history;

    /******* variables above maybe deprecated *******/ 
    inline static const std::string DUMP_FILEPATH_LEVEL0 = "history/picking_history_level0";
    inline static const std::string DUMP_FILEPATH_LEVEL1 = "history/picking_history_level1";

    // this file will record some numerical data
    inline static const std::string RESULT_FILEPATH = "result/result.txt";

    // this file will record the minimal WA so far
    inline static const std::string MINIMUM_FILEPATH = "result/minimum.txt";

    // file for log
    inline static const std::string LOG_FILEPATH = "log/log.txt";

    // the version forests which stores the history of all levels
    VersionForests forests;

    // current global minimum WA
    size_t global_min_WA;
    size_t global_min_WA_corresponding_left_bytes;

    // current WA
    size_t WA;
    size_t WA_corresponding_left_bytes;

    // remaining bytes to be inserted
    size_t left_bytes;

private:
    /**
     * serialize the Fsize vector.
     * @brief firstly sort the hashed Fsize vector to avoid the same version but different order scenario,
     *        then return the serialized hashed Fsize vector
     * @note the result is non-deserializable.
     *       and the hash collision is ignored.
     * @param temp: the current file version
     * @return serialized hash string
     */
    static std::string serialize_Fsize_vec(const std::vector<Fsize>& temp);

    void PrintCurrentData();

public:
    // FIXME: initialize global_min_WA, WA and left_bytes
    PickingHistoryCollector() : forests(VersionForests({DUMP_FILEPATH_LEVEL0, DUMP_FILEPATH_LEVEL1})){
        auto p = CS561Log::LoadMinimum();
        global_min_WA = p.first;
        global_min_WA_corresponding_left_bytes = p.second;
        WA = 0;
        left_bytes = 0;
    }

    ~PickingHistoryCollector() {
        DumpToFile();
    }

    /******* functions above maybe deprecated *******/ 
    // TODO: Chen
    /**
     * Update the current WA
     * @param new_WA: the newly computed WA
    */
    void UpdateWA(size_t new_WA);

    // TODO: Chen
    /**
     * Check whether the current WA has already exceeded the minimum even 
     * all the left compactions are trivial move.
    */
    bool CheckContinue();

    // TODO: Chen
    /**
     * Update the left inserts write
     * @param dec: decrement of left bytes
    */
    void UpdateLeftBytes(size_t dec);

    /**
     * Get forests
    */
    VersionForests& GetVersionForests();

    /**
     * Dump all record to files
    */
    void DumpToFile();

    void DumpWAResult();
    
    void DumpWAMinimum();

};

} // namespace ROCKSDB_NAMESPACE
