BIN_NAME=ue02_tlm_lt

# Compile and link flags
DEFINES=
CPPFLAGS=-Wall -Werror -pedantic --std=c++17 -O0 -g $(addprefix -D,$(DEFINES)) -I $(INC_DIR)
LDFLAGS=-lpthread -lsystemc

# Directory constants
BIN_DIR=bin
SRC_DIR=src
INC_DIR=../common
OBJ_DIR=obj

# Source and object files
BINARY=$(BIN_DIR)/$(BIN_NAME)
SOURCES=$(wildcard $(SRC_DIR)/*.cpp)
OBJECTS=$(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)


# General targets
all: $(BIN_DIR) $(OBJ_DIR) $(BINARY)

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)


# Compile and link targets
$(BINARY): $(OBJECTS)
	$(CXX) $^ $(LDFLAGS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) -c $< $(CPPFLAGS) -o $@



.PHONY: all clean
