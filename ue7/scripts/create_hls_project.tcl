open_project -reset prj
add_files       ../src/syn/cordic_cc.cpp     -cflags "--std=c++11"
add_files -tb   ../src/syn/cordic.hpp
add_files -tb   ../src/sim/top.h
add_files -tb   ../src/sim/CordicTb.h
add_files -tb   ../src/sim/cordic_syn.h
add_files -tb   ../src/sim/CordicTb.cpp      -cflags "--std=c++11"
add_files -tb   ../src/sim/main.cpp          -cflags "--std=c++11"

open_solution -reset solution

set_top cordic_cc
config_export -ipname cordic_cc
create_clock -period 20 -name default
# TODO: is it possible to set the MiniZed board directly?
set_part "xc7z007sclg225-1"


quit
