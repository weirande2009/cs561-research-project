#include <fstream>
#include <iomanip>

#include "cs561/picking_history_collector.h"
#include "cs561/cs561_log.h"

using std::endl;

namespace ROCKSDB_NAMESPACE {

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
    for (std::size_t i = 0; i < n; ++ i) {
        res += std::to_string(ordered_temp[i]);
        res.push_back('A');
    }
    return res;
}

void PickingHistoryCollector::UpdateWA(size_t new_WA) {
    WA += new_WA;
    WA_corresponding_left_bytes = left_bytes;
    // std::cout << std::endl << "New added WA: " << new_WA << std::endl;
    // PrintCurrentData();
}

bool PickingHistoryCollector::CheckContinue() {
    return (WA + WA_corresponding_left_bytes) <= (global_min_WA + global_min_WA_corresponding_left_bytes);
}

void PickingHistoryCollector::UpdateLeftBytes(size_t new_left_bytes) {
    left_bytes = new_left_bytes;
}

VersionForests& PickingHistoryCollector::GetVersionForests(){
    return forests;
}

void PickingHistoryCollector::DumpToFile(){
    // dump wa
    CS561Log::LogResult(WA, WA_corresponding_left_bytes);
    // compute minimum WA
    if((WA + WA_corresponding_left_bytes) <= (global_min_WA + global_min_WA_corresponding_left_bytes)){
        // dump minimum
        CS561Log::LogMinimum(WA, WA_corresponding_left_bytes);
    }
}

void PickingHistoryCollector::DumpWAResult(){
    // dump wa
    CS561Log::LogResult(WA, WA_corresponding_left_bytes);
}

void PickingHistoryCollector::DumpWAMinimum(){
    // compute minimum WA
    if((WA + WA_corresponding_left_bytes) <= (global_min_WA + global_min_WA_corresponding_left_bytes)){
        // dump minimum
        CS561Log::LogMinimum(WA, WA_corresponding_left_bytes);
    }
    else{
        // dump minimum
        CS561Log::LogMinimum(global_min_WA, global_min_WA_corresponding_left_bytes);
    }
}

void PickingHistoryCollector::PrintCurrentData(){
    std::cout << "Current WA: " << WA << std::endl;
    std::cout << "Gloabl Minimum WA: " << global_min_WA << std::endl;
    std::cout << "Left Bytes: " << left_bytes << std::endl;
}

} // namespace ROCKSDB_NAMESPACE
