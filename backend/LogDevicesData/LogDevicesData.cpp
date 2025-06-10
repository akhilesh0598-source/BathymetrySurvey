#include "LogDevicesData.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <iostream>
#include <filesystem>
#include <chrono>
#include <ctime>

namespace fs = std::filesystem;


LogData::LogData(const std::string& logFilePath)
    : logFilePath_(logFilePath),
      terminateThread_(false),
      loggingActive_(true)
{}

LogData::~LogData() {
    stop();
}

void LogData::start() {
    terminateThread_ = false;
    loggingActive_ = true;
    logThread_ = std::thread(&LogData::run, this);
}

void LogData::stop() {
    terminateThread_ = true;
    if (logThread_.joinable()) {
        logThread_.join();
    }
}

const std::string& LogData::getFilePath() const {
    return logFilePath_;
}

void LogData::run() {
    try {
        // Use a unique logger name per file path to avoid conflicts
        auto logger = spdlog::basic_logger_mt("csv_logger_" + logFilePath_, logFilePath_);
        logger->set_pattern("%v");
        logger->info("timestamp,latitude,longitude,distance,confidence");

        while (!terminateThread_) {
            if (loggingActive_) {
                // Sleep 1 second between logs
                std::this_thread::sleep_for(std::chrono::seconds(1));

                double lat, lon;
                int dist, conf;
                {
                    boost::mutex::scoped_lock lock1(sonar_mutex);
                    boost::mutex::scoped_lock lock2(gps_mutex);
                    lat = gpsLatitude;
                    lon = gpsLongitude;
                    dist = sonarDistance;
                    conf = sonarConfidence;
                }

                auto now = std::chrono::system_clock::now();
                std::time_t now_c = std::chrono::system_clock::to_time_t(now);
                char timeStr[32];
                std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", std::localtime(&now_c));

                logger->info("{},{},{},{},{}", timeStr, lat, lon, dist, conf);
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }

        spdlog::drop("csv_logger_" + logFilePath_);
    } catch (const spdlog::spdlog_ex& ex) {
        std::cerr << "Log error: " << ex.what() << std::endl;
    }
}

// --- LogManager implementation ---

LogManager::LogManager() {}

LogManager::~LogManager() {
    std::lock_guard<std::mutex> lock(loggerMutex_);
    for (auto& [_, logger] : activeLoggers_) {
        logger->stop();
    }
    activeLoggers_.clear();
}

void LogManager::startLogging(const std::string& clientId) {
    std::lock_guard<std::mutex> lock(loggerMutex_);

    // Stop existing logger for client if any
    auto it = activeLoggers_.find(clientId);
    if (it != activeLoggers_.end()) {
        it->second->stop();
        activeLoggers_.erase(it);
    }

    std::string filePath = createNewLogFilePath(clientId);
    lastLogFilePathMap_[clientId] = filePath;

    auto logger = std::make_unique<LogData>(filePath);
    logger->start();
    activeLoggers_[clientId] = std::move(logger);
}

void LogManager::stopLogging(const std::string& clientId) {
    std::lock_guard<std::mutex> lock(loggerMutex_);
    auto it = activeLoggers_.find(clientId);
    if (it != activeLoggers_.end()) {
        it->second->stop();
        activeLoggers_.erase(it);
    }
}

std::string LogManager::getLastLogFilePath(const std::string& clientId) {
    std::lock_guard<std::mutex> lock(loggerMutex_);

    // Return active logger path if running
    auto it = activeLoggers_.find(clientId);
    if (it != activeLoggers_.end()) {
        return it->second->getFilePath();
    }

    // Otherwise return cached last log path
    auto pathIt = lastLogFilePathMap_.find(clientId);
    if (pathIt != lastLogFilePathMap_.end()) {
        return pathIt->second;
    }

    return "";
}

std::string LogManager::createNewLogFilePath(const std::string& clientId) {
    fs::path logsDir = getLogsDir();
    if (!fs::exists(logsDir)) {
        fs::create_directories(logsDir);
    }

    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    char timeStr[32];
    std::strftime(timeStr, sizeof(timeStr), "%Y%m%d_%H%M%S", std::localtime(&now_c));

    std::string filename = clientId + "_" + timeStr + ".csv";
    return (logsDir / filename).string();
}

std::string LogManager::getLogsDir() const {
    return (fs::current_path() / "logs").string();
}
