#include "LogDevicesData.hpp"


LogData::LogData(const std::string& logFilePath)
    : logFilePath(logFilePath), stopLogging(false) {}

LogData::~LogData() {
    stop();
}

void LogData::start() {
    logThread = std::thread(&LogData::run, this);
}

void LogData::stop() {
    stopLogging = true;
    if (logThread.joinable()) {
        logThread.join();
    }
}

void LogData::run() {
    try {
        auto logger = spdlog::basic_logger_mt("csv_logger", logFilePath);
        logger->set_pattern("%v");  // Only message (CSV content)

        // CSV header
        logger->info("timestamp,latitude,longitude,distance,confidence");

        while (!stopLogging) {
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
        }

        spdlog::drop("csv_logger");
    } catch (const spdlog::spdlog_ex& ex) {
        std::cerr << "Log error: " << ex.what() << std::endl;
    }
}
