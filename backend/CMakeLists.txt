cmake_minimum_required(VERSION 3.14)  # Ensure compatibility with FetchContent_MakeAvailable

project(Main LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_BUILD_TYPE Debug)


# Set default build type to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type (default: Release)" FORCE)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

include(FetchContent)

# Fetch spdlog
FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.13.0
)

# Fetch nlohmann/json
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.2
)

FetchContent_MakeAvailable(spdlog json)

# Find Boost
find_package(Boost REQUIRED COMPONENTS system)

if(NOT Boost_FOUND)
  message(FATAL_ERROR "Boost not found!")
endif()

# Source files
set(SOURCE_FILES
    main.cpp
    Config/Config.cpp
    EchoSounderDevice/PingSonarDevice.cpp
    GlobalVariables/GlobalDevicesData.cpp
    GlobalVariables/GlobalVariable.cpp
    GPSDevice/GPSDevice.cpp
    HttpServer/HttpServer.cpp
    JsonReader/JsonReader.cpp
    LogDevicesData/LogDevicesData.cpp
    WebSocket/WebSocketServer.cpp
)

# Executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Output binary to root of project
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# Link libraries
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
    Boost::system
    spdlog::spdlog_header_only
    nlohmann_json::nlohmann_json
)

# Include directories
target_include_directories(${PROJECT_NAME} 
    PRIVATE 
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
)
