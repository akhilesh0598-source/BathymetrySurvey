#pragma once

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/thread.hpp>
#include <string>
#include <vector>
#include <mutex>
#include <iostream>
#include <sstream>

extern std::string gpsDateTime;
extern double gpsLatitude;
extern double gpsLongitude;
extern boost::mutex gps_mutex;

class GPSDevice {
public:
    GPSDevice(const std::string& device_port, uint32_t baudrate);

    bool Start();
    void Stop();

private:
    static constexpr std::size_t SERIAL_PORT_READ_BUF_SIZE = 512;

    boost::asio::io_context io_context;
    boost::asio::serial_port serial_port;
    std::string device_port;
    uint32_t baudrate;
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guard_;

    char read_buf_raw_[SERIAL_PORT_READ_BUF_SIZE];
    std::string read_buf_str;

    boost::mutex mutex_;
    std::thread io_thread;

    void async_read_some();
    void on_receive(const boost::system::error_code& ec, std::size_t bytes_transferred);

    void parseGPSLine(std::string& line);
    double dm_to_dd_latitude(const std::string& latitude);
    double dm_to_dd_longitude(const std::string& longitude);
    std::vector<std::string> splitLine(const std::string& line);
};
