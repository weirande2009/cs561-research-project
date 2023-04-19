#pragma once

#include <string>

class CS561Log{
private:
    // this file will record some numerical data
    inline static const std::string RECORD_FILEPATH = "log/log.txt";
    inline static const std::string RESULT_FILEPATH = "result/result.txt";
    inline static const std::string MINIMUM_FILEPATH = "result/minimum.txt";

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

    /**
     * Record the total WA for a certain run
     * @param WA write amplification of a run
     * @param left_bytes left bytes when the WA is recorded
    */
    static void LogResult(size_t WA, size_t left_bytes);

    /**
     * Record the minimum WA so far
     * @param WA minimum write amplification so far
    */
    static void LogMinimum(size_t WA, size_t left_bytes);

    /**
     * Load minimum WA so far
    */
    static std::pair<size_t, size_t> LoadMinimum();

};
