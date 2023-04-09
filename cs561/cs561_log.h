#pragma once

#include <string>

class CS561Log{
private:
    // this file will record some numerical data
    inline static const std::string RECORD_FILEPATH = "log/RecordFile";

public:
    enum LogLevel{
        INFO,
        WARNING,
        ERROR,
    };

    // TODO: Peixu
    /**
     * Log file selection of a certain version
     * @param content content to log
    */
    static void Log(const std::string& content, LogLevel log_level=INFO);

};
