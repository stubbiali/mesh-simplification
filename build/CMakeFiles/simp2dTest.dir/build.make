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
CMAKE_BINARY_DIR = /home/stefano/Desktop/mesh-simplification/build

# Include any dependencies generated for this target.
include CMakeFiles/simp2dTest.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/simp2dTest.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/simp2dTest.dir/flags.make

CMakeFiles/simp2dTest.dir/unitTests/simp2dTest.cpp.o: CMakeFiles/simp2dTest.dir/flags.make
CMakeFiles/simp2dTest.dir/unitTests/simp2dTest.cpp.o: ../unitTests/simp2dTest.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/stefano/Desktop/mesh-simplification/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/simp2dTest.dir/unitTests/simp2dTest.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/simp2dTest.dir/unitTests/simp2dTest.cpp.o -c /home/stefano/Desktop/mesh-simplification/unitTests/simp2dTest.cpp

CMakeFiles/simp2dTest.dir/unitTests/simp2dTest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/simp2dTest.dir/unitTests/simp2dTest.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/stefano/Desktop/mesh-simplification/unitTests/simp2dTest.cpp > CMakeFiles/simp2dTest.dir/unitTests/simp2dTest.cpp.i

CMakeFiles/simp2dTest.dir/unitTests/simp2dTest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/simp2dTest.dir/unitTests/simp2dTest.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/stefano/Desktop/mesh-simplification/unitTests/simp2dTest.cpp -o CMakeFiles/simp2dTest.dir/unitTests/simp2dTest.cpp.s

CMakeFiles/simp2dTest.dir/unitTests/simp2dTest.cpp.o.requires:

.PHONY : CMakeFiles/simp2dTest.dir/unitTests/simp2dTest.cpp.o.requires

CMakeFiles/simp2dTest.dir/unitTests/simp2dTest.cpp.o.provides: CMakeFiles/simp2dTest.dir/unitTests/simp2dTest.cpp.o.requires
	$(MAKE) -f CMakeFiles/simp2dTest.dir/build.make CMakeFiles/simp2dTest.dir/unitTests/simp2dTest.cpp.o.provides.build
.PHONY : CMakeFiles/simp2dTest.dir/unitTests/simp2dTest.cpp.o.provides

CMakeFiles/simp2dTest.dir/unitTests/simp2dTest.cpp.o.provides.build: CMakeFiles/simp2dTest.dir/unitTests/simp2dTest.cpp.o


# Object files for target simp2dTest
simp2dTest_OBJECTS = \
"CMakeFiles/simp2dTest.dir/unitTests/simp2dTest.cpp.o"

# External object files for target simp2dTest
simp2dTest_EXTERNAL_OBJECTS =

simp2dTest: CMakeFiles/simp2dTest.dir/unitTests/simp2dTest.cpp.o
simp2dTest: CMakeFiles/simp2dTest.dir/build.make
simp2dTest: src/libmeshSimplification.a
simp2dTest: RobustPredicates/libpredicates.a
simp2dTest: CMakeFiles/simp2dTest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/stefano/Desktop/mesh-simplification/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable simp2dTest"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/simp2dTest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/simp2dTest.dir/build: simp2dTest

.PHONY : CMakeFiles/simp2dTest.dir/build

CMakeFiles/simp2dTest.dir/requires: CMakeFiles/simp2dTest.dir/unitTests/simp2dTest.cpp.o.requires

.PHONY : CMakeFiles/simp2dTest.dir/requires

CMakeFiles/simp2dTest.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/simp2dTest.dir/cmake_clean.cmake
.PHONY : CMakeFiles/simp2dTest.dir/clean

CMakeFiles/simp2dTest.dir/depend:
	cd /home/stefano/Desktop/mesh-simplification/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/stefano/Desktop/mesh-simplification /home/stefano/Desktop/mesh-simplification /home/stefano/Desktop/mesh-simplification/build /home/stefano/Desktop/mesh-simplification/build /home/stefano/Desktop/mesh-simplification/build/CMakeFiles/simp2dTest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/simp2dTest.dir/depend

