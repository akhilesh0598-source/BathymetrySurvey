#include "GPSDevice.hpp"



GPSDevice::GPSDevice(std::string device_port, uint baudrate)
    : serial_port(io_context), device_port(device_port), baudrate(baudrate)
{
}

bool GPSDevice::Start()
{
    if (!serial_port.is_open())
        serial_port.open(device_port);

    serial_port.set_option(boost::asio::serial_port_base::baud_rate(baudrate));
    serial_port.set_option(boost::asio::serial_port_base::character_size(8));
    serial_port.set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));
    serial_port.set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
    serial_port.set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));

    std::thread t([&]()
                  { io_context.run(); });
    t.detach();
    async_read_some();
    return true;
}

void GPSDevice::async_read_some()
{
    if (!serial_port.is_open())
        return;
    serial_port.async_read_some(boost::asio::buffer(read_buf_raw_, SERIAL_PORT_READ_BUF_SIZE),
                                boost::bind(&GPSDevice::on_receive, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void GPSDevice::on_receive(const boost::system::error_code &ec, size_t bytes_transferred)
{
    boost::mutex::scoped_lock look(mutex_);
    if (!serial_port.is_open())
        return;

    if (ec)
    {
        std::cerr << "Error: " << ec.what() << std::endl;
        async_read_some();
        return;
    }

    for (int i = 0; i < bytes_transferred; i++)
    {
        char c = read_buf_raw_[i];
        if (c == end_of_line_char_)
        {
            parseGPSLine(read_buf_str);
            read_buf_str.clear();
        }
        else
        {
            read_buf_str += c;
        }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    async_read_some();
}

void GPSDevice::Stop()
{
    boost::mutex::scoped_lock look(mutex_);
    if (serial_port.is_open())
    {
        serial_port.cancel();
        serial_port.close();
    }
    io_context.stop();
    io_context.reset();
}

double GPSDevice::dm_to_dd_latitude(std::string latitude) // DD means decimal degrees
{
    std::string first_part = latitude.substr(0, 2);
    std::string second_part = latitude.substr(2, latitude.length() - 2);
    double latitude_in_dd = std::stod(first_part) + (std::stod(second_part)) / 60;
    return latitude_in_dd;
}

double GPSDevice::dm_to_dd_longitude(std::string longitude)
{
    std::string first_part = longitude.substr(0, 3);
    std::string second_part = longitude.substr(3, longitude.length() - 3);
    double longitude_in_dd = std::stod(first_part) + (std::stod(second_part)) / 60;
    return longitude_in_dd;
}

void GPSDevice::parseGPSLine(std::string &line)
{
    try
    {
        // line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
        std::vector<std::string> tokens = splitLine(line);
        std::string identifier = "$GPGGA";
        if (tokens.empty() || tokens[0].compare(identifier) != 0)
        {
            return;
        }
        std::cout << line << std::endl;
        std::string utc_time = tokens[1].substr(0, 2) + ":" + tokens[1].substr(2, 2) + ":" + tokens[1].substr(4, 2);

        double latitude = dm_to_dd_latitude(tokens[2]);
        latitude = tokens[3] == "S" ? latitude * -1 : latitude;

        double longitude = dm_to_dd_longitude(tokens[4]);
        longitude = tokens[5] == "W" ? longitude * -1 : longitude;

        gpsDateTime = utc_time;
        gpsLatitude = latitude;
        gpsLongitude = longitude;
        // std::this_thread::sleep_for(std::chrono::milliseconds(10));
        std::cout << "time: " << utc_time << "\n";
        std::cout << "latitude: " << latitude << "\n";
        std::cout << "longitude: " << longitude << "\n";
    }
    catch (std::exception ex)
    {
    }
}
