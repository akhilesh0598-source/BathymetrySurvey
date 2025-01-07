#include<iostream>
#include "../Device/GPSDevice.cpp"
//#include "../Device/gps_reader.cpp"

int main()
{
    //read_gps("./gps_data.txt");
    //std::string data="$GPGGA,173309.00,5101.04028,N,11402.38289,W,2,07,1.4,1071.0,M,- 17.8,M,6.0, 0122*48";
    //parseGPSLine(data);
    GPSDevice gpsDevice("/dev/ttyUSB0",19200);
    
    int itr=0;
    bool start=false;
    while(true)
    {

        if(!start&&gpsDevice.Start())
        {
            start=true;
        }
        itr++;
        //std::cout<<itr<<std::endl;
    }
    gpsDevice.Stop();
    
    return 0;
}