cpp: all.sim all.tb

all: all.sim all.tb all.hls all.vivado

clean: clean.sim clean.tb clean.hls clean.vivado
	rm -rf generated/


# Run simulation on emulated CPU
EMUCPU=1


-include makefiles/Makefile.sim
-include makefiles/Makefile.tb
-include makefiles/Makefile.hls
-include makefiles/Makefile.vivado


.PHONY: all clean cpp
