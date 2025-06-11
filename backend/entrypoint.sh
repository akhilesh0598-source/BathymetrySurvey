#!/bin/bash

# Clean up old PTYs
rm -f /tmp/ttyReadPing /tmp/ttyWritePing /tmp/ttyReadGPS /tmp/ttyWriteGPS

# Start socat virtual serial ports
socat -d -d PTY,raw,echo=0,link=/tmp/ttyReadPing,mode=666 PTY,raw,echo=0,link=/tmp/ttyWritePing,mode=666 &
socat -d -d PTY,raw,echo=0,link=/tmp/ttyReadGPS,mode=666 PTY,raw,echo=0,link=/tmp/ttyWriteGPS,mode=666 &

# Allow PTYs to initialize
sleep 2

# Start Python mock data generators
python3 /app/MockDevice/GPSDeviceMockData.py &
python3 /app/MockDevice/PingDeviceMockData.py &

# Execute main C++ app
exec /app/Main