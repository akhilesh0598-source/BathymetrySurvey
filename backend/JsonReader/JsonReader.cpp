#include "JsonReader.hpp"
#include <fstream>
#include <iostream>

JsonReader::JsonReader(const std::string& filePath) : filePath(filePath) {}

bool JsonReader::read() {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Cannot open JSON file: " << filePath << std::endl;
        return false;
    }
    try {
        file >> jsonData;
    } catch (const std::exception& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
        return false;
    }
    return true;
}

template<typename T>
T JsonReader::get(const std::string& key) const {
    return jsonData.at(key).get<T>();
}

// Explicit template instantiations
template int JsonReader::get<int>(const std::string& key) const;
template std::string JsonReader::get<std::string>(const std::string& key) const;
template bool JsonReader::get<bool>(const std::string& key) const;
template double JsonReader::get<double>(const std::string& key) const;

void JsonReader::setConfig() {
    if (!read()) {
        throw std::runtime_error("Failed to read config");
    }

    try {
        config.WebSocketPort = get<int>("WebSocketPort");
        config.HttpServerPort = get<int>("HttpServerPort");
        config.PingDevicePort = get<std::string>("PingDevicePort");
        config.PingDeviceBaudRate = get<int>("PingDeviceBaudRate");
        config.GPSDevicePort = get<std::string>("GPSDevicePort");
        config.GPSDeviceBaudRate = get<int>("GPSDeviceBaudRate");
        config.setLogFilePath(get<std::string>("LogFilePath"));
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Error parsing config JSON: ") + e.what());
    }
}

