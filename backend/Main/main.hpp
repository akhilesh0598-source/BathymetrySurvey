#pragma once
#include <iostream>
#include <thread>
#include <chrono>
#include <csignal>
#include <atomic>

#include "../GPSDevice/GPSDevice.hpp"
#include "../EchoSounderDevice/PingSonarDevice.hpp"
#include "../WebSocket/WebSocketServer.hpp"
#include "../GlobalVariables/GlobalDevicesData.hpp"
#include "../LogDevicesData/LogDevicesData.hpp"
#include "../HttpServer/HttpServer.hpp" 


// Global control flags
extern std::atomic<bool> keepRunning;
