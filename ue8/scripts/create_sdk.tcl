################################################################################
# File        : open_sdk.tcl
# Author      : Michael Wurm <michael.wurm@students.fh-hagenberg.at>
#               Sebastian Kaupper <sebastian.kaupper@students.fh-hagenberg.at>
# Description : Opens Vivado project, then opens SDK
################################################################################

set origin_dir [file dirname [info script]]
set workspace_dir [file normalize "${origin_dir}/../generated/sdk"]

set design_name "basic_cordic_design"

set hw_name ${design_name}_hw
set bsp_name standalone_bsp
set app_name app

set hdf_filepath ../binaries/${design_name}_wrapper.hdf
set bit_filepath ../binaries/${design_name}_wrapper.bit

puts "### Opening SDK..."

setws $workspace_dir
cd $workspace_dir
puts "### Set workspace to '[getws]'."

puts "--- Creating HW platform..."
if {![file exists "$hw_name"]} {
  createhw -name $hw_name -hwspec $hdf_filepath

  # Create driver files
  # NOTE: This is not documented in the XSCT UG1208
  getperipherals $hw_name/system.hdf

  # Replace the auto-generated bitfile with the 'real' one.
  file copy -force $bit_filepath $hw_name/
}

puts "--- Creating BSP..."
if {![file exists "$bsp_name"]} {
  createbsp -name $bsp_name -hwproject $hw_name -proc ps7_cortexa9_0 -os standalone
  configbsp -bsp $bsp_name stdin ps7_uart_1
  configbsp -bsp $bsp_name stdout ps7_uart_1

  updatemss -mss $workspace_dir/$bsp_name/system.mss
  regenbsp -bsp $bsp_name
}

puts "--- Importing main application..."
importprojects ../../src/sdk/$app_name

puts "--- Importing other sources..."
# The following command could probably be replaced with
# 'createlib' in the future (library project).
# This would also remove the need to gitignore imported source files.
#importsources -name $app_name -path ../../src/firmware

puts "--- Loaded projects:"
set projects [getprojects]
set i 0
foreach proj $projects {
  incr i
  puts "\t($i) $proj"
}

if {[catch {
  puts "--- Building all projects..."
  puts "(skipping this for now)"
  #projects -build
} errmsg ]} {
  puts "--- Failed building all projects!"
  puts "--- Error information:"
  puts "ErrorMsg: $errmsg"
  puts "ErrorCode: $errorCode"
  puts "ErrorInfo:\n$errorInfo\n"
  exit 1
}
