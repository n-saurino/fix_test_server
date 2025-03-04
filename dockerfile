FROM ubuntu:latest

# Install dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libssl-dev \
    autoconf \
    automake \
    libtool \
    git \
    && rm -rf /var/lib/apt/lists/*

# Clone and build QuickFIX with SBE support
# WORKDIR /quickfix
# RUN git clone https://github.com/quickfix/quickfix.git . && \
# ./bootstrap && ./configure --enable-sbe && make && make install


# Set environment variables for linking QuickFIX
ENV LD_LIBRARY_PATH=/usr/local/lib

# Create work directory for the server application
WORKDIR /app

# Copy server source code
COPY . .

# Ensure a clean build directory
RUN rm -rf build && mkdir build

# Install QuickFIX
WORKDIR /app/deps/quickfix
RUN ./bootstrap && ./configure --enable-sbe && make && make install

# Set environment variables for linking QuickFIX
ENV LD_LIBRARY_PATH=/usr/local/lib

# Build the server application
WORKDIR /app/build
RUN cmake .. && make

# Expose FIX port
EXPOSE 5001

RUN chmod +x /app/build/FIXServer

# Run the server
CMD ["./FIXServer"]
