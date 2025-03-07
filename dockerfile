FROM ubuntu:latest

# Update system
RUN apt-get update && apt-get upgrade -y

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

RUN apt-get update && apt-get install -y libtool autoconf automake

# Clean previous builds in case of version mismatches
RUN make clean || true
RUN rm -rf autom4te.cache aclocal.m4 configure Makefile Makefile.in config.log config.status libtool

# Change directory
WORKDIR /app/deps/quickfix

# Regenerate build scripts
RUN autoreconf --force --install

# Install QuickFIX
RUN ./bootstrap && ./configure --enable-shared --disable-static --with-mysql --with-postgresql --with-odbc \
    && make clean \
    && make \
    && make install

# Set environment variables for linking QuickFIX
ENV LD_LIBRARY_PATH=/usr/local/lib

# Build the server application
WORKDIR /app/build
RUN cmake .. && make

RUN mkdir -p /app/store

# Expose FIX port
EXPOSE 5001

RUN chmod +x /app/build/FIXServer

# Run the server
# Uncomment when you are ready to run deploy the server without edits
# CMD ["./FIXServer", "/app/config/acceptor.cfg"]
CMD ["/bin/bash"]
