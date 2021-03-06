set SRC_DIR  "../../src"
set CPPFLAGS "-I$SRC_DIR"

open_project -reset prj
add_files       $SRC_DIR/cordic/cordic_cc.cpp     -cflags $CPPFLAGS
add_files -tb   $SRC_DIR/cordic/cordic.hpp
add_files -tb   $SRC_DIR/tb/top.h
add_files -tb   $SRC_DIR/tb/CordicTb.h
add_files -tb   $SRC_DIR/tb/cordic_syn.h
add_files -tb   $SRC_DIR/tb/CordicTb.cpp          -cflags $CPPFLAGS
add_files -tb   $SRC_DIR/tb/main.cpp              -cflags $CPPFLAGS

open_solution -reset solution

set_top cordic_cc
create_clock -period 20 -name default
# TODO: is it possible to set the MiniZed board directly?
set_part "xc7z007sclg225-1"

config_rtl -reset control -reset_level low

quit
