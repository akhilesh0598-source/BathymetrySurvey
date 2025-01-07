#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <thread>
#include <chrono>

#include "../../WebSocket/WebSocketResponseMessage/WebSocketResponseMessage.hpp"

std::string gpsDateTime = "";
double gpsLatitude = 0;
double gpsLongitude = 0;
int pingDeviceDistance=0;
int pingDeviceConfidence=0;

std::vector<std::string> splitLine(std::string &line, char delimeter = ',')
{
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string token;
    while (std::getline(ss, token, delimeter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

double dm_to_dd_latitude(std::string latitude) // DD means decimal degrees
{
    std::string first_part = latitude.substr(0, 2);
    std::string second_part = latitude.substr(2, latitude.length() - 2);
    double latitude_in_dd = std::stod(first_part) + (std::stod(second_part)) / 60;
    return latitude_in_dd;
}

double dm_to_dd_longitude(std::string longitude)
{
    std::string first_part = longitude.substr(0, 3);
    std::string second_part = longitude.substr(3, longitude.length() - 3);
    double longitude_in_dd = std::stod(first_part) + (std::stod(second_part)) / 60;
    return longitude_in_dd;
}

void parseGPSLine(std::string &line)
{
    try
    {
        if (!line.empty() && (line[0] == '\r' || line[0] == '\n')) {
            line.erase(0, 1); 
        }
        std::vector<std::string> tokens = splitLine(line);
        std::string identifier = "$GPGGA";
        if (tokens.empty() || tokens[0].compare(identifier) != 0)
        {
            return;
        }
        //std::cout << line << std::endl;
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
