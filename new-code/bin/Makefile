CXX=g++
CXXFLAGS=-std=c++11 -g
SRC_DIRECTORY=../src
SRC=$(SRC_DIRECTORY)/shapes.cpp $(SRC_DIRECTORY)/point.cpp $(SRC_DIRECTORY)/searchPoint.cpp $(SRC_DIRECTORY)/dataPoint.cpp
OBJ=$(patsubst %.cpp, %.o, $(SRC)) 

.PHONY: all move clean

all: $(OBJ) move

move:
	mv $(SRC_DIRECTORY)/*.o .

clean:
	$(RM) *.o
