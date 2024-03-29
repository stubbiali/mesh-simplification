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
include CMakeFiles/geometric_simplification.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/geometric_simplification.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/geometric_simplification.dir/flags.make

CMakeFiles/geometric_simplification.dir/bin/geometric_simplification.cpp.o: CMakeFiles/geometric_simplification.dir/flags.make
CMakeFiles/geometric_simplification.dir/bin/geometric_simplification.cpp.o: ../bin/geometric_simplification.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/stefano/Desktop/mesh-simplification/buildRelease/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/geometric_simplification.dir/bin/geometric_simplification.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/geometric_simplification.dir/bin/geometric_simplification.cpp.o -c /home/stefano/Desktop/mesh-simplification/bin/geometric_simplification.cpp

CMakeFiles/geometric_simplification.dir/bin/geometric_simplification.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/geometric_simplification.dir/bin/geometric_simplification.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/stefano/Desktop/mesh-simplification/bin/geometric_simplification.cpp > CMakeFiles/geometric_simplification.dir/bin/geometric_simplification.cpp.i

CMakeFiles/geometric_simplification.dir/bin/geometric_simplification.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/geometric_simplification.dir/bin/geometric_simplification.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/stefano/Desktop/mesh-simplification/bin/geometric_simplification.cpp -o CMakeFiles/geometric_simplification.dir/bin/geometric_simplification.cpp.s

CMakeFiles/geometric_simplification.dir/bin/geometric_simplification.cpp.o.requires:

.PHONY : CMakeFiles/geometric_simplification.dir/bin/geometric_simplification.cpp.o.requires

CMakeFiles/geometric_simplification.dir/bin/geometric_simplification.cpp.o.provides: CMakeFiles/geometric_simplification.dir/bin/geometric_simplification.cpp.o.requires
	$(MAKE) -f CMakeFiles/geometric_simplification.dir/build.make CMakeFiles/geometric_simplification.dir/bin/geometric_simplification.cpp.o.provides.build
.PHONY : CMakeFiles/geometric_simplification.dir/bin/geometric_simplification.cpp.o.provides

CMakeFiles/geometric_simplification.dir/bin/geometric_simplification.cpp.o.provides.build: CMakeFiles/geometric_simplification.dir/bin/geometric_simplification.cpp.o


# Object files for target geometric_simplification
geometric_simplification_OBJECTS = \
"CMakeFiles/geometric_simplification.dir/bin/geometric_simplification.cpp.o"

# External object files for target geometric_simplification
geometric_simplification_EXTERNAL_OBJECTS =

geometric_simplification: CMakeFiles/geometric_simplification.dir/bin/geometric_simplification.cpp.o
geometric_simplification: CMakeFiles/geometric_simplification.dir/build.make
geometric_simplification: src/libmeshSimplification.a
geometric_simplification: RobustPredicates/libpredicates.a
geometric_simplification: /usr/lib/libblas/libblas.so.3.6.0
geometric_simplification: /usr/lib/lapack/liblapack.so.3.6.0
geometric_simplification: CMakeFiles/geometric_simplification.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/stefano/Desktop/mesh-simplification/buildRelease/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable geometric_simplification"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/geometric_simplification.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/geometric_simplification.dir/build: geometric_simplification

.PHONY : CMakeFiles/geometric_simplification.dir/build

CMakeFiles/geometric_simplification.dir/requires: CMakeFiles/geometric_simplification.dir/bin/geometric_simplification.cpp.o.requires

.PHONY : CMakeFiles/geometric_simplification.dir/requires

CMakeFiles/geometric_simplification.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/geometric_simplification.dir/cmake_clean.cmake
.PHONY : CMakeFiles/geometric_simplification.dir/clean

CMakeFiles/geometric_simplification.dir/depend:
	cd /home/stefano/Desktop/mesh-simplification/buildRelease && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/stefano/Desktop/mesh-simplification /home/stefano/Desktop/mesh-simplification /home/stefano/Desktop/mesh-simplification/buildRelease /home/stefano/Desktop/mesh-simplification/buildRelease /home/stefano/Desktop/mesh-simplification/buildRelease/CMakeFiles/geometric_simplification.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/geometric_simplification.dir/depend

