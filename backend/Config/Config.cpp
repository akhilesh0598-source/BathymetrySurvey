#include "Config.hpp"

Config::Config()
    : WebSocketPort(0), HttpServerPort(0), PingDeviceBaudRate(0), GPSDeviceBaudRate(0) {
    // Initialize other members if needed
}

std::string Config::getLogFilePath() const {
    std::shared_lock<std::shared_mutex> lock(logFileMutex);  // shared lock for reading
    return LogFilePath_;
}

void Config::setLogFilePath(const std::string& path) {
    std::unique_lock<std::shared_mutex> lock(logFileMutex);  // exclusive lock for writing
    LogFilePath_ = path;
}
