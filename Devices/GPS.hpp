#ifndef GPS_HPP
#define GPS_HPP

#include <iostream>
#include "SerialDevice.hpp"

class GPS : public SerialDevice
{
private:
    std::string device_name="GPS";

public:
    GPS(boost::asio::io_context &io_context,std::string port_name, uint baudrate);
    void OnDataReceived(const std::string& received_data) override;
};

#endif