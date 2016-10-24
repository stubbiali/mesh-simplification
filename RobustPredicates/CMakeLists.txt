cmake_minimum_required (VERSION 2.6)

# name of the project
project (PREDICATES)

# compilers 
enable_language(CXX)

# The version number.
set (PREDICATES_VERSION_MAJOR 1)
set (PREDICATES_VERSION_MINOR 0)
set (PREDICATES_PATCH_VERSION 0)

set (PREDICATES_VERSION "${PREDICATES_MAJOR_VERSION}.${PREDICATES_MINOR_VERSION}")

# add the flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall ")

file(GLOB SRCFILES *.cxx)

add_library(predicates ${SRCFILES})
