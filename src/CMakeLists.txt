cmake_minimum_required (VERSION 2.6)

# name of the project
project (MESHDOCTORLIB)

# compilers 
enable_language(CXX)

# compilers 
enable_language(CXX)

# The version number.
set (MESHDOCTORLIB_VERSION_MAJOR 1)
set (MESHDOCTORLIB_VERSION_MINOR 0)
set (MESHDOCTORLIB_PATCH_VERSION 0)

set (MESHDOCTORLIB_VERSION "${MESHDOCTORLIB_MAJOR_VERSION}.${MESHDOCTORLIB_MINOR_VERSION}")

# add the flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall ")

file(GLOB SRCFILES adding/*.cpp core/*.cpp doctor/*.cpp geometry/*.cpp meshOperation/*.cpp utility/*.cpp file/*.cpp intersect/.cpp)

add_library(meshSimplification ${SRCFILES})
