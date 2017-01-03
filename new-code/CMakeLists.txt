cmake_minimum_required (VERSION 2.6)

# Name of the project
project (MESHSIMPLIFICATION)

# Compilers 
enable_language (CXX)

# Version number
set (MESHSIMPLIFICATION_VERSION_MAJOR 1)
set (MESHSIMPLIFICATION_VERSION_MINOR 0)

# Add compiler flag for warnings
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall ")

# Specify path to meshsimplification library headers
include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/include")

# Specify path to Eigen library headers
include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/../Eigen")

# Build meshsimplification library
add_subdirectory (src)

#
# Build executables
#

# Extract source files
file (GLOB BIN_SRCS bin/*.cpp)

foreach(sourcefile ${BIN_SRCS})
    # String replace to cut off .cpp.
    string (REPLACE ".cpp" "" sourcefileNoExtension ${sourcefile})
    string (REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/bin/" "" executablefile ${sourcefileNoExtension})
    
    # Create the executable
    add_executable (${executablefile} ${sourcefile})
    target_link_libraries (${executablefile} meshsimplification)
endforeach(sourcefile ${BIN_SOURCES})


