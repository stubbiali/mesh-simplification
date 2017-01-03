cmake_minimum_required (VERSION 2.6)

# Name of the project
project (MESHSIMPLIFICATIONLIB)

# Compilers 
enable_language (CXX)

# Version number
set (MESHSIMPLIFICATIONLIB_VERSION_MAJOR 1)
set (MESHSIMPLIFICATIONLIB_VERSION_MINOR 0)

# Add compiler flag for warnings
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall ")

# Specify path to meshsimplification library headers
include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/../include")

# Specify path to Eigen library headers
include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/../../Eigen")

# List all source files
file (GLOB SRCS *.cpp)

# Build the library
add_library (meshsimplification SHARED ${SRCS})
