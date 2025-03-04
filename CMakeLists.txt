cmake_minimum_required(VERSION 3.5.0)
project(FIXServer VERSION 0.1.0 LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the source files
file(GLOB SOURCES "src/*.cpp")

# Create the executable
add_executable(FIXServer ${SOURCES})

# Include directories
target_include_directories(FIXServer PRIVATE "${CMAKE_SOURCE_DIR}/src" "${CMAKE_SOURCE_DIR}/generated/sbe" "${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/deps/quickfix/src/C++")

# Find QuickFIX library
find_library(QUICKFIX_LIBRARY quickfix REQUIRED)

# Link QuickFIX to the server
target_link_libraries(FIXServer PRIVATE ${QUICKFIX_LIBRARY})

# Link QuickFIX to the server
# target_link_libraries(FIXServer PRIVATE "${CMAKE_SOURCE_DIR}/deps/quickfix/libquickfix.so")

# Set output directory for binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

