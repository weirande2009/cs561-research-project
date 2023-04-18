#include "cs561/cs561_log.h"

#include <fstream>
#include <iomanip>
#include <chrono>

void CS561Log::Log(const std::string &content, LogLevel log_level) {
    static std::ofstream f(RECORD_FILEPATH, std::ios::app);
    const auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    const auto local_time = std::localtime(&now);
    std::string log_type;
    switch (log_level)
    {
    case INFO:
        log_type = "INFO";
        break;
    case WARNING:
        log_type = "WARNING";
        break;
    case ERROR:
        log_type = "ERROR";
        break;
    default:
        log_type = "NONE";
        break;
    }
    f << std::put_time(local_time, "%Y-%m-%d %H:%M:%S") << " --- " << "(" << log_type << ")" << content << std::endl;
    f.flush();
}

void CS561Log::LogResult(size_t WA){
    static std::ofstream f(RESULT_FILEPATH, std::ios::app);
    const auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    const auto local_time = std::localtime(&now);
    f << std::put_time(local_time, "%Y-%m-%d %H:%M:%S") << " --- " << WA << std::endl;
    f.flush();
}

void CS561Log::LogMinimum(size_t WA){
    static std::ofstream f(MINIMUM_FILEPATH);
    f << WA << std::endl;
    f.flush();
}

size_t CS561Log::LoadMinimum(){
    std::ifstream f(MINIMUM_FILEPATH);
    size_t wa;
    f >> wa;
    return wa;
}
