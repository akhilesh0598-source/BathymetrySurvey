#ifndef GPS_HPP
#define GPS_HPP

#include <iostream>
#include "SerialDevice.hpp"

class GPS : public SerialDevice
{
private:
    std::string device_name;

public:
    GPS(std::string port_name, uint baudrate);
    
};

#endif