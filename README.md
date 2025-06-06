# Bathymetry Survey

## 1. Project Overview

This repository hosts the full-stack **Bathymetry Survey** system, consisting of two main components:

* **Backend:**
  A C++17 application for real-time sensor data acquisition, WebSocket and HTTP communication, CSV logging, and mock device simulation.

* **Frontend:**
  A React.js application for visualizing and interacting with live sensor data.

---

## 2. System Setup

### 2.1 Backend

#### 2.1.1 Features

1. Real-time data acquisition from Ping Sonar and GPS devices
2. Asynchronous serial communication with Boost Asio
3. WebSocket server for live data streaming
4. HTTP server for control commands (e.g., start/stop logging)
5. Background CSV data logging with timestamps
6. Graceful termination using OS signal handling
7. Support for mock devices using virtual serial ports
8. Clean, modular, and scalable C++ codebase

#### 2.1.2 Prerequisites

Install required dependencies:

```bash
sudo apt update
sudo apt install -y cmake g++ python3 socat libboost-all-dev
```

Clone `spdlog` from GitHub:

```bash
cd BathymetrySurvey/backend/external
git clone https://github.com/gabime/spdlog.git
```

#### 2.1.3 Build & Run Backend

Clone and compile the backend:

```bash
git clone https://github.com/akhilesh0598-source/BathymetrySurvey.git
cd BathymetrySurvey
git submodule update --init --recursive
cd backend
mkdir build && cd build
cmake ..
make -j4
```

#### 2.1.4 Setup Mock Devices

Create virtual serial ports:

**Terminal 1:**

```bash
cd ../MockDevice/Command
sh command.sh
```

Run mock data scripts:

**Terminal 2:**

```bash
cd ../GenerateMockData
python3 GPSDeviceMockData.py
```

**Terminal 3:**

```bash
cd ../GenerateMockData
python3 PingDeviceMockData.py
```

#### 2.1.5 Launch Backend

**Terminal 4:**

```bash
cd ../../build
./Main
```

---

### 2.2 Frontend

#### 2.2.1 Features

1. Real-time visualization of sonar and GPS data
2. WebSocket-based live updates
3. Responsive and intuitive UI

#### 2.2.2 Setup Frontend

Navigate to the frontend folder and install dependencies:

**Terminal 5:**

```bash
cd ../../frontend
npm install
```

Start the development server:

```bash
npm start
```

---

## 3. ⚡ Coming Soon

* REST APIs for historical data access
* Web dashboard for advanced plotting and controls
* Unit testing via Catch2 framework
* Dockerized deployment setup

---

## 4. Author

**Akhilesh Chaurasiya**

Feel free to open issues or contribute to this project on GitHub.
