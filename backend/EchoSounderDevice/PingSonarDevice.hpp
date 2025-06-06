#pragma once

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <thread>
#include <vector>
#include <atomic>
#include <iostream>

#include "../GlobalVariables/GlobalDevicesData.hpp"

class PingSonarDevice {
public:
    PingSonarDevice(const std::string& port, uint32_t baudrate);

    // Start the sonar device
    bool Start();

    // Stop the sonar device
    void Stop();

private:
    // Constants for the protocol
    static constexpr uint8_t START_BYTE_1 = 0x42;
    static constexpr uint8_t START_BYTE_2 = 0x52;
    static constexpr uint16_t MESSAGE_ID_DISTANCE = 1212;
    static constexpr size_t HEADER_SIZE = 8;
    static constexpr size_t PAYLOAD_SIZE = 22;
    static constexpr size_t CHECKSUM_SIZE = 2;
    static constexpr size_t TOTAL_SIZE = HEADER_SIZE + PAYLOAD_SIZE + CHECKSUM_SIZE;

    // Configuration
    std::string port_name_;
    uint32_t baud_rate_;

    // Asio and threading
    boost::asio::io_context io_context_;
    boost::asio::serial_port serial_port_;
    std::vector<uint8_t> buffer_{std::vector<uint8_t>(TOTAL_SIZE)};
    std::atomic<bool> stop_flag_;
    std::thread io_thread_;

    // Async run method
    void run();
};
