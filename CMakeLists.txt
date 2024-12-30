cmake_minimum_required(VERSION 3.10)

project(Main LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --[ 1. Source files ]--
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

# --[ 2. Find Boost 1.86 Exactly ]--
# Make sure you list all COMPONENTS you need (system, thread, filesystem, etc.)
find_package(Boost 1.86 EXACT REQUIRED COMPONENTS system)

# If CMake cannot find Boost 1.86, it will error out here.
if(NOT Boost_FOUND)
  message(FATAL_ERROR "Boost 1.86 not found!")
endif()

# --[ 3. Print debug info to ensure you're getting the correct version ]--
message(STATUS "Detected Boost version: ${Boost_VERSION}")
message(STATUS "Boost include dirs: ${Boost_INCLUDE_DIRS}")
message(STATUS "Boost libraries: ${Boost_LIBRARIES}")

# --[ 4. Create the executable ]--
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# --[ 5. Add include paths and link libraries ]--
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES})

# Optional: If you want to see which headers are really included during compilation
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -E -H")  # for GCC/Clang
