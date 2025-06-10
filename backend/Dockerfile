# Base image
FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive
# Install system dependencies
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    python3 \
    python3-pip \
    socat \
    wget \
    tar \
    dos2unix \
    curl \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy entire project into container
COPY . .

# Install Boost from local extracted folder (assumes you have it under /app/tmp/boost_1_83_0)
WORKDIR /app/tmp/boost_1_83_0
RUN ./bootstrap.sh --with-libraries=system,filesystem,thread,regex && \
    ./b2 install -j$(nproc)

# Go back to main app and build the C++ project
WORKDIR /app
RUN mkdir -p build && 
WORKDIR /app/build
RUN cmake .. && make -j$(nproc)

# Convert shell and Python files to Unix format
RUN dos2unix /app/MockDevice/Command/command.sh && \
    dos2unix /app/MockDevice/GenerateMockData/*.py && \
    chmod +x /app/MockDevice/Command/command.sh && \
    chmod +x /app/MockDevice/GenerateMockData/*.py

# Create the entrypoint script
RUN echo '#!/bin/bash\n\
set -e\n\
echo "Running finite process: command.sh..."\n\
sh /app/MockDevice/Command/command.sh &\n\
echo "Starting infinite process: GPSDeviceMockData.py..."\n\
python3 /app/MockDevice/GenerateMockData/GPSDeviceMockData.py &\n\
echo "Starting infinite process: PingDeviceMockData.py..."\n\
python3 /app/MockDevice/GenerateMockData/PingDeviceMockData.py &\n\
echo "Starting infinite process: Main (C++ backend)..."\n\
/app/Main\n' > /app/entrypoint.sh && chmod +x /app/entrypoint.sh

# Set the default command
ENTRYPOINT ["/app/entrypoint.sh"]
