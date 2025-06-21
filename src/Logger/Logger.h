#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <sstream>
#include <vector>
#include <chrono>
#include <ctime>
#include <iostream>
#include <termcolor/termcolor.hpp>

enum LogType
{
    INFO,
    WARNING,
    ERROR
};

struct LogEntry
{
    LogType type;
    std::string message;
};

class Logger
{
private:
    static std::string CurrentDateTimeToString()
    {
        std::time_t now_c = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        const char *timeFormat = "%d/%b/%Y %H:%M:%S";
        char output[100];
        std::strftime(output, sizeof(output), timeFormat, std::localtime(&now_c));
        return output;
    }

    template <typename... Args>
    static std::string constructLogMessage(const char *logType, const Args &...args)
    {
        std::ostringstream oss;
        oss << logType << " | " << CurrentDateTimeToString() << " - ";
        (oss << ... << args);
        return oss.str();
    }

public:
    static std::vector<LogEntry> logEntries;
    template <typename... Args>
    static void Log(const Args &...args);
    template <typename... Args>
    static void Warn(const Args &...args);
    template <typename... Args>
    static void Err(const Args &...args);
};

template <typename... Args>
void Logger::Log(const Args &...args)
{
    LogEntry entry = {.type = LogType::INFO};
    entry.message = constructLogMessage("INFO", args...);
    std::cout << termcolor::green << entry.message << termcolor::reset << std::endl;
    logEntries.push_back(entry);
}

template <typename... Args>
void Logger::Warn(const Args &...args)
{
    LogEntry entry = {.type = LogType::WARNING};
    entry.message = constructLogMessage("WARNING", args...);
    std::cout << termcolor::yellow << entry.message << termcolor::reset << std::endl;
    logEntries.push_back(entry);
}

template <typename... Args>
void Logger::Err(const Args &...args)
{
    LogEntry entry = {.type = LogType::ERROR};
    entry.message = constructLogMessage("ERROR", args...);
    std::cerr << termcolor::red << entry.message << termcolor::reset << std::endl;
    logEntries.push_back(entry);
}

#endif
