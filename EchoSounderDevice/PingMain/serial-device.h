//#pragma once
#ifndef SERIAL_DEVICE_H
#define SERIAL_DEVICE_H

#include <array>
#include <atomic>
#include <future>
#include <memory>
#include <mutex>
#include <string>

#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/signals2.hpp>

#include "ping-port.h"

#include <iostream>

class SerialLink : public PingPort {
public:

    SerialLink(const std::string& port, uint32_t baudrate);
    ~SerialLink();
    virtual void close() override final { _serialPort.close(); };
    virtual bool isOpen() override final { return _serialPort.is_open(); };
    virtual int read(uint8_t* buffer, int nBytes) override final;
    virtual int write(const uint8_t* data, int nBytes) override final;
    boost::signals2::connection doOnReceived(std::function<void(std::vector<uint8_t>)> slot)
    {
        return _onReceived.connect(slot);
    }
private:
    SerialLink(const SerialLink&) = delete;
    const SerialLink& operator=(const SerialLink&) = delete;

    /**
     * @brief Start event loop process
     *
     */
    void runEventLoop();

    /**
     * @brief Schedule an async read event
     *
     */
    void bindRead();

    /**
     * @brief Async event callback
     *
     * @param error Error code
     * @param bytesReceived Number of bytes received
     */
    void doRead(boost::system::error_code error, size_t bytesReceived);

    /**
     * @brief Deal with the async event loop
     *  run: Will control if the event loop will run or not
     *  future: Will check if the event loop was able to finish
     */
    struct {
        std::atomic<bool> run;
        boost::asio::io_context eventLoop;
        std::future<void> future;
    } _context;

    boost::asio::serial_port _serialPort;

    // Buffer used to async read
    std::vector<uint8_t> _rxBuffer;

    /**
     * @brief Buffer used to transfer data between async read and our public `read` function.
     *  This is necessary to avoid any problem of simultaneous `read` call while async read is received.
     */
    struct {
        std::vector<uint8_t> data;
        std::mutex mutex;
    } _linkBuffer;

    protected:
    boost::signals2::signal<void(std::vector<uint8_t>)> _onReceived;
    
};

#endif
