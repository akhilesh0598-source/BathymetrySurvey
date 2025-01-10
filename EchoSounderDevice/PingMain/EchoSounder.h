#include <iostream>
#include "../device/ping-device-ping1d.h"
#include "../message/ping-message-ping1d.h"
#include "serial-device.h"
#include "ping-time.h"

SerialLink serialLink("/dev/ttyUSB1",115200);
Ping1d ping1d(serialLink);

void initializePingDevice()
{
    if (ping1d.initialize(100))
    {
        std::cout << "initialized successfully!" << std::endl;
    }
    if (ping1d.set_mode_auto(true))
    {
        std::cout << "automode is set" << std::endl;
    }
    if (ping1d.set_speed_of_sound(343000)) // Speed of sound in air- 343000, Speed of sound in water- 1550000
    {
        std::cout << "speed of sound is set" << std::endl;
    }
    if (ping1d.set_gain_setting(1))
    {
        std::cout << "gain settign is set" << std::endl;
    }
    if (ping1d.set_ping_enable(1))
    {
        std::cout << "gain settign is set" << std::endl;
    }
}

void closePingDevice()
{
    if(serialLink.isOpen())
        serialLink.close();
}