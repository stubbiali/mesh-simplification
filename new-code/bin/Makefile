# This is a simple Makefile to test the compilation (in debug mode) of the new code.

INC_DIR=../include
SRC_DIR=../src
TEST_DIR=../test
SRC=$(SRC_DIR)/shapes.cpp $(SRC_DIR)/point.cpp $(SRC_DIR)/searchPoint.cpp $(SRC_DIR)/dataPoint.cpp $(SRC_DIR)/graphItem.cpp $(TEST_DIR)/instantiation.cpp
OBJ=$(patsubst %.cpp, %.o, $(SRC)) 

CXX=g++
CXXFLAGS=-std=c++11 -g -I$(INC_DIR)

.PHONY: all move clean

all: $(OBJ) move

move:
	mv $(SRC_DIR)/*.o . 
	mv $(TEST_DIR)/*.o . 

clean:
	$(RM) *.o
