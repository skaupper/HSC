cpp: all.sim all.tb
all: all.sim all.tb all.hls all.vivado
clean: clean.sim clean.tb clean.hls clean.vivado

# Run simulation on emulated CPU
EMUCPU=1

-include Makefile.sim
-include Makefile.tb
-include Makefile.hls
-include Makefile.vivado

.PHONY: all clean cpp
