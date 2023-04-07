#include "rocksdb/cs561/cs561_log.h"

#include <fstream>
#include <iomanip>
#include <chrono>

void CS561Log::Log(const std::string &content, LogLevel log_level=INFO) {
    static std::ofstream f(RECORD_FILEPATH);

    const auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    const auto local_time = std::localtime(&now);
    f << std::put_time(local_time, "%Y-%m-%d %H:%M:%S") << " --- " << content << std::endl;
}

