#include "main.hpp"
  // Include your HTTP server header

std::atomic<bool> keepRunning{true};

// Signal handler for Ctrl+C (SIGINT)
void signalHandler(int signum) {
    std::cout << "\nInterrupt signal (" << signum << ") received. Exiting...\n";
    keepRunning = false;
}

int main() {
    std::signal(SIGINT, signalHandler);

    boost::asio::io_context ioc;

    // Start your devices
    PingSonarDevice sonar("/tmp/ttyReadPing", 115200);
    GPSDevice gps("/tmp/ttyReadGPS", 9600);
    sonar.Start();
    gps.Start();

    // Start CSV logger
    LogData logger("LogDataResult/device_log.csv");
    logger.start();

    // Start your existing WebSocket server
    WebSocketServer wsServer(ioc, 9002);
    wsServer.start();

    // Start your new HTTP server on port 8080
    HttpServer httpServer(ioc, 8080);
    httpServer.start();

    // Run the io_context in a separate thread so both WebSocket and HTTP run asynchronously
    std::thread ioc_thread([&ioc]() {
        ioc.run();
    });

    // Main thread: periodically print device status until interrupted
    while (keepRunning) {
        {
            boost::mutex::scoped_lock lock1(sonar_mutex);
            boost::mutex::scoped_lock lock2(gps_mutex);
            std::cout << "Sonar distance: " << sonarDistance
                      << " Confidence: " << sonarConfidence
                      << " | GPS lat: " << gpsLatitude
                      << " lon: " << gpsLongitude
                      << " time: " << gpsDateTime << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // Clean shutdown
    sonar.Stop();
    gps.Stop();
    logger.stop();

    // Stop boost asio context, which will stop WebSocket and HTTP servers
    ioc.stop();

    if (ioc_thread.joinable()) {
        ioc_thread.join();
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout << "Program exited cleanly.\n";
    return 0;
}
