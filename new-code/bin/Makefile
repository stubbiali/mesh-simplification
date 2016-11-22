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
SRC_TEST=$(TEST_DIR)/main_geoPoint.cpp $(TEST_DIR)/main_point.cpp $(TEST_DIR)/main_bmesh.cpp $(TEST_DIR)/main_mesh.cpp $(TEST_DIR)/main_graphItem.cpp $(TEST_DIR)/main_bconnect.cpp $(TEST_DIR)/main_connect.cpp $(TEST_DIR)/main_meshInfo.cpp $(TEST_DIR)/main_intersect.cpp $(TEST_DIR)/main_boundingBox.cpp
OBJ_TEST=$(patsubst %.cpp, %.o, $(SRC_TEST)) 

# Executables
EXEC=main_geoPoint main_point main_bmesh main_mesh main_graphItem main_bconnect main_connect main_meshInfo main_intersect main_boundingBox

# Compilation flags
CXX=g++
CXXFLAGS=-std=c++11 -g -I$(INC_DIR)
#CXXFLAGS=-std=c++11 -DNDEBUG -O3 -ftree-vectorize -I$(INC_DIR)


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
	$(RM) $(SRC_DIR)/*.o $(TEST_DIR)/*.o $(OBJ_DIR)/*.o $(EXEC)
	
	
