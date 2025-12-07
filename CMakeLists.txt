cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Image2ObjectsGD VERSION 1.0.0)

# Add Geode CPM
file(
    DOWNLOAD
    https://github.com/geode-sdk/cli/releases/latest/download/GeodeToolchainCPM.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/cmake/GeodeToolchainCPM.cmake
)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/GeodeToolchainCPM.cmake)

# Collect source files
file(GLOB_RECURSE SOURCES
    src/*.cpp
)

# Create the mod
add_library(${PROJECT_NAME} SHARED ${SOURCES})

# Link Geode
target_link_libraries(${PROJECT_NAME} geode-sdk)

# Setup the mod
setup_geode_mod(${PROJECT_NAME})
