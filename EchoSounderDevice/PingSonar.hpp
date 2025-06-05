#pragma once

#include <boost/asio.hpp>
#include <thread>
#include <atomic>
#include <functional>
#include <iostream>

class SonarPingReader {
public:
    using Callback = std::function<void(uint32_t distance, uint16_t confidence)>;

    SonarPingReader(const std::string& port, unsigned int baud_rate)
        : port_name(port), baud_rate(baud_rate), stop_flag(false) {}

    void start(Callback callback) {
        stop_flag = false;
        reader_thread = std::thread(&SonarPingReader::run, this, callback);
    }

    void stop() {
        stop_flag = true;
        if (reader_thread.joinable())
            reader_thread.join();
    }

    ~SonarPingReader() {
        stop();
    }

private:
    std::string port_name;
    unsigned int baud_rate;
    std::atomic<bool> stop_flag;
    std::thread reader_thread;

    static constexpr uint8_t START_BYTE_1 = 0x42;
    static constexpr uint8_t START_BYTE_2 = 0x52;
    static constexpr uint16_t MESSAGE_ID_DISTANCE = 1212;
    static constexpr size_t HEADER_SIZE = 8;  // B B H H B B
    static constexpr size_t PAYLOAD_SIZE = 22;
    static constexpr size_t CHECKSUM_SIZE = 2;

    void run(Callback callback) {
        try {
            boost::asio::io_service io;
            boost::asio::serial_port serial(io, port_name);

            serial.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
            serial.set_option(boost::asio::serial_port_base::character_size(8));
            serial.set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
            serial.set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
            serial.set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));

            std::vector<uint8_t> buffer(HEADER_SIZE + PAYLOAD_SIZE + CHECKSUM_SIZE);

            while (!stop_flag) {
                boost::asio::read(serial, boost::asio::buffer(buffer, buffer.size()));

                // Validate start bytes
                if (buffer[0] != START_BYTE_1 || buffer[1] != START_BYTE_2)
                    continue;

                uint16_t length = buffer[2] | (buffer[3] << 8);
                uint16_t message_id = buffer[4] | (buffer[5] << 8);

                if (message_id != MESSAGE_ID_DISTANCE || length != PAYLOAD_SIZE)
                    continue;

                uint16_t received_checksum = buffer[HEADER_SIZE + PAYLOAD_SIZE] | (buffer[HEADER_SIZE + PAYLOAD_SIZE + 1] << 8);
                uint16_t calculated_checksum = 0;
                for (size_t i = 0; i < HEADER_SIZE + PAYLOAD_SIZE; ++i)
                    calculated_checksum += buffer[i];

                if ((calculated_checksum & 0xFFFF) != received_checksum)
                    continue;

                // Extract fields
                const uint8_t* payload = &buffer[HEADER_SIZE];
                uint32_t distance = *(uint32_t*)&payload[0];
                uint16_t confidence = *(uint16_t*)&payload[4];

                if (callback)
                    callback(distance, confidence);
            }

        } catch (const std::exception& e) {
            std::cerr << "Error in sonar thread: " << e.what() << std::endl;
        }
    }
};
