# This is a simple Makefile to test the compilation (in debug mode) 
# for the new code.

SRC_DIR=../src
INC_DIR=../include
SRC=$(SRC_DIR)/shapes.cpp $(SRC_DIR)/point.cpp $(SRC_DIR)/searchPoint.cpp $(SRC_DIR)/dataPoint.cpp $(SRC_DIR)/graphItem.cpp 
OBJ=$(patsubst %.cpp, %.o, $(SRC)) 

CXX=g++
CXXFLAGS=-std=c++11 -g -I$(INC_DIR)

.PHONY: all move clean

all: $(OBJ) move

move:
	mv $(SRC_DIR)/*.o .

clean:
	$(RM) *.o
