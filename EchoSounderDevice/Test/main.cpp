#include <iostream>
#include "../PingMain/EchoSounder.h"

int main()
{
    initializePingDevice();
    int i = 0;
    while (i < 10000)
    {
        ping_message *response = ping1d.request(Ping1dId::DISTANCE, 1000);
        if (response == nullptr)
        {
            std::cerr << "Failed to receive distance data" << std::endl;
        }
        if (ping1d.distance_data.confidence >= 0)
        {
            std::cout << "Depth: " << ping1d.distance_data.distance << " mm" << std::endl;
            std::cout << "Confidence: " << static_cast<int>(ping1d.distance_data.confidence) << "%" << std::endl;
        }

        i++;
        PingTime::microsecondDelay(1000);
    }
    closePingDevice();

    return 0;
}