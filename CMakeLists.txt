cmake_minimum_required(VERSION 3.10)
project(AutomaticTrashCan CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable verbose output for debugging
set(CMAKE_VERBOSE_MAKEFILE ON)

# This CMakeLists.txt is primarily for GitHub language detection
# The actual build for ESP32 is done via Arduino IDE or PlatformIO

# Add include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
)

# List of source files
set(SOURCES
    ${CMAKE_SOURCE_DIR}/src/main.cpp
)

# Create executable (for reference only - actual ESP32 build uses Arduino framework)
add_executable(${PROJECT_NAME} ${SOURCES})

# Print configuration info
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Source files: ${SOURCES}")
message(STATUS "")
message(STATUS "Note: This project is built for ESP32 using Arduino framework")
message(STATUS "Use Arduino IDE or PlatformIO for actual firmware compilation")

