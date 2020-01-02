# CMake version cmake@v3.10.x
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

# Project name and a few useful settings. Other commands can pick up the results
project(opencl
    VERSION 0.1.0)

# Must use GNUInstallDirs to install libraries into correct locations on all platforms.
include(GNUInstallDirs)

# Include CMAKE
include(${CMAKE_ROOT}/Modules/FindPkgConfig.cmake)

# Build settings
# set(CMAKE_C_STANDARD 11)
# set(CMAKE_C_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/)

# Add subdirectories - executable
add_subdirectory(./apps/)

# Add subdirectories - libraries
# add_subdirectory(./libs/util/)
