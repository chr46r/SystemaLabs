FROM ubuntu:20.04

# Non-interactive config
ENV DEBIAN_FRONTEND=noninteractive

# Install dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    git \
    cmake \
    gnucobol \
    net-tools \
    iputils-ping

WORKDIR /app

# Copy source
COPY cobol-ledger/ /app/cobol-ledger/
COPY cpp-consensus/ /app/cpp-consensus/

# Build COBOL ledger
WORKDIR /app/cobol-ledger
RUN chmod +x build.sh && ./build.sh

# Build C++ consensus
WORKDIR /app/cpp-consensus
RUN mkdir build && cd build && cmake .. && make

# Final container
WORKDIR /app
CMD ["/bin/bash"]
