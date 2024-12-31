#include<iostream>
#include "../EchoSounderDevice/PingMain/EchoSounder.h"
#include "../WebSocket/WebSocketResponseMessage/WebSocketResponseMessage.hpp"
#include "../WebSocket/WebSocketServer.hpp"
#include "../GPSDevice/Device/gps_reader.cpp"

int pingDeviceDistance=0;
int pingDeviceConfidence=0;

#define path "/home/akhilesh/Desktop/Working/Planys_Project_Bathymatric/bathymetric-core/Main/gps_data.txt"

int readGPSDeviceData()
{
    read_gps(path);
    return 0;
}

int readPingDevicedata()
{
    initializePingDevice();
    while (true)
    {
        ping_message *response = ping1d.request(Ping1dId::DISTANCE, 1000);
        if (response == nullptr)
        {
            std::cerr << "Failed to receive distance data" << std::endl;
        }
        if (ping1d.distance_data.confidence >= 0)
        {
            pingDeviceDistance = ping1d.distance_data.distance;
            pingDeviceConfidence = static_cast<int>(ping1d.distance_data.confidence);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(90));
    }
    closePingDevice();
}

int main(int argc, char *argv[])
{
    
    std::thread PingDeviceThread(readPingDevicedata);
    std::thread GPSDeviceThread(readGPSDeviceData);
    auto const address = boost::asio::ip::make_address("127.0.0.1");
    auto const port = static_cast<unsigned short>(5001);
    auto const threads = 1;
    boost::asio::io_context ioc{threads};

    std::make_shared<listener>(ioc, boost::asio::ip::tcp::endpoint{address, port})->run();

    ioc.run();
    
    return 0;
}
