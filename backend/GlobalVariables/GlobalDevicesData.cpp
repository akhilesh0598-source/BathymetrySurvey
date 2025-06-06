#include "GlobalDevicesData.hpp"

uint32_t sonarDistance = 0;        // definition + initialization
uint16_t sonarConfidence = 0;
double gpsLatitude = 0.0;
double gpsLongitude = 0.0;
std::string gpsDateTime="00:00:00";

boost::mutex sonar_mutex;
boost::mutex gps_mutex;
