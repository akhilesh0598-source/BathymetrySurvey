#pragma once

#include<iostream>
#include<boost/asio/io_context.hpp>
#include<boost/asio/serial_port.hpp>

#define SERIAL_PORT_READ_BUF_SIZE 256

class GPSDevice
{
    private:
        boost::asio::io_context io_context;
        boost::asio::serial_port serial_port;
        std::string device_port;
        uint baudrate;
        char read_buf_raw_[SERIAL_PORT_READ_BUF_SIZE];
        std::string read_buf_str;
        
    public:
        GPSDevice(std::string device_port,uint baudrate);
        bool Start();
        void Stop();

    private:
        void async_read_some();
        void on_receive();

};