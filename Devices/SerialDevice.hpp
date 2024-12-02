#ifndef SERIALDEVICE_HPP
#define SERIALDEVICE_HPP

#include <iostream>
#include<boost/asio/io_context.hpp>
#include<boost/asio/serial_port.hpp>

class SerialDevice
{
    private:
    boost::asio::io_context io_context;
    boost::asio::serial_port serial_port;

    public:

};
#endif