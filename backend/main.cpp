#include "main.hpp"

bool keepRunning{true};

// Signal handler for Ctrl+C (SIGINT)
void signalHandler(int signum) {
    std::cout << "\nInterrupt signal (" << signum << ") received. Exiting...\n";
    keepRunning = false;
}

int main() {
    std::signal(SIGINT, signalHandler);
    
    JsonReader reader("config.json");
    reader.setConfig();

    boost::asio::io_context ioc;

    PingSonarDevice sonar(config.PingDevicePort, config.PingDeviceBaudRate);
    GPSDevice gps(config.GPSDevicePort, config.GPSDeviceBaudRate);

    sonar.Start();
    gps.Start();

    WebSocketServer wsServer(ioc,config.WebSocketPort);
    wsServer.start();

    HttpServer httpServer(ioc, config.HttpServerPort);
    httpServer.start();


    std::thread ioc_thread([&ioc]() {
        ioc.run();
    });

    // Main thread: periodically print device status until interrupted
    for (; keepRunning;) {
    {
        boost::mutex::scoped_lock lock1(sonar_mutex);
        boost::mutex::scoped_lock lock2(gps_mutex);
        std::cout << "Sonar distance: " << sonarDistance
                  << " Confidence: " << sonarConfidence
                  << " | GPS lat: " << gpsLatitude
                  << " lon: " << gpsLongitude
                  << " time: " << gpsDateTime << std::endl;
    }

    for (int i = 0; i < 10 && keepRunning; ++i)
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

    // Clean shutdown
    sonar.Stop();
    gps.Stop();

    // Stop boost asio context, which will stop WebSocket and HTTP servers
    ioc.stop();

    if (ioc_thread.joinable()) {
        ioc_thread.join();
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout << "Program exited cleanly.\n";
    return 0;
}
