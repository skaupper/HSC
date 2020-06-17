cpp: all.sim all.tb

hw: all.hls all.vivado

all: cpp hw clang-format

clang-format:
	@echo "Auto formatting all C/CPP files under src/"
	clang-format-6.0 -i $$(git ls-files 'src/*.[ch]')
	clang-format-6.0 -i $$(git ls-files 'src/*.[ch]pp')

clean: clean.sim clean.tb clean.hls clean.vivado
	rm -rf generated/


# Run simulation on emulated CPU
EMUCPU=1


-include makefiles/Makefile.sim
-include makefiles/Makefile.tb
-include makefiles/Makefile.hls
-include makefiles/Makefile.vivado
-include makefiles/Makefile.sdk


.PHONY: all clean cpp hw clang-format
