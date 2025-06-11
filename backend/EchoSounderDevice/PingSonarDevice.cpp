#include "PingSonarDevice.hpp"

PingSonarDevice::PingSonarDevice(const std::string &port, uint32_t baudrate)
    : port_name_(port),
      baud_rate_(baudrate),
      io_context_(),
      serial_port_(io_context_),
      stop_flag_(false) {}

bool PingSonarDevice::Start()
{
    try
    {
        if (!serial_port_.is_open())
        {
            serial_port_.open(port_name_);
        }

        serial_port_.set_option(boost::asio::serial_port_base::baud_rate(baud_rate_));
        serial_port_.set_option(boost::asio::serial_port_base::character_size(8));
        serial_port_.set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
        serial_port_.set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
        serial_port_.set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));

        stop_flag_ = false;

        // Run io_context in a separate thread
        io_thread_ = std::thread([this]()
                                 {
            std::cout << "io_context running thread started for Sonar Device\n";
            std::cout << "Reading started for Sonar Device!" << std::endl;
            this->run();
            io_context_.run();
            std::cout << "io_context run() finished for GPS Device\n"; });

        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "PingSonar Start failed: " << e.what() << std::endl;
        return false;
    }
}

void PingSonarDevice::Stop()
{
    stop_flag_ = true;

    serial_port_.cancel();
    serial_port_.close();

    io_context_.stop();

    if (io_thread_.joinable())
    {
        io_thread_.join();
    }
    
    std::cout << "Sonar device stopped reading." << std::endl;
}

void PingSonarDevice::run()
{
    if (stop_flag_)
        return;

    boost::asio::async_read(
        serial_port_,
        boost::asio::buffer(buffer_),
        [this](const boost::system::error_code &ec, std::size_t bytes_transferred)
        {
            if (!ec && bytes_transferred == TOTAL_SIZE)
            {
                if (buffer_[0] == START_BYTE_1 && buffer_[1] == START_BYTE_2)
                {
                    uint16_t length = buffer_[2] | (buffer_[3] << 8);
                    uint16_t message_id = buffer_[4] | (buffer_[5] << 8);

                    if (message_id == MESSAGE_ID_DISTANCE && length == PAYLOAD_SIZE)
                    {
                        uint16_t received_checksum = buffer_[HEADER_SIZE + PAYLOAD_SIZE] |
                                                     (buffer_[HEADER_SIZE + PAYLOAD_SIZE + 1] << 8);
                        uint16_t calculated_checksum = 0;
                        for (size_t i = 0; i < HEADER_SIZE + PAYLOAD_SIZE; ++i)
                            calculated_checksum += buffer_[i];

                        if ((calculated_checksum & 0xFFFF) == received_checksum)
                        {
                            const uint8_t *payload = &buffer_[HEADER_SIZE];
                            uint32_t distance = *(uint32_t *)&payload[0];
                            uint16_t confidence = *(uint16_t *)&payload[4];

                            boost::mutex::scoped_lock lock(sonar_mutex);
                            sonarDistance = distance;
                            sonarConfidence = confidence;
                        }
                    }
                }
            }
            else if (ec)
            {
                if (!stop_flag_)
                {
                    if (ec == boost::asio::error::eof)
                        std::cerr << "PingSonar read stopped (EOF).\n";
                    else
                        std::cerr << "PingSonar async read error: " << ec.message() << std::endl;
                }
                return;
            }

            // Continue async read
            if (!stop_flag_)
            {
                run();
            }
        });
}
