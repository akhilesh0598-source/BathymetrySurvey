#include <iostream>
#include <thread>
#include <chrono>
#include <csignal>
#include "../GPSDevice/Device/GPSDevice.hpp"
#include "../EchoSounderDevice/PingSonarDevice.hpp"

std::atomic<bool> keepRunning{true};
std::atomic<bool> signalHandled{false};

void signalHandler(int signum) {
    if (!signalHandled.exchange(true)) {  // Only execute once
        std::cout << "\nInterrupt signal (" << signum << ") received. Exiting...\n";
        keepRunning = false;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

int main() {
    std::string gpsPort = "/tmp/ttyReadGPS";
    std::string sonarPort = "/tmp/ttyReadPing";

    // Register signal handler
    std::signal(SIGINT, signalHandler);

    GPSDevice gps(gpsPort, 9600);
    PingSonarDevice sonar(sonarPort, 115200);

    if (!gps.Start()) {
        std::cerr << "Failed to start GPS device.\n";
        return 1;
    }

    if (!sonar.Start()) {
        std::cerr << "Failed to start Sonar device.\n";
        return 1;
    }

    while (keepRunning) {
        {
            boost::mutex::scoped_lock lock(gps_mutex);
            std::cout << "\n[GPS] Time: " << gpsDateTime
                      << ", Latitude: " << gpsLatitude
                      << ", Longitude: " << gpsLongitude << std::endl;
        }

        {
            boost::mutex::scoped_lock lock(sonar_mutex);
            std::cout << "[SONAR] Distance: " << sonarDistance
                      << " mm, Confidence: " << sonarConfidence << "%" << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // Graceful shutdown
    gps.Stop();
    sonar.Stop();

    std::cout << "Devices stopped. Program exited cleanly.\n";
    return 0;
}
