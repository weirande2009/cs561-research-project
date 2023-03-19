#include <fstream>

#include "PickingHistoryCollector.h"

using std::endl;

void PickingHistoryCollector::AddPickingFile(const std::vector<ROCKSDB_NAMESPACE::Fsize> &temp, int index) {
//    std::string serialized_tmp = serialize_Fsize_vec(temp);
//    m_history[serialized_tmp].insert(index);

    static std::hash<std::vector<ROCKSDB_NAMESPACE::Fsize>> hasher;
    m_history[hasher(temp)].insert(index);
}

void PickingHistoryCollector::AddPickingFile(size_t hash_value, int index) {
    m_history[hash_value].insert(index);
}

bool PickingHistoryCollector::IsInHistory(const std::vector<ROCKSDB_NAMESPACE::Fsize> &temp, int index) {
//    std::string serialized_tmp = serialize_Fsize_vec(temp);
//    return m_history[serialized_tmp].count(index);

    static std::hash<std::vector<ROCKSDB_NAMESPACE::Fsize>> hasher;
    return m_history[hasher(temp)].count(index);
}

bool PickingHistoryCollector::IsInHistory(size_t hash_value, int index) {
    return m_history[hash_value].count(index);
}

std::string PickingHistoryCollector::serialize_Fsize_vec(const std::vector<ROCKSDB_NAMESPACE::Fsize> &temp) {
    static FileSerializer serializer;
    static std::hash<std::string> hasher;

    size_t n = temp.size();

    std::vector<size_t> ordered_temp;
    ordered_temp.reserve(n);
    for (const auto& t: temp) {
        std::string ret = serializer.Serialize(t.file);
        ordered_temp.push_back(hasher(ret));
    }
    std::sort(ordered_temp.begin(), ordered_temp.end());

    std::string res;
    // size_t length(10/20) + split character(1)
    res.reserve((std::to_string(std::numeric_limits<size_t>::max()).size() + 1) * n);
    for (int i = 0; i < n; ++ i) {
        res += std::to_string(ordered_temp[i]);
        res.push_back('A');
    }
    return res;
}

void PickingHistoryCollector::recover_from_file() {
    std::fstream f(DUMP_FILENAME);
    m_history.clear();

    std::string s;
    while (!f.eof()) {
        size_t hash_value;
        size_t n;
        int idx;
        std::set<int> indexes;

        f >> hash_value >> n;
        for (int i = 0; i < n; ++ i) {
            f >> idx;
            indexes.insert(idx);
        }

        m_history[hash_value] = std::move(indexes);
    }
    f.close();
}

void PickingHistoryCollector::dump_to_file() {
    std::fstream f(DUMP_FILENAME);
    f.clear();

    for (const auto& [hash_value, indexes]: m_history) {
        f << hash_value << endl;
        f << indexes.size() << endl;
        for (int idx: indexes)
            f << idx << endl;
    }

    f.close();
}
