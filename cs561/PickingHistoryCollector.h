#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <set>

//#include "version_set.h"
#include "db/version_set.h"

#include "FileSerializer.h"

class PickingHistoryCollector;

namespace std {
    template<>
    struct hash<std::vector<ROCKSDB_NAMESPACE::Fsize>> {
        inline size_t operator()(const std::vector<ROCKSDB_NAMESPACE::Fsize>& temp) const {
            static FileSerializer serializer;
            static hash<std::string> hasher;

            size_t res = 0;

            for (const auto& t: temp)
                res ^= hasher(serializer.Serialize(t.file));

            return res;
        }
    };
}

class PickingHistoryCollector{

private:
    // serialized FileMetaData(s) -> chose indexes
//    std::unordered_map<std::string, std::set<int>> m_history;
    // hashed vector -> chose indexes
    std::unordered_map<size_t, std::set<int>> m_history;

    inline static const std::string DUMP_FILENAME = "DumpFile";

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
    static std::string serialize_Fsize_vec(const std::vector<ROCKSDB_NAMESPACE::Fsize>& temp);

    void recover_from_file();

    void dump_to_file();

public:
    PickingHistoryCollector() {
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
};

