cmake_minimum_required(VERSION 3.10)

project(Main LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES
    Main/main.cpp
    EchoSounderDevice/PingMain/serial-device.cpp
    EchoSounderDevice/PingMain/ping-port.h
    EchoSounderDevice/PingMain/ping-time.cpp
    EchoSounderDevice/device/ping-device.cpp
    EchoSounderDevice/device/ping-device-ping1d.cpp
    EchoSounderDevice/message/ping-message-all.h
    EchoSounderDevice/message/ping-message-common.h
    EchoSounderDevice/message/ping-message-bluebps.h
    WebSocket/WebSocketServer.cpp
    GPSDevice/Device/GPSDevice.cpp
    Utility/Utility.cpp
)

find_package(Boost 1.86 EXACT REQUIRED COMPONENTS system)

if(NOT Boost_FOUND)
  message(FATAL_ERROR "Boost 1.86 not found!")
endif()

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES})

