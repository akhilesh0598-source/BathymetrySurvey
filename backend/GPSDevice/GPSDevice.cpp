#include "GPSDevice.hpp"

// Constructor
GPSDevice::GPSDevice(const std::string &device_port, uint32_t baudrate)
    : serial_port(io_context), device_port(device_port), baudrate(baudrate),work_guard_(boost::asio::make_work_guard(io_context))
{
    std::cout << "GPS device initialized!" << std::endl;
}

// Start the GPS device communication
bool GPSDevice::Start()
{
    if (!serial_port.is_open())
    {
        serial_port.open(device_port);
        if (serial_port.is_open())
        {
            std::cout << "Serial port opened successfully\n";
        }
        else
        {
            std::cerr << "Failed to open serial port\n";
            return false;
        }
    }

    serial_port.set_option(boost::asio::serial_port_base::baud_rate(baudrate));
    serial_port.set_option(boost::asio::serial_port_base::character_size(8));
    serial_port.set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));
    serial_port.set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
    serial_port.set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));

    io_thread_=std::thread([this]()
                {
    std::cout << "io_context running thread started\n";
    io_context.run();
    std::cout << "io_context run() finished\n"; });

    std::cout << "Reading started!" << std::endl;
    async_read_some();
    return true;
}

// Stop the GPS device communication
void GPSDevice::Stop()
{
    boost::mutex::scoped_lock lock(mutex_);

    if (serial_port.is_open())
    {
        serial_port.cancel();
        serial_port.close();
    }

    work_guard_.reset();  // Allow io_context to stop when done
    io_context.stop();
    if(io_thread_.joinable())
    {
        io_thread_.join();
    }
    io_context.reset();
    std::cout << "GPS device stopped." << std::endl;

}

// Begin asynchronous read
void GPSDevice::async_read_some()
{
    if (!serial_port.is_open())
        return;

    serial_port.async_read_some(
        boost::asio::buffer(read_buf_raw_, SERIAL_PORT_READ_BUF_SIZE),
        boost::bind(&GPSDevice::on_receive, this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
}

// Callback for receiving data
void GPSDevice::on_receive(const boost::system::error_code &ec, size_t bytes_transferred)
{
    boost::mutex::scoped_lock lock(mutex_);

    if (!serial_port.is_open())
        return;

    if (ec)
    {
        std::cerr << "Error: " << ec.what() << std::endl;
        async_read_some();
        return;
    }
    for (size_t i = 0; i < bytes_transferred; ++i)
    {
        char c = read_buf_raw_[i];
        if (c == '\r' || c == '\n')
        {
            parseGPSLine(read_buf_str);
            read_buf_str.clear();
        }
        else
        {
            read_buf_str += c;
        }
    }
    async_read_some();
}

// Parse a line from GPS and update global variables
void GPSDevice::parseGPSLine(std::string &line)
{
    try
    {
        if (line.substr(0, 6) != "$GPGGA")
            return;

        std::vector<std::string> tokens = splitLine(line);
        if (tokens.size() < 6)
            return;

        // Parse UTC Time
        std::string utc_time = tokens[1].substr(0, 2) + ":" +
                               tokens[1].substr(2, 2) + ":" +
                               tokens[1].substr(4, 2);

        // Convert latitude
        double latitude = dm_to_dd_latitude(tokens[2]);
        if (tokens[3] == "S")
            latitude *= -1;

        // Convert longitude
        double longitude = dm_to_dd_longitude(tokens[4]);
        if (tokens[5] == "W")
            longitude *= -1;

        boost::mutex::scoped_lock lock(gps_mutex);
        // Update global variables
        gpsDateTime = utc_time;
        gpsLatitude = latitude;
        gpsLongitude = longitude;
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Parsing error: " << ex.what() << std::endl;
    }
}

// Convert Latitude from Degrees Minutes (DM) to Decimal Degrees (DD)
double GPSDevice::dm_to_dd_latitude(const std::string &latitude)
{
    std::string deg = latitude.substr(0, 2);
    std::string min = latitude.substr(2);
    return std::stod(deg) + std::stod(min) / 60.0;
}

// Convert Longitude from Degrees Minutes (DM) to Decimal Degrees (DD)
double GPSDevice::dm_to_dd_longitude(const std::string &longitude)
{
    std::string deg = longitude.substr(0, 3);
    std::string min = longitude.substr(3);
    return std::stod(deg) + std::stod(min) / 60.0;
}

std::vector<std::string> GPSDevice::splitLine(const std::string &line)
{
    std::vector<std::string> result;
    std::stringstream ss(line);
    std::string token;

    while (std::getline(ss, token, ','))
    {
        result.push_back(token);
    }

    return result;
}