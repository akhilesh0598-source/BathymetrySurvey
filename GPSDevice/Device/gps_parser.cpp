#include<iostream>
#include<string>
#include<sstream>
#include<vector>

std::vector<std::string> splitLine(std::string &line, char delimeter=',')
{
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string token;
    while (std::getline(ss,token,delimeter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

double dm_to_dd_latitude(std::string latitude) // DD means decimal degrees
{
    std::string first_part=latitude.substr(0,2);
    std::string second_part=latitude.substr(2,8);
    double latitude_in_dd=std::stod(first_part)+(std::stod(second_part))/60;
    return latitude_in_dd;
}

double dm_to_dd_longitude(std::string longitude)
{
    std::string first_part=longitude.substr(0,3);
    std::string second_part=longitude.substr(3,8);
    double longitude_in_dd=std::stod(first_part)+(std::stod(second_part))/60;
    return longitude_in_dd;
}

//GPS one line forat :- $GPGGA,HHMMSS.SS,DDMM.MMMMM,K,DDDMM.MMMMM,L,N,QQ,PP.P,AAAA.AA,M,±XX.XX,M, SSS,RRRR*CC<CR><LF>
void parseGPSLine(std::string &line)
{
    std::vector<std::string> tokens=splitLine(line);
    std::string identifier="$GPGGA";
    if(tokens.empty()||tokens[0]!=identifier)
    {
        std::cerr<<"Error: Identider mismatch is occurred!\n";
        return;
    }
        
    if(tokens[1].length()<6)
    {
        std::cerr<<"Error: UTC time is a bad!\n";
        return;
    }
    std::string utc_time=tokens[1].substr(0,2)+":"+tokens[1].substr(2,2)+":"+tokens[1].substr(4,2);
        
    if(tokens[2].length()!=10)
    {
        std::cerr<<"Error: Latitude value is wrong!\n";
        return;
    }
    double latitude=dm_to_dd_latitude(tokens[2]);
    latitude=tokens[3]=="S"?latitude*-1:latitude;   

    if(tokens[4].length()!=11)
    {
        std::cerr<<"Error: Longitude value is wrong!\n";
        return;
    }
    double longitude=dm_to_dd_longitude(tokens[4]);
    longitude=tokens[5]=="W"?longitude*-1:longitude;

    std::cout<<"time: "<<utc_time<<"\n";
    std::cout<<"latitude: "<<latitude<<"\n";
    std::cout<<"longitude: "<<longitude<<"\n";
}


