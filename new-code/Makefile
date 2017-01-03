# Makefile for compiling the meshsimplification library, the executables and the tests.

#
# Variables
#

# Library directories
LIB_INC_DIR=include
LIB_SRC_DIR=src
TEST_SRC_DIR=test
MAIN_SRC_DIR=main

OBJ_DIR=build
LIB_OBJ_DIR=$(OBJ_DIR)
TEST_OBJ_DIR=$(OBJ_DIR)/test
MAIN_OBJ_DIR=$(OBJ_DIR)/main

LIB_DIR=lib

BIN_DIR=bin
TEST_BIN_DIR=$(BIN_DIR)/test
MAIN_BIN_DIR=$(BIN_DIR)/main

# Path to Eigen directory
EIGEN_DIR=$(LIB_DIR)/Eigen

# Name of the library
LIB=meshsimplification

# Source and object files for meshsimplification library
LIB_SRC=$(wildcard $(LIB_SRC_DIR)/*.cpp)
LIB_OBJ=$(patsubst $(LIB_SRC_DIR)/%.cpp, $(LIB_OBJ_DIR)/%.o, $(LIB_SRC)) 

# Source, object and binary files for tests
TEST_SRC=$(wildcard $(TEST_SRC_DIR)/*.cpp)
TEST_OBJ=$(patsubst $(TEST_SRC_DIR)/%.cpp, $(TEST_OBJ_DIR)/%.o, $(TEST_SRC))
TEST_BIN=$(patsubst $(TEST_SRC_DIR)/%.cpp, $(TEST_BIN_DIR)/%, $(TEST_SRC))

# Source, object and binary files for executables
MAIN_SRC=$(wildcard $(MAIN_SRC_DIR)/*.cpp)
MAIN_OBJ=$(patsubst $(MAIN_SRC_DIR)/%.cpp, $(MAIN_OBJ_DIR)/%.o, $(MAIN_SRC))
MAIN_BIN=$(patsubst $(MAIN_SRC_DIR)/%.cpp, $(MAIN_BIN_DIR)/%, $(MAIN_SRC))

# Source and object files to explicitly instantiate template classes
#SRC_TMP=$(TEST_DIR)/instantiation.cpp
#OBJ_TMP=$(patsubst %.cpp, %.o, $(SRC_TMP)) 

# Compilation mode; default is DEBUG
DEBUG=yes
RELEASE=no

# Allow for mesh self intersections; default is NO
ENABLE_SELF_INTERSECTIONS=no

# Compilation flags
CXX=clang++

ifeq ($(RELEASE),yes)
	CXXFLAGS=-std=c++11 -DNDEBUG -O3 -ftree-vectorize -I $(LIB_INC_DIR) -I $(EIGEN_DIR)
else
	CXXFLAGS=-std=c++11 -g -I $(LIB_INC_DIR) -I $(EIGEN_DIR)
endif
	
ifeq ($(ENABLE_SELF_INTERSECTIONS),yes)
	CXXFLAGS+=-DENABLE_SELF_INTERSECTIONS 
endif
	
LDFLAGS=-L $(LIB_DIR) -l $(LIB)

#
# Targets
#

all: create_folders lib test main 

#
# Build the library
#

$(LIB_OBJ_DIR)/%.o: $(LIB_SRC_DIR)/%.cpp
	@echo -n Compiling $@ ... 
	@$(CXX) $(CXXFLAGS)	-c -o	$@	$<	
	@echo " done"

lib: create_folders $(LIB_OBJ)
	@echo -n Creating libmeshsimplification.a ...
	@ar -r -s	$(LIB_DIR)/lib$(LIB).a	$(LIB_OBJ) > /dev/null 2>&1
	@echo " done"
	@echo "\033[92m\nmeshsimplification library successfully built\n\033[0m"

#
# Build the tests
#

$(TEST_OBJ_DIR)/%.o: $(TEST_SRC_DIR)/%.cpp
	@echo -n Compiling $@ ... 
	@$(CXX) $(CXXFLAGS)	-c -o	$@	$<	
	@echo " done"
	
$(TEST_BIN_DIR)/%: $(TEST_OBJ_DIR)/%.o lib
	@echo -n Linking $@ ... 
	@$(CXX) $(CXXFLAGS)	-o	$@	$<	$(LDFLAGS)
	@echo " done"
	
test: create_folders $(TEST_BIN)
	@echo "\033[92m\nTests successfully compiled and linked\n\033[0m"

#
# Build the executables
#

$(MAIN_OBJ_DIR)/%.o: $(MAIN_SRC_DIR)/%.cpp
	@echo -n Compiling $@ ... 
	@$(CXX) $(CXXFLAGS)	-c -o	$@	$<	
	@echo " done"
	
$(MAIN_BIN_DIR)/%: $(MAIN_OBJ_DIR)/%.o lib
	@echo -n Linking $@ ... 
	@$(CXX) $(CXXFLAGS)	-o	$@	$<	$(LDFLAGS)
	@echo " done"
	
main: create_folders $(MAIN_BIN)
	@echo "\033[92m\nExecutables successfully compiled and linked\n\033[0m"

#
# Phony targets
#

.PHONY: create_folders clean
	
create_folders:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(LIB_OBJ_DIR)
	@mkdir -p $(TEST_OBJ_DIR)
	@mkdir -p $(MAIN_OBJ_DIR)
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(MAIN_BIN_DIR)
	@mkdir -p $(TEST_BIN_DIR)

clean:
	@$(RM) -r $(OBJ_DIR) $(BIN_DIR) $(LIB_DIR)/lib$(LIB).a

#
# Prevent object files to be deleted
#

.SECONDARY: $(TEST_OBJ) $(MAIN_OBJ)
	
	
