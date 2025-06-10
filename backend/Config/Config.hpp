#pragma once
#include <string>
#include<mutex>
#include <shared_mutex>  // for std::shared_mutex

struct Config {
    int WebSocketPort;
    int HttpServerPort;
    std::string PingDevicePort;
    int PingDeviceBaudRate;
    std::string GPSDevicePort;
    int GPSDeviceBaudRate;

private:
    std::string LogFilePath_;
    mutable std::shared_mutex logFileMutex;  // shared_mutex instead of mutex

public:
    Config();

    // Thread-safe getter and setter for LogFilePath using shared_mutex
    std::string getLogFilePath() const;
    void setLogFilePath(const std::string& path);
};
