#include "main.hpp"

bool keepRunning{true};
boost::asio::io_context* global_ioc = nullptr; 

// Signal handler for Ctrl+C (SIGINT)
void signalHandler(int signum) {
    std::cout << "\nInterrupt signal (" << signum << ") received. Exiting...\n";
    keepRunning = false;

    if (global_ioc) {
        global_ioc->stop();
    }
}

int main() {
    std::signal(SIGINT, signalHandler);
    
    JsonReader reader("./config.json");
    reader.setConfig();

    PingSonarDevice sonar(config.PingDevicePort, config.PingDeviceBaudRate);
    GPSDevice gps(config.GPSDevicePort, config.GPSDeviceBaudRate);

    sonar.Start();
    gps.Start();

    boost::asio::io_context ioc;
    global_ioc = &ioc; 
    //WebSocketServer wsServer(ioc,config.WebSocketPort);
    //wsServer.start();

    HttpServer httpServer(ioc, config.HttpServerPort);
    httpServer.start();


    std::thread ioc_thread([&ioc]() {
        ioc.run();
    });

    // Main thread: periodically print device status until interrupted
    while(keepRunning) 
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    // Clean shutdown
    sonar.Stop();
    gps.Stop();
    //wsServer.stop();
    httpServer.stop();

    if (ioc_thread.joinable()) {
        ioc_thread.join();
    }

    //std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout << "Program exited cleanly.\n";
    return 0;
}
