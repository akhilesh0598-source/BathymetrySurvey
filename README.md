
# Bathymetric-Core

## 1. Features

### 1.1 Real-time Sensor Integration

- Supports Ping Sonar and GPS devices for continuous data acquisition.

### 1.2 Asynchronous Data Processing

- Non-blocking serial communication using Boost Asio for smooth, real-time reads.

### 1.3 WebSocket Server

- Streams live sensor data to connected clients for real-time monitoring.

### 1.4 HTTP Server

- Provides endpoints for remote commands (e.g., start/stop logging).

### 1.5 Data Logging

- Background CSV logger that records sensor data with timestamps.

### 1.6 Graceful Shutdown

- Handles OS signals (e.g., SIGINT) for clean application termination.

### 1.7 Mock Device Support

- Easily create virtual serial devices for development and testing without hardware.

### 1.8 Structured Project Architecture

- Modular folder structure for scalability and maintainability.

---

## 2. Installation & Setup Guide

### 2.1 Prerequisites

- **Operating System:** Linux (Ubuntu preferred)
- **Build Tools:**
  - C++17 compatible compiler (e.g., `g++`)
  - CMake (version 3.10+)
- **Libraries:**
  - C/C++ Standard Library
  - Boost (system, asio, beast)
  - `spdlog` (included via external directory)
- **Utilities:**
  - `socat` for virtual serial ports
- **Python 3** for running mock device scripts

### 2.2 Clone the Repository

```bash
git clone https://github.com/yourusername/bathymetric-core.git
cd bathymetric-core/backend
```

### 2.3 Build the Project

```bash
mkdir build
cd build
cmake ..
make -j4
```

### 2.4 Setup Virtual Serial Devices

```bash
cd ../MockDevice/Command
./command.sh
```

### 2.5 Run Mock Data Scripts

```bash
cd ../GenerateMockData
python3 GPSDeviceMockData.py
python3 PingDeviceMockData.py
```

### 2.6 Run the Application

```bash
cd ../../build
./Main
```

---

## 3. Folder Structure Overview

```bash
backend/
├── CMakeLists.txt
├── Main/
│   └── main.cpp
├── EchoSounderDevice/
│   └── PingSonarDevice.cpp/hpp
├── GPSDevice/
│   └── GPSDevice.cpp/hpp
├── WebSocket/
│   └── WebSocketServer.cpp/hpp
├── HttpServer/
│   └── HttpServer.cpp/hpp
├── LogDeviceData/
│   └── LogDevicesData.cpp/hpp
├── GlobalVariables/
│   └── GlobalDevicesData.cpp/hpp
├── MockDevice/
│   ├── Command/
│   │   └── command.sh
│   └── GenerateMockData/
│       ├── GPSDeviceMockData.py
│       └── PingDeviceMockData.py
└── external/
    └── spdlog/
```

---

## 4. ⚡ Coming Soon

- REST API endpoints for historical data
- Web dashboard for live plotting
- Unit tests using Catch2
- Dockerized deployment

---

## 5. Author

**Akhilesh Chaurasiya**

Feel free to contribute or raise issues on GitHub!
