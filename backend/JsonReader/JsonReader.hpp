#pragma once
#include <string>
#include <nlohmann/json.hpp>

#include "../GlobalVariables/GlobalVariable.hpp"

class JsonReader {
public:
    explicit JsonReader(const std::string& filePath);
    bool read();
    
    template<typename T>
    T get(const std::string& key) const;

     void setConfig();

private:
    std::string filePath;
    nlohmann::json jsonData;
};
