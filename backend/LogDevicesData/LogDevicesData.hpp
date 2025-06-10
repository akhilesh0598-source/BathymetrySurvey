#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <mutex>
#include <thread>
#include <atomic>

#include "../GlobalVariables/GlobalDevicesData.hpp"

class LogData {
public:
    explicit LogData(const std::string& logFilePath);
    ~LogData();

    void start();
    void stop();
    const std::string& getFilePath() const;

private:
    void run();

    std::string logFilePath_;
    std::atomic<bool> terminateThread_;
    std::atomic<bool> loggingActive_;
    std::thread logThread_;
};

class LogManager {
public:
    LogManager();
    ~LogManager();

    void startLogging(const std::string& clientId);
    void stopLogging(const std::string& clientId);
    std::string getLastLogFilePath(const std::string& clientId);

private:
    std::string createNewLogFilePath(const std::string& clientId);
    std::string getLogsDir() const;

    std::unordered_map<std::string, std::unique_ptr<LogData>> activeLoggers_;
    std::unordered_map<std::string, std::string> lastLogFilePathMap_;
    std::mutex loggerMutex_;
};
