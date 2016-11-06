# This is a simple Makefile to test the compilation (in debug mode) 
# for the new code.

CXX=g++
CXXFLAGS=-std=c++11 -g
SRC_DIRECTORY=../src
SRC=$(SRC_DIRECTORY)/shapes.cpp $(SRC_DIRECTORY)/point.cpp $(SRC_DIRECTORY)/searchPoint.cpp $(SRC_DIRECTORY)/dataPoint.cpp $(SRC_DIRECTORY)/graphItem.cpp
OBJ=$(patsubst %.cpp, %.o, $(SRC)) 

.PHONY: all move clean

all: $(OBJ) move

move:
	mv $(SRC_DIRECTORY)/*.o .

clean:
	$(RM) *.o
