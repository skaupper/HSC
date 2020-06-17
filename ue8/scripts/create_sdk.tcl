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

puts "### Opening SDK..."

setws $workspace_dir
cd $workspace_dir
puts "### Set workspace to '[getws]'."

if {![file exists "$hw_name"]} {
  createhw -name $hw_name -hwspec ${design_name}_wrapper.hdf

  # Create driver files
  # NOTE: This is not documented in the XSCT UG1208
  getperipherals $hw_name/system.hdf

}
if {![file exists "$bsp_name"]} {
  createbsp -name $bsp_name -hwproject $hw_name -proc ps7_cortexa9_0 -os standalone
  configbsp -bsp $bsp_name stdin ps7_uart_1
  configbsp -bsp $bsp_name stdout ps7_uart_1
}

updatemss -mss $bsp_name/system.mss
regenbsp -bsp $bsp_name
importprojects $app_name

projects -build

# NOTE: The following command could probably be replaced with
#       'createlib' in the future (library project).
#       This would also remove the need to gitignore imported source files.
# Import more source files to app
#importsources -name $app_name -path src

set projects [getprojects]
puts "### Loaded projects:"
set i 0
foreach proj $projects {
  incr i
  puts "                     ($i) $proj"
}

if {[file exists RemoteSystemsTempFiles]} {
  # NOTE: Only need to re-generate the BSP when opening the projects
  #       for the first time, for whatever Eclipse reason....
  puts "### Skipped re-generating of BSP."
  puts "### Building project '$app_name'..."
  if {[catch {
    projects -build -type app -name $app_name
  } errmsg ]} {
    puts "### Error while building '$app_name':"
    puts "ErrorMsg: $errmsg"
    puts "ErrorCode: $errorCode"
    puts "ErrorInfo:\n$errorInfo\n"
    exit 1
  }
} elseif {[catch {
  set bsp_to_regenerate $bsp_name/system.mss
  puts "### Re-generating BSP '$bsp_to_regenerate'..."
  regenbsp -bsp $bsp_name/system.mss

  puts "### Building all projects..."
  projects -build
} ]} {
  puts "### Failed building all projects!"
  exit 1
}
