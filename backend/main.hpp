#pragma once
#include <iostream>
#include <thread>
#include <chrono>
#include <csignal>
#include <atomic>


#include "WebSocket/WebSocketServer.hpp"
#include "GlobalVariables/GlobalDevicesData.hpp"
#include "HttpServer/HttpServer.hpp" 
#include "GlobalVariables/GlobalVariable.hpp"
#include "JsonReader/JsonReader.hpp"
#include "GPSDevice/GPSDevice.hpp"
#include "EchoSounderDevice/PingSonarDevice.hpp"

extern bool keepRunning;

