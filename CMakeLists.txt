cmake_minimum_required(VERSION 3.10)

project(Main)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${CMAKE_SOURCE_DIR})

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
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

message(STATUS "Build output: ${PROJECT_BINARY_DIR}")
