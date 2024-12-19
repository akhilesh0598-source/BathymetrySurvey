
cmake_minimum_required(VERSION 3.10)

project(Main)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${CMAKE_SOURCE_DIR})

set(SOURCE_FILES
    main.cpp
    ../EchoSounder/ping-cpp/src/Testing/serial-device.cpp
    ../EchoSounder/ping-cpp/src/device/ping-device.cpp
    ../EchoSounder/ping-cpp/src/device/ping-device-ping1d.cpp
    ../EchoSounder/ping-cpp/src/message/ping-message-all.h
    ../EchoSounder/ping-cpp/src/message/ping-message-common.h
    ../EchoSounder/ping-cpp/src/message/ping-message-bluebps.h
    ../EchoSounder/ping-cpp/src/Testing/ping-port.h
    ../EchoSounder/ping-cpp/src/Testing/ping-time.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

message(STATUS "Build output: ${PROJECT_BINARY_DIR}")
