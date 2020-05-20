################################################################################
# File        : create_bd.tcl
# Author      : Michael Wurm <wurm.michael95@gmail.com>
# Description : Creates block design *.bd
################################################################################

puts "--- - Creating block design"

# Create block design
source $origin_dir/../src/bd/$proj_name.tcl

# Generate the wrapper
set design_name [get_bd_designs]
make_wrapper -files [get_files $design_name.bd] -top -import

puts "(VideoSigXilinx) Block design created."
