################################################################################
# File        : open_sdk.tcl
# Author      : Michael Wurm <michael.wurm@students.fh-hagenberg.at>
#               Sebastian Kaupper <sebastian.kaupper@students.fh-hagenberg.at>
# Description : Opens Vivado project, then opens SDK
################################################################################

set origin_dir "."
set workspace_dir [file normalize "${origin_dir}/../sdk"]

set bsp_name standalone_bsp_0
set app_name app

puts "(VideoSigXilinx) Opening SDK..."

setws $workspace_dir
puts "(VideoSigXilinx) Set workspace to '[getws]'."

importprojects $workspace_dir/basic_design_wrapper_hw_platform_0
importprojects $workspace_dir/$bsp_name
importprojects $workspace_dir/$app_name
# The following command could probably be replaced with
# 'createlib' in the future (library project).
# This would also remove the need to gitignore imported source files.
importsources -name $app_name -path $workspace_dir/src

set projects [getprojects]
puts "(VideoSigXilinx) Loaded projects:"
set i 0
foreach proj $projects {
  incr i
  puts "                     ($i) $proj"
}

if {[file exists $workspace_dir/RemoteSystemsTempFiles]} {
  # NOTE: Only need to re-generate the BSP when opening the projects
  #       for the first time, for whatever Eclipse reason....
  puts "(VideoSigXilinx) Skipped re-generating of BSP."
  puts "(VideoSigXilinx) Building project '$app_name'..."
  if {[catch {
    projects -build -type app -name $app_name
  } errmsg ]} {
    puts "(VideoSigXilinx) Error while building '$app_name':"
    puts "ErrorMsg: $errmsg"
    puts "ErrorCode: $errorCode"
    puts "ErrorInfo:\n$errorInfo\n"
    exit 1
  }
} elseif {[catch {
  set bsp_to_regenerate $workspace_dir/$bsp_name/system.mss
  puts "(VideoSigXilinx) Re-generating BSP '$bsp_to_regenerate'..."
  regenbsp -bsp $workspace_dir/$bsp_name/system.mss

  puts "(VideoSigXilinx) Building all projects..."
  projects -build
} ]} {
  puts "(VideoSigXilinx) Failed building all projects!"
  exit 1
}
