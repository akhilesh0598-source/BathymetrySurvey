#include<iostream>
#include<fstream>
#include<string>
#include<thread>

#include "gps_parser.cpp"


void read_gps(std::string filePath)
{
    std::ifstream gps_file;
    gps_file.open(filePath);
    if(!gps_file.is_open())
    {
        std::cerr<<"Error: file is not openable!!"<<std::endl;
        return;
    }

    while(gps_file.is_open())
    {
        std::string line;
        while (std::getline(gps_file,line))
        {
            parseGPSLine(line);
        } 
        gps_file.clear();
        gps_file.seekg(0, std::ios::beg);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return;
    
}