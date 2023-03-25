#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <set>

//#include "version_set.h"
#include "db/version_edit.h"

#include "rocksdb/cs561/file_serializer.h"
#include "rocksdb/cs561/version_forests.h"

namespace ROCKSDB_NAMESPACE {

struct Fsize;

class PickingHistoryCollector{

private:
    // serialized FileMetaData(s) -> chose indexes
//    std::unordered_map<std::string, std::set<int>> m_history;
    // hashed vector -> chose indexes
    std::unordered_map<size_t, std::set<int>> m_history;

    /******* variables above maybe deprecated *******/ 
    inline static const std::string DUMP_FILEPATH1 = "DumpFile1";
    inline static const std::string DUMP_FILEPATH2 = "DumpFile2";

    // this file will record some numerical data
    inline static const std::string RECORD_FILEPATH = "RecordFile";

    // the version forests which stores the history of all levels
    VersionForests forests;

    // current global minimum WA
    size_t global_min_WA;

    // current WA
    size_t WA;

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

    void recover_from_file();

    void dump_to_file();

public:
    PickingHistoryCollector() : forests(VersionForests({DUMP_FILEPATH1, DUMP_FILEPATH2})){
        recover_from_file();
    }

    ~PickingHistoryCollector() {
        dump_to_file();
    }

    // Add one selection to history (Maybe later we need to also collect the corresponding WA of this selection)
    // @param temp: the current file version
    // @param index: the index of the file that is selected
    void AddPickingFile(const std::vector<ROCKSDB_NAMESPACE::Fsize> &temp, int index);

    void AddPickingFile(size_t hash_value, int index);

    // Check whether the index of the current file version is in the history
    // @param temp: the current file version
    // @param index: the index of the file that is going to be checked
    // @return: true if the index is in the history, false otherwise
    bool IsInHistory(const std::vector<ROCKSDB_NAMESPACE::Fsize> &temp, int index);

    bool IsInHistory(size_t hash_value, int index);

    /******* functions above maybe deprecated *******/ 

    // TODO: Chen
    /**
     * Find a file index in the current level to compact
     * @param level: the level of the version
     * @param hash_value: the hash value of the version
     * @return the index of the file in the version
     */
    size_t FindPickingFile(int level, size_t hash_value);

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

   // TODO: Peixu
   /**
    * Log file selection of a certain version
    * @param hash_value: hash value of the version
    * @param index: index of the chosen file
   */
  void LogSelection(size_t hash_value, int index);
};

} // namespace ROCKSDB_NAMESPACE
