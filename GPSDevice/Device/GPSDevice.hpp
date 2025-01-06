#ifndef GPSDEVICE_HPP
#define GPSDEVICE_HPP

#include <iostream>
#include <boost/asio/io_context.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind/bind.hpp>
#include <thread>

#include "../../WebSocket/WebSocketResponseMessage/WebSocketResponseMessage.hpp"
#include "../../Utility/Utility.hpp"

#define SERIAL_PORT_READ_BUF_SIZE 1024


class GPSDevice
{
private:
    boost::asio::io_context io_context;
    boost::asio::serial_port serial_port;
    std::string device_port;
    uint baudrate;
    char read_buf_raw_[SERIAL_PORT_READ_BUF_SIZE];
    std::string read_buf_str;
    boost::mutex mutex_;
    char end_of_line_char_ = 0x0d;

public:
    GPSDevice(std::string device_port, uint baudrate);
    bool Start();
    void Stop();

private:
    void async_read_some();
    void on_receive(const boost::system::error_code &ec, size_t bytes_transferred);
    double dm_to_dd_latitude(std::string latitude);
    double dm_to_dd_longitude(std::string longitude);
    void parseGPSLine(std::string &line);
};
#endif