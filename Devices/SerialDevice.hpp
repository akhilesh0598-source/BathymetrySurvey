#ifndef SERIALDEVICE_HPP
#define SERIALDEVICE_HPP

#include <iostream>
#include <boost/asio/io_context.hpp>
#include <boost/asio/serial_port.hpp>

class SerialDevice
{
private:
    boost::asio::serial_port serial_port;
    std::string device_port;
    uint baudrate;
    bool is_running;

public:
    // Default length of reading character size in one time.
    uint one_time_read_length = 1024; 

    SerialDevice(boost::asio::io_context &io_context, std::string device_port, uint baudrate);
    void Initialize();
    std::string ReadOneLine();
};
#endif