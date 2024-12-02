#ifndef GPS_HPP
#define GPS_HPP

#include<iostream>

class GPS
{
    private:
    std::string portName;
    uint baudrate;
    
    
    public:
    GPS(std::string portName,uint baudrate);
    void OpenDevice()
};


#endif