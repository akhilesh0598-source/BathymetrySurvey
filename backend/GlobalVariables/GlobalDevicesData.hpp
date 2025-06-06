#pragma once
#include <boost/thread.hpp>

extern uint32_t sonarDistance;     // declaration
extern uint16_t sonarConfidence;
extern double gpsLatitude;
extern double gpsLongitude;
extern std::string gpsDateTime;

extern boost::mutex sonar_mutex;
extern boost::mutex gps_mutex;
