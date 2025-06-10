cmake_minimum_required(VERSION 3.10)

project(Main LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(external/spdlog)
add_subdirectory(external/json)

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

# Find Boost
find_package(Boost REQUIRED COMPONENTS system)

if(NOT Boost_FOUND)
  message(FATAL_ERROR "Boost not found!")
endif()

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# Link libraries
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
    Boost::system                # Use Boost target
    spdlog::spdlog_header_only  # Header-only target
    nlohmann_json::nlohmann_json 
)

# Include directories
target_include_directories(${PROJECT_NAME} 
    PRIVATE 
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}  # Ensure headers from project subfolders are found
    external/spdlog/include
    external/json/include
)
