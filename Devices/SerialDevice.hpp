#ifndef SERIALDEVICE_HPP
#define SERIALDEVICE_HPP

#include <iostream>
#include <boost/asio/io_context.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/asio/read.hpp>

class SerialDevice
{
private:
    boost::asio::serial_port serial_port;
    std::string device_port;
    uint device_baudrate;
    bool is_device_running;
    std::shared_ptr<std::array<char,1024>> buffer_;
    void ReadHandler(const boost::system::error_code& error, std::size_t bytes_transferred, std::shared_ptr<std::array<char,1024>> buffer);
public:
    // Default length of reading character size in one time.
    uint one_time_read_length = 1024; 

    SerialDevice(boost::asio::io_context &io_context, std::string device_port, uint device_baudrate);
    std::string AsyncReadDevice();
protected:
    virtual void OnDataReceived(const std::string& data);
};
#endif