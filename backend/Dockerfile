FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    g++ \
    gcc \
    python3 \
    python3-pip \
    socat \
    wget \
    tar \
    dos2unix \
    curl \
    git \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

RUN pip3 install pyserial

WORKDIR /app

ENV BOOST_VERSION=1.83.0
ENV BOOST_VERSION_UNDERSCORE=1_83_0

WORKDIR /app/tmp

RUN wget -q -O boost_${BOOST_VERSION_UNDERSCORE}.tar.gz \
    https://sourceforge.net/projects/boost/files/boost/${BOOST_VERSION}/boost_${BOOST_VERSION_UNDERSCORE}.tar.gz/download && \
    tar xzf boost_${BOOST_VERSION_UNDERSCORE}.tar.gz && \
    cd boost_${BOOST_VERSION_UNDERSCORE} && \
    ./bootstrap.sh --with-libraries=system,filesystem,thread,regex && \
    ./b2 install -j$(nproc) && \
    cd .. && rm -rf boost_${BOOST_VERSION_UNDERSCORE} boost_${BOOST_VERSION_UNDERSCORE}.tar.gz


# Copy full project
WORKDIR /app
COPY . .

RUN git submodule update --init --recursive

# Fix permissions and formats
RUN chmod 666 /app/config.json && \
    dos2unix /app/MockDevice/*.py && \
    chmod +x /app/MockDevice/*.py

# Build the C++ project
RUN mkdir build 
WORKDIR /app/build
RUN cmake .. || (cat CMakeFiles/CMakeError.log && false)
RUN make -j$(nproc)

WORKDIR /app
# Copy and set up entrypoint
COPY entrypoint.sh /app/entrypoint.sh
RUN chmod +x /app/entrypoint.sh

EXPOSE 8080 9002

ENTRYPOINT ["/app/entrypoint.sh"]
