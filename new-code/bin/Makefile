# This is a simple Makefile to test the compilation (in debug mode) of the new code.

#
# Variables
#

# Directories
INC_DIR=../include
SRC_DIR=../src
TEST_DIR=../test
OBJ_DIR=obj

# Source and object files
SRC=$(wildcard $(SRC_DIR)/*.cpp)
OBJ=$(patsubst %.cpp, %.o, $(SRC)) 

# Source and object files to explicitly instantiate template classes
SRC_TMP=$(TEST_DIR)/instantiation.cpp
OBJ_TMP=$(patsubst %.cpp, %.o, $(SRC_TMP)) 

# Source and object files for test
SRC_TEST=$(TEST_DIR)/main_point.cpp $(TEST_DIR)/main_bmesh.cpp
OBJ_TEST=$(patsubst %.cpp, %.o, $(SRC_TEST)) 

# Executables
EXEC=main_point main_bmesh

# Compilation flags
CXX=g++
CXXFLAGS=-std=c++11 -g -I$(INC_DIR)


#
# Targets
#

all: $(EXEC) $(OBJ_TMP) move 

$(EXEC): $(OBJ) $(OBJ_TEST)
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_DIR)/$@.o $(OBJ)


# Phony targets
.PHONY: move clean
	
move:
	mkdir -p $(OBJ_DIR)
	mv $(SRC_DIR)/*.o $(OBJ_DIR)
	mv $(TEST_DIR)/*.o $(OBJ_DIR)

clean:
	$(RM) $(OBJ_DIR)/*.o $(EXEC)
	
	
