cmake_minimum_required(VERSION 3.10)

project(Main LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

# Add spdlog (header-only)
add_subdirectory(external/spdlog)

# Source files
set(SOURCE_FILES
    Main/main.cpp
    GlobalVariables/GlobalDevicesData.cpp
    EchoSounderDevice/PingSonarDevice.cpp
    WebSocket/WebSocketServer.cpp
    GPSDevice/GPSDevice.cpp
    LogDevicesData/LogDevicesData.cpp
    HttpServer/HttpServer.cpp
)

# Find Boost
find_package(Boost 1.83 EXACT REQUIRED COMPONENTS system)

if(NOT Boost_FOUND)
  message(FATAL_ERROR "Boost 1.83 not found!")
endif()

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Link libraries
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
    Boost::system                # Use Boost target
    spdlog::spdlog_header_only  # Header-only target
)

# Include directories
target_include_directories(${PROJECT_NAME} 
    PRIVATE 
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}  # Ensure headers from project subfolders are found
    external/spdlog/include
)
