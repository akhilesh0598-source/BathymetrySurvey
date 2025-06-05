#pragma once

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <string>
#include <vector>
#include <mutex>

class GPSDevice {
public:
    GPSDevice(std::string device_port, uint32_t baudrate);
    bool Start();
    void Stop();

private:
    static constexpr size_t SERIAL_PORT_READ_BUF_SIZE = 512;
    boost::asio::io_context io_context;
    boost::asio::serial_port serial_port;
    std::string device_port;
    uint32_t baudrate;

    char read_buf_raw_[SERIAL_PORT_READ_BUF_SIZE];
    std::string read_buf_str;
    boost::mutex mutex_;
    std::thread io_thread;

    void async_read_some();
    void on_receive(const boost::system::error_code& ec, size_t bytes_transferred);
    void parseGPSLine(std::string& line);
    double dm_to_dd_latitude(std::string latitude);
    double dm_to_dd_longitude(std::string longitude);

    std::vector<std::string> splitLine(const std::string& line);
};
