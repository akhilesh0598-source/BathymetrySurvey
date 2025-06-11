FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

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
    && apt-get clean && rm -rf /var/lib/apt/lists/*

RUN pip3 install pyserial

WORKDIR /app

# Copy and install Boost (cached layer)
COPY tmp/boost_1_83_0/ /app/tmp/boost_1_83_0
WORKDIR /app/tmp/boost_1_83_0
RUN ./bootstrap.sh --with-libraries=system,filesystem,thread,regex && \
    ./b2 install -j$(nproc)

# Copy full project
WORKDIR /app
COPY . .

# Fix permissions and formats
RUN chmod 666 /app/config.json && \
    dos2unix /app/MockDevice/*.py && \
    chmod +x /app/MockDevice/*.py

# Build the C++ project
RUN mkdir build 
WORKDIR /app/build
RUN cmake .. && make -j$(nproc)

WORKDIR /app
# Copy and set up entrypoint
COPY entrypoint.sh /app/entrypoint.sh
RUN chmod +x /app/entrypoint.sh

ENTRYPOINT ["/app/entrypoint.sh"]
