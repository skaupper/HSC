BIN_NAME=ue37

# Compile and link flags
DEFINES=
CPPDEFINES=$(DEFINES) SC_INCLUDE_FX
CDEFINES=$(DEFINES)
CPPFLAGS=-Wall -Werror -pedantic --std=c++11 -O0 -g $(addprefix -D,$(CPPDEFINES))
CFLAGS=-Wall -Werror -pedantic --std=gnu11 -O0 -g $(addprefix -D,$(CDEFINES))
LDFLAGS=-lpthread -lsystemc

# Directory constants
BIN_DIR=bin
SRC_DIRS=src src/firmware src/sim
OBJ_DIR=obj

# Source and object files
BINARY=$(BIN_DIR)/$(BIN_NAME)

CPP_SOURCES=$(foreach CUR_SRC_DIR,$(SRC_DIRS),$(wildcard $(CUR_SRC_DIR)/*.cpp))
C_SOURCES=$(foreach CUR_SRC_DIR,$(SRC_DIRS),$(wildcard $(CUR_SRC_DIR)/*.c))
SOURCES=$(CPP_SOURCES) $(C_SOURCES)

CPP_OBJECTS=$(CPP_SOURCES:%.cpp=%.o)
C_OBJECTS=$(C_SOURCES:%.c=%.o)
OBJECTS_NO_PREFIX=$(CPP_OBJECTS) $(C_OBJECTS)
OBJECTS=$(addprefix $(OBJ_DIR)/,$(OBJECTS_NO_PREFIX))

C_DEPS=$(addprefix $(OBJ_DIR)/,$(C_OBJECTS:%.o=%.d))
CPP_DEPS=$(addprefix $(OBJ_DIR)/,$(CPP_OBJECTS:%.o=%.d))


# General targets
all: $(BIN_DIR) $(OBJ_DIR) $(BINARY)

print:
	@echo "Sources:"
	@echo $(SOURCES)
	@echo
	@echo "Objects:"
	@echo $(OBJECTS)
	@echo
	@echo "C++ Deps:"
	@echo $(CPP_DEPS)

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

-include $(CPP_DEPS)
$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(shell dirname $@)
	$(CXX) -c $< $(CPPFLAGS) -MMD -o $@

-include $(C_DEPS)
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(shell dirname $@)
	$(CC) -c $< $(CFLAGS) -MMD -o $@



.PHONY: all clean
