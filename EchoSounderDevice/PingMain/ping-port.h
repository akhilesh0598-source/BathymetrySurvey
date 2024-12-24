//#pragma once
#ifndef PING_PORT_H
#define PING_PORT_H

#include <stdint.h>

// port interface used by PingDevice class
class PingPort {
public:
    virtual void close()=0;
    virtual bool isOpen()=0;
    virtual int read(uint8_t* buffer, int nBytes) = 0;
    virtual int write(const uint8_t* data, int nBytes) = 0;
};

#endif