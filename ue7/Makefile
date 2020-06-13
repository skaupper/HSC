BIN_NAME=ue7

# Compile and link flags
DEFINES=
CPPDEFINES=$(DEFINES) SC_INCLUDE_FX WRITE_TRACE
CPPFLAGS=-Wall -Werror -Wno-unknown-pragmas -pedantic --std=c++11 -O0 -g $(addprefix -D,$(CPPDEFINES))
LDFLAGS=-lpthread -lsystemc

# Directory constants
BIN_DIR=bin
SRC_DIRS=src/syn src/sim
OBJ_DIR=obj

# Source and object files
BINARY=$(BIN_DIR)/$(BIN_NAME)

CPP_SOURCES=$(foreach CUR_SRC_DIR,$(SRC_DIRS),$(wildcard $(CUR_SRC_DIR)/*.cpp))
SOURCES=$(CPP_SOURCES)

CPP_OBJECTS=$(CPP_SOURCES:%.cpp=%.o)
OBJECTS_NO_PREFIX=$(CPP_OBJECTS)
OBJECTS=$(addprefix $(OBJ_DIR)/,$(OBJECTS_NO_PREFIX))

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



#
# HLS specific targets
#

SCRIPT_DIR=scripts

hls: | hls_project hls_synth hls_export

hls_project:
	$(SCRIPT_DIR)/run.sh project_hls

hls_synth:
	$(SCRIPT_DIR)/run.sh synth_hls

hls_export:
	$(SCRIPT_DIR)/run.sh export_hls

hls_clean:
	rm -rf generated/


.PHONY: all clean hls hls_project hls_synth hls_export
