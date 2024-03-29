# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/stefano/Desktop/mesh-simplification

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/stefano/Desktop/mesh-simplification/buildRelease

# Include any dependencies generated for this target.
include CMakeFiles/taubinSmoothingRoutine.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/taubinSmoothingRoutine.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/taubinSmoothingRoutine.dir/flags.make

CMakeFiles/taubinSmoothingRoutine.dir/bin/taubinSmoothingRoutine.cpp.o: CMakeFiles/taubinSmoothingRoutine.dir/flags.make
CMakeFiles/taubinSmoothingRoutine.dir/bin/taubinSmoothingRoutine.cpp.o: ../bin/taubinSmoothingRoutine.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/stefano/Desktop/mesh-simplification/buildRelease/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/taubinSmoothingRoutine.dir/bin/taubinSmoothingRoutine.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/taubinSmoothingRoutine.dir/bin/taubinSmoothingRoutine.cpp.o -c /home/stefano/Desktop/mesh-simplification/bin/taubinSmoothingRoutine.cpp

CMakeFiles/taubinSmoothingRoutine.dir/bin/taubinSmoothingRoutine.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/taubinSmoothingRoutine.dir/bin/taubinSmoothingRoutine.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/stefano/Desktop/mesh-simplification/bin/taubinSmoothingRoutine.cpp > CMakeFiles/taubinSmoothingRoutine.dir/bin/taubinSmoothingRoutine.cpp.i

CMakeFiles/taubinSmoothingRoutine.dir/bin/taubinSmoothingRoutine.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/taubinSmoothingRoutine.dir/bin/taubinSmoothingRoutine.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/stefano/Desktop/mesh-simplification/bin/taubinSmoothingRoutine.cpp -o CMakeFiles/taubinSmoothingRoutine.dir/bin/taubinSmoothingRoutine.cpp.s

CMakeFiles/taubinSmoothingRoutine.dir/bin/taubinSmoothingRoutine.cpp.o.requires:

.PHONY : CMakeFiles/taubinSmoothingRoutine.dir/bin/taubinSmoothingRoutine.cpp.o.requires

CMakeFiles/taubinSmoothingRoutine.dir/bin/taubinSmoothingRoutine.cpp.o.provides: CMakeFiles/taubinSmoothingRoutine.dir/bin/taubinSmoothingRoutine.cpp.o.requires
	$(MAKE) -f CMakeFiles/taubinSmoothingRoutine.dir/build.make CMakeFiles/taubinSmoothingRoutine.dir/bin/taubinSmoothingRoutine.cpp.o.provides.build
.PHONY : CMakeFiles/taubinSmoothingRoutine.dir/bin/taubinSmoothingRoutine.cpp.o.provides

CMakeFiles/taubinSmoothingRoutine.dir/bin/taubinSmoothingRoutine.cpp.o.provides.build: CMakeFiles/taubinSmoothingRoutine.dir/bin/taubinSmoothingRoutine.cpp.o


# Object files for target taubinSmoothingRoutine
taubinSmoothingRoutine_OBJECTS = \
"CMakeFiles/taubinSmoothingRoutine.dir/bin/taubinSmoothingRoutine.cpp.o"

# External object files for target taubinSmoothingRoutine
taubinSmoothingRoutine_EXTERNAL_OBJECTS =

taubinSmoothingRoutine: CMakeFiles/taubinSmoothingRoutine.dir/bin/taubinSmoothingRoutine.cpp.o
taubinSmoothingRoutine: CMakeFiles/taubinSmoothingRoutine.dir/build.make
taubinSmoothingRoutine: src/libmeshSimplification.a
taubinSmoothingRoutine: RobustPredicates/libpredicates.a
taubinSmoothingRoutine: /usr/lib/libblas/libblas.so.3.6.0
taubinSmoothingRoutine: /usr/lib/lapack/liblapack.so.3.6.0
taubinSmoothingRoutine: CMakeFiles/taubinSmoothingRoutine.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/stefano/Desktop/mesh-simplification/buildRelease/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable taubinSmoothingRoutine"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/taubinSmoothingRoutine.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/taubinSmoothingRoutine.dir/build: taubinSmoothingRoutine

.PHONY : CMakeFiles/taubinSmoothingRoutine.dir/build

CMakeFiles/taubinSmoothingRoutine.dir/requires: CMakeFiles/taubinSmoothingRoutine.dir/bin/taubinSmoothingRoutine.cpp.o.requires

.PHONY : CMakeFiles/taubinSmoothingRoutine.dir/requires

CMakeFiles/taubinSmoothingRoutine.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/taubinSmoothingRoutine.dir/cmake_clean.cmake
.PHONY : CMakeFiles/taubinSmoothingRoutine.dir/clean

CMakeFiles/taubinSmoothingRoutine.dir/depend:
	cd /home/stefano/Desktop/mesh-simplification/buildRelease && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/stefano/Desktop/mesh-simplification /home/stefano/Desktop/mesh-simplification /home/stefano/Desktop/mesh-simplification/buildRelease /home/stefano/Desktop/mesh-simplification/buildRelease /home/stefano/Desktop/mesh-simplification/buildRelease/CMakeFiles/taubinSmoothingRoutine.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/taubinSmoothingRoutine.dir/depend

