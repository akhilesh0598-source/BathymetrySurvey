#include "Utility.hpp"

std::vector<std::string> splitLine(std::string &line, char delimeter)
{
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string token;
    while (std::getline(ss, token, delimeter))
    {
        tokens.push_back(token);
    }
    return tokens;
}
