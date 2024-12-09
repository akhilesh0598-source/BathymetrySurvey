#include "GPS.hpp"

GPS::GPS(boost::asio::io_context &io_context,std::string device_port,uint baudrate) : SerialDevice(io_context,device_port,baudrate)
{
}
void GPS::OnDataReceived(const std::string& received_data) 
{
    std::cout<<"GPS data "<<received_data;
}

