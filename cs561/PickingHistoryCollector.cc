#include "PickingHistoryCollector.h"


void PickingHistoryCollector::AddPickingFile(const std::vector<ROCKSDB_NAMESPACE::Fsize> &temp, int index) {
//    std::string serialized_tmp = serialize_Fsize_vec(temp);
//    m_history[serialized_tmp].insert(index);
    m_history[temp].insert(index);
}


bool PickingHistoryCollector::IsInHistory(const std::vector<ROCKSDB_NAMESPACE::Fsize> &temp, int index) {
//    std::string serialized_tmp = serialize_Fsize_vec(temp);
//    return m_history[serialized_tmp].count(index);
    return m_history[temp].count(index);
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
