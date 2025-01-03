#include <iostream>
#include <boost/asio/io_context.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/asio/placeholders.hpp>
#include<boost/thread/thread.hpp>
#include <thread>
#include <boost/bind/bind.hpp>
#include "./gps_parser.cpp"

#define SERIAL_PORT_READ_BUF_SIZE 1024

class GPSDevice
{
public:
    GPSDevice(std::string device_port, uint baudrate)
        : serial_port(io_context), device_port(device_port), baudrate(baudrate)
    {
    }

    bool Start()
    {
        if (!serial_port.is_open())
            serial_port.open(device_port);

        serial_port.set_option(boost::asio::serial_port_base::baud_rate(baudrate));
        serial_port.set_option(boost::asio::serial_port_base::character_size(8));
        serial_port.set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));
        serial_port.set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
        serial_port.set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));

        std::thread t([&](){ io_context.run(); });
        t.detach();
        async_read_some();
        return true;
    }

    void async_read_some()
    {
        if (!serial_port.is_open())
            return;
        serial_port.async_read_some(boost::asio::buffer(read_buf_raw_, SERIAL_PORT_READ_BUF_SIZE),
                                    boost::bind(&GPSDevice::on_receive, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }

    void on_receive(const boost::system::error_code &ec, size_t bytes_transferred)
    {
        boost::mutex::scoped_lock look(mutex_);
        if (!serial_port.is_open())
            return;

        if (ec)
        {
            std::cerr<<"Error: "<<ec.what()<<std::endl;
            async_read_some();
            return;
        }

        //std::cout<<"Data is "<<std::string(read_buf_raw_)<<std::endl;
        
        for (int i = 0; i < bytes_transferred; i++)
        {
            char c = read_buf_raw_[i];
            if (c == end_of_line_char_)
            {
                parseGPSLine(read_buf_str);
                read_buf_str.clear();
            }
            else
            {
                read_buf_str += c;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        async_read_some();
    }

    void stop()
    {
        boost::mutex::scoped_lock look(mutex_);
        if (serial_port.is_open())
        {
            serial_port.cancel();
            serial_port.close();
        }
        io_context.stop();
        io_context.reset();
    }

private:
    boost::asio::io_context io_context;
    boost::asio::serial_port serial_port;
    std::string device_port;
    uint baudrate;
    char read_buf_raw_[SERIAL_PORT_READ_BUF_SIZE];
    std::string read_buf_str;
    boost::mutex mutex_;
    char end_of_line_char_=0x0d;
};