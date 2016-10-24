cmake_minimum_required (VERSION 2.6)

project (MESHDOCTORSIMP)

# compilers 
enable_language(CXX)

# The version number.
set (MESHDOCTORSIMP_VERSION_MAJOR 1)
set (MESHDOCTORSIMP_VERSION_MINOR 0)
set (MESHDOCTORSIMP_PATCH_VERSION 0)

find_package(BLAS REQUIRED)
find_package(LAPACK REQUIRED)

# include the header
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/RobustPredicates")
# add the subdirectory 
add_subdirectory(RobustPredicates)

# all the headers 
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src")
# add the subdirectory 
add_subdirectory(src)

# add the flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall ")

# 
# Method to define an executable
# 
function(add_meshSimplification_executable arg1 arg2)
# add executable 
add_executable(${arg1} ${arg2})
target_link_libraries(${arg1} meshSimplification)
target_link_libraries(${arg1} predicates)
target_link_libraries(${arg1} blas)
target_link_libraries(${arg1} lapack)
# add the link
endfunction(add_meshSimplification_executable)

#
# other mains for tests
# 
enable_testing()
file(GLOB APP_SOURCES unitTests/*.cpp)
foreach(testsourcefile ${APP_SOURCES})
    # I used a simple string replace, to cut off .cpp.
    string(REPLACE ".cpp" "" testnameNoExtension ${testsourcefile})
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/unitTests/" "" perfectTestName ${testnameNoExtension})
    # create the executable
    add_meshSimplification_executable(${perfectTestName} ${testsourcefile})
    # create the test 
    add_test("${perfectTestName}" ${perfectTestName})
endforeach(testsourcefile ${APP_SOURCES})

file(GLOB BIN_SOURCES bin/*.cpp)
foreach(testsourcefile ${BIN_SOURCES})
    # I used a simple string replace, to cut off .cpp.
    string(REPLACE ".cpp" "" testnameNoExtension ${testsourcefile})
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/bin/" "" perfectTestName ${testnameNoExtension})
    # create the executable
    add_meshSimplification_executable(${perfectTestName} ${testsourcefile})
    # create the test 
endforeach(testsourcefile ${BIN_SOURCES})






