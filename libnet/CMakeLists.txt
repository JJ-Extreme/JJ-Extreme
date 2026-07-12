cmake_minimum_required(VERSION 3.15)
project(libnet VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Project structure configuration
set(LIBNET_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(LIBNET_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Include directories
include_directories(${LIBNET_INCLUDE_DIR})

# Collect all source files from src directory
file(GLOB_RECURSE SOURCES "${LIBNET_SRC_DIR}/*.cpp")

# Define the library (Static by default for easier cross-platform distribution)
add_library(libnet STATIC ${SOURCES})

# Target include directories for consumers of this library
target_include_directories(libnet PUBLIC 
    $<BUILD_INTERFACE:${LIBNET_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:include>
)

# Platform specific configurations
if(WIN32)
    target_compile_definitions(libnet PRIVATE PLATFORM_WINDOWS)
    target_link_libraries(libnet PUBLIC ws2_32) # Link Windows Socket API
elseif(APPLE)
    target_compile_definitions(libnet PRIVATE PLATFORM_MACOS)
else()
    target_compile_definitions(libnet PRIVATE PLATFORM_LINUX)
endif()

# --- Examples ---
enable_testing()
add_executable(net_example examples/main.cpp)
target_link_libraries(net_example PRIVATE libnet)