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
S_LIB_OBJ_DIR=$(OBJ_DIR)/static
D_LIB_OBJ_DIR=$(OBJ_DIR)/dynamic
TEST_OBJ_DIR=$(OBJ_DIR)/test
MAIN_OBJ_DIR=$(OBJ_DIR)/main

LIB_DIR=lib

BIN_DIR=bin
TEST_BIN_DIR=$(BIN_DIR)/test
MAIN_BIN_DIR=$(BIN_DIR)/main

DOC_DIR=doc

# Path to Eigen directory
EIGEN_DIR=$(LIB_DIR)/Eigen

# Name of the library
LIB=meshsimplification

# Include, source and object files for meshsimplification library
LIB_INC=$(wildcard $(LIB_INC_DIR)/*.hpp $(LIB_INC_DIR)/implementation/*.hpp $(LIB_INC_DIR)/inline/*.hpp)
#LIB_IMP=$(wildcard $(LIB_INC_DIR)/implementation/*.hpp)
#LIB_INL=$(wildcard $(LIB_INC_DIR)/inline/*.hpp)
LIB_SRC=$(wildcard $(LIB_SRC_DIR)/*.cpp)
S_LIB_OBJ=$(patsubst $(LIB_SRC_DIR)/%.cpp, $(S_LIB_OBJ_DIR)/%.o, $(LIB_SRC))
D_LIB_OBJ=$(patsubst $(LIB_SRC_DIR)/%.cpp, $(D_LIB_OBJ_DIR)/%.o, $(LIB_SRC)) 

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
	
LDFLAGS=-L $(LIB_DIR) -l $(LIB) -Wl,-rpath=$(LIB_DIR)

#
# Targets
#

all: create_folders dynamic main 

#
# Build static library
#

$(S_LIB_OBJ_DIR)/%.o: $(LIB_SRC_DIR)/%.cpp $(LIB_INC_DIR)/%.hpp $(LIB_INC)
	@echo "Compiling $@" 
	@$(CXX) $(CXXFLAGS)	-c -o	$@	$<	
	@echo "Compiling $@ -- done"

static: create_folders $(S_LIB_OBJ)
	@echo "Creating lib$(LIB).a"
	@ar -r -s	$(LIB_DIR)/lib$(LIB).a	$(S_LIB_OBJ) > /dev/null 2>&1
	@echo "Creating lib$(LIB).a -- done"
	@echo "\033[92mStatic version of meshsimplification library successfully built\n\033[0m"
	
#
# Build dynamic library
#

$(D_LIB_OBJ_DIR)/%.o: $(LIB_SRC_DIR)/%.cpp $(LIB_INC_DIR)/%.hpp $(LIB_INC)
	@echo "Compiling $@" 
	@$(CXX) $(CXXFLAGS)	-fPIC -c -o	$@	$<	
	@echo "Compiling $@ -- done"

dynamic: create_folders $(D_LIB_OBJ)
	@echo "Creating lib$(LIB).so.1.0"
	@$(CXX) $(CXXFLAGS) -shared 	-o	lib$(LIB).so	$(D_LIB_OBJ) 
	@mv lib$(LIB).so $(LIB_DIR)
	@echo "Creating lib$(LIB).so.1.0 -- done"
	@echo "\033[92mDynamic version of meshsimplification library successfully built\n\033[0m"

#
# Build the tests
#

$(TEST_OBJ_DIR)/%.o: $(TEST_SRC_DIR)/%.cpp $(LIB_INC)
	@echo "Compiling $@" 
	@$(CXX) $(CXXFLAGS)	-c -o	$@	$<	
	@echo "Compiling $@ -- done"
	
$(TEST_BIN_DIR)/%: $(TEST_OBJ_DIR)/%.o dynamic
	@echo "Linking $@" 
	@$(CXX) $(CXXFLAGS)	-o	$@	$<	$(LDFLAGS)	
	@echo "Linking $@ -- done"
	
test: create_folders $(TEST_BIN)
	@echo "\033[92mTests successfully compiled and linked\n\033[0m"

#
# Build the executables
#

$(MAIN_OBJ_DIR)/%.o: $(MAIN_SRC_DIR)/%.cpp $(LIB_INC)
	@echo "Compiling $@" 
	@$(CXX) $(CXXFLAGS)	-c -o	$@	$<	
	@echo "Compiling $@ -- done"
	
$(MAIN_BIN_DIR)/%: $(MAIN_OBJ_DIR)/%.o dynamic
	@echo "Linking $@" 
	@$(CXX) $(CXXFLAGS)	-o	$@	$<	$(LDFLAGS)
	@echo "Linking $@ -- done"
	
main: $(MAIN_BIN)
	@echo "\033[92mExecutables successfully compiled and linked\n\033[0m"

#
# Phony targets
#

.PHONY: clean create_folders doc

clean:
	@$(RM) -r $(OBJ_DIR) $(BIN_DIR) $(LIB_DIR)/lib$(LIB).* $(DOC_DIR)/Doxyfile $(DOC_DIR)/html $(DOC_DIR)/latex
		
create_folders: 
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(S_LIB_OBJ_DIR)
	@mkdir -p $(D_LIB_OBJ_DIR)
	@mkdir -p $(TEST_OBJ_DIR)
	@mkdir -p $(MAIN_OBJ_DIR)
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(MAIN_BIN_DIR)
	@mkdir -p $(TEST_BIN_DIR)

doc:
	@$(RM) -r $(DOC_DIR)/Doxyfile $(DOC_DIR)/html $(DOC_DIR)/latex
	@mkdir -p $(DOC_DIR)
	@doxygen -g $(DOC_DIR)/Doxyfile > /dev/null 2>&1
	@sed -i 's:PROJECT_NAME           = "My Project":PROJECT_NAME           = "A mesh simplification procedure by iterative edge collapsing":' $(DOC_DIR)/Doxyfile
	@sed -i 's:OUTPUT_DIRECTORY       =:OUTPUT_DIRECTORY       = $(DOC_DIR):' $(DOC_DIR)/Doxyfile
	@sed -i 's:INPUT                  =:INPUT                  = $(LIB_INC_DIR) $(LIB_INC_DIR)/implementation $(LIB_INC_DIR)/inline $(LIB_SRC_DIR):' $(DOC_DIR)/Doxyfile
	@sed -i 's:RECURSIVE              =:RECURSIVE              = YES:' $(DOC_DIR)/Doxyfile
	@doxygen $(DOC_DIR)/Doxyfile > /dev/null 2>&1
	
#
# Prevent object files to be deleted
#

.SECONDARY: $(TEST_OBJ) $(MAIN_OBJ)
	
	
