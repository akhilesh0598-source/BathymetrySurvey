#include "SerialDevice.hpp"

SerialDevice::SerialDevice(boost::asio::io_context &io_context, std::string devicePort, uint baudrate)
    :serial_port(io_context), baudrate(baudrate)
{
    this->is_running=false;
}

void SerialDevice::Initialize()
{
    if(!serial_port.is_open())
        serial_port.open(this->device_port);
    serial_port.set_option(boost::asio::serial_port::baud_rate(baudrate));
    serial_port.set_option(boost::asio::serial_port::parity::none);
    serial_port.set_option(boost::asio::serial_port::character_size(8));
    serial_port.set_option(boost::asio::serial_port::flow_control::none);
    serial_port.set_option(boost::asio::serial_port::stop_bits::one);
    this->is_running=true;
}

std::string SerialDevice::ReadOneLine()
{
    if(!this->is_running)
    {
        std::cerr<<"File Name: " __FILE__<< " Method Name: " <<__func__<< "Error: "<<this->device_port<<" is not readable.";
        return "";
    }
    std::string reading_data;
    serial_port.async_read_some(boost::asio::buffer(reading_data,this->one_time_read_length),handler);
    return reading_data;
}

void handler(
  const boost::system::error_code& error, // Result of operation.
  std::size_t bytes_transferred           // Number of bytes read.
);
