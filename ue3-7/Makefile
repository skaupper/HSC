BIN_NAME=ue37

# Compile and link flags
DEFINES=
CPPFLAGS=-Wall -Werror -pedantic --std=c++17 -O0 -g $(addprefix -D,$(DEFINES))
CFLAGS=
LDFLAGS=-lpthread -lsystemc

# Directory constants
BIN_DIR=bin
SRC_DIRS=src src/firmware src/sim
OBJ_DIR=obj

SRC_EXTENSIONS=cpp c

# Source and object files
BINARY=$(BIN_DIR)/$(BIN_NAME)

CPP_SOURCES=$(foreach CUR_SRC_DIR,$(SRC_DIRS),$(wildcard $(CUR_SRC_DIR)/*.cpp))
C_SOURCES=$(foreach CUR_SRC_DIR,$(SRC_DIRS),$(wildcard $(CUR_SRC_DIR)/*.c))
SOURCES=$(CPP_SOURCES) $(C_SOURCES)

CPP_OBJECTS=$(CPP_SOURCES:%.cpp=%.o)
C_OBJECTS=$(C_SOURCES:%.c=%.o)
OBJECTS_NO_PREFIX=$(CPP_OBJECTS) $(C_OBJECTS)
OBJECTS=$(addprefix $(OBJ_DIR)/,$(OBJECTS_NO_PREFIX))



# General targets
all: $(BIN_DIR) $(OBJ_DIR) $(BINARY)

print:
	@echo "Sources:"
	@echo $(SOURCES)
	@echo
	@echo "Objects:"
	@echo $(OBJECTS)

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

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(shell dirname $@)
	$(CXX) -c $< $(CPPFLAGS) -o $@

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(shell dirname $@)
	$(CC) -c $< $(CFLAGS) -o $@



.PHONY: all clean
