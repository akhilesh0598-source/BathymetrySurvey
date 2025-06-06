#pragma once

#include<iostream>
#include <string>
#include <thread>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>


#include "../GlobalVariables/GlobalDevicesData.hpp"

class LogData {
public:
    explicit LogData(const std::string& logFilePath);
    ~LogData();

    void start();
    void stop();

private:
    void run();

    std::string logFilePath;
    std::thread logThread;
    std::atomic<bool> stopLogging;
};

