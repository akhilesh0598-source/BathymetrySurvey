
cmake_minimum_required(VERSION 3.10)

project(PingProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${CMAKE_SOURCE_DIR})

set(SOURCE_FILES
    Test/main.cpp
    PingMain/serial-device.cpp
    device/ping-device.cpp
    device/ping-device-ping1d.cpp
    message/ping-message-all.h
    message/ping-message-common.h
    message/ping-message-bluebps.h
    PingMain/ping-port.h
    PingMain/ping-time.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

message(STATUS "Build output: ${PROJECT_BINARY_DIR}")
