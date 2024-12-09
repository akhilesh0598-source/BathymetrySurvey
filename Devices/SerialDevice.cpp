#include "SerialDevice.hpp"

SerialDevice::SerialDevice(boost::asio::io_context &io_context, std::string devicePort, uint device_baudrate)
    : serial_port(io_context), device_baudrate(device_baudrate), is_device_running(false)
{
    try
    {
        if (!serial_port.is_open())
            serial_port.open(this->device_port);
        serial_port.set_option(boost::asio::serial_port::baud_rate(device_baudrate));
        serial_port.set_option(boost::asio::serial_port::parity::none);
        serial_port.set_option(boost::asio::serial_port::character_size(8));
        serial_port.set_option(boost::asio::serial_port::flow_control::none);
        serial_port.set_option(boost::asio::serial_port::stop_bits::one);
        this->is_device_running = true;
    }
    catch (std::exception ex)
    {
        std::cerr << "File Name: " __FILE__ << " Method Name: " << __func__ << "Error: " << this->device_port << " is failed to initialize and exception message is " + std::string(ex.what());
        this->is_device_running = false;
    }
}

std::string SerialDevice::AsyncReadDevice()
{
    if (!this->is_device_running)
    {
        std::cerr << "File Name: " __FILE__ << " Method Name: " << __func__ << "Error: " << this->device_port << " is not readable.";
        return "";
    }
    boost::asio::async_read(this->serial_port, boost::asio::buffer(*this->buffer_),
                            [this, buffer = buffer_](const boost::system::error_code &error, std::size_t bytes_transferred)
                            {
                                this->ReadHandler(error, bytes_transferred, buffer);
                            });
}

void SerialDevice::ReadHandler(const boost::system::error_code &error, std::size_t bytes_transferred, std::shared_ptr<std::array<char,1024>> buffer)
{
    if (!error)
    {
        std::string data(buffer->begin(), buffer->begin() + bytes_transferred);
        OnDataReceived(data);
        AsyncReadDevice();
    }
    else if (error != boost::asio::error::operation_aborted)
    {
        std::cerr << "File Name: " __FILE__ << " Method Name: " << __func__ << "Error: " << this->device_port << " is failed in reading.";
    }
}

void SerialDevice::OnDataReceived(const std::string& received_data)
{
    std::cout << this->device_port <<" Data " << received_data<<std::endl;

}