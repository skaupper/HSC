################################################################################
# File        : create_bd.tcl
# Author      : Michael Wurm <michael.wurm@students.fh-hagenberg.at>
#               Sebastian Kaupper <sebastian.kaupper@students.fh-hagenberg.at>
# Description : Creates block design *.bd
################################################################################

puts "--- - Creating block design"

# Create block design
source $origin_dir/../src/bd/$_xil_proj_name_.tcl

# Generate the wrapper
set design_name [get_bd_designs]
make_wrapper -files [get_files $design_name.bd] -top -import

puts "### Block design created."
