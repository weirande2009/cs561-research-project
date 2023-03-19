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
    struct std::hash<std::vector<ROCKSDB_NAMESPACE::Fsize>> {
        inline size_t operator()(const std::vector<ROCKSDB_NAMESPACE::Fsize>& temp) const {
            static FileSerializer serializer;
            static std::hash<std::string> hasher;

            size_t res = 0;

            for (const auto& t: temp)
                res ^= hasher(serializer.Serialize(t));

            return res;
        }
    };
}

class PickingHistoryCollector{

private:
    // serialized FileMetaData(s) -> chose indexes
//    std::unordered_map<std::string, std::set<int>> m_history;
    // hashed vector -> chose indexes
    std::unordered_map<std::vector<ROCKSDB_NAMESPACE::Fsize>, std::set<int>> m_history;

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

public:
    PickingHistoryCollector() = default;
    ~PickingHistoryCollector() = default;

    // Add one selection to history (Maybe later we need to also collect the corresponding WA of this selection)
    // @param temp: the current file version
    // @param index: the index of the file that is selected
    void AddPickingFile(const std::vector<ROCKSDB_NAMESPACE::Fsize> &temp, int index);

    // Check whether the index of the current file version is in the history
    // @param temp: the current file version
    // @param index: the index of the file that is going to be checked
    // @return: true if the index is in the history, false otherwise
    bool IsInHistory(const std::vector<ROCKSDB_NAMESPACE::Fsize> &temp, int index);
};

