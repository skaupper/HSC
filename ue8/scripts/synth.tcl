################################################################################
# File        : synth.tcl
# Author      : Michael Wurm <michael.wurm@students.fh-hagenberg.at>
#               Sebastian Kaupper <sebastian.kaupper@students.fh-hagenberg.at>
# Description : Runs synthesis, implementation and creates bitstream for project
################################################################################

set proj_name "BasicCordicSystem"
set design_name "basic_cordic_design"

set parallel_jobs 4

set project_dir ../$design_name
set cache_dir $project_dir/ip_cache

# Open project
if {[catch {
  open_project $project_dir/$proj_name.xpr
} ]} {
  puts "### ERROR: Couldn't open project."
  exit 1
}

# Set IP cache
if {[catch {
  file mkdir $cache_dir
  config_ip_cache -use_cache_location $cache_dir
  update_ip_catalog
} ]} {
  puts "### ERROR: Failed to set IP cache directory."
  exit 1
}

# Start synthesis
if {[catch {
  puts "--- - Running synthesis"
  reset_run synth_1

  # Optimize the runtime of the synthesis
  set_property STEPS.SYNTH_DESIGN.ARGS.DIRECTIVE RuntimeOptimized [get_runs synth_1]

  # Disable all reports
  set synth_reports [get_report_config -of_object [get_runs synth_1]]
  foreach rpt $synth_reports {
      set_property IS_ENABLED false [get_report_config -of_object [get_runs synth_1] $rpt]
  }

  # Launch synthesis
  launch_runs synth_1 -jobs $parallel_jobs
  wait_on_run synth_1

  # Analyze log files in case of an error
  if {[get_property PROGRESS [get_runs synth_1]] != "100%"} {
    foreach runme_log_file [glob $project_dir/$proj_name.runs/*/*runme.log] {
      set fp [open "$runme_log_file" r]
      set line_ctr 0
      while {[gets $fp line] >= 0} {
        incr line_ctr
        if {[string match -nocase {*ERROR:*} $line]} {
          puts "### An error occured in file '$runme_log_file', line $line_ctr."
          puts "### The error reads: $line"
          close $fp
          exit 1
        }
      }
      close $fp
    }
  }
} ]} {
  puts "### ERROR: Synthesis failed."
  exit 1
}

# Start implementation
if {[catch {
  puts "--- - Running implementation"
  reset_run impl_1

  # Optimize the runtime of the implementation
  set_property STEPS.ROUTE_DESIGN.ARGS.DIRECTIVE RuntimeOptimized [get_runs impl_1]
  set_property STEPS.PLACE_DESIGN.ARGS.DIRECTIVE RuntimeOptimized [get_runs impl_1]

  # Disable all reports
  set impl_reports [get_report_config -of_object [get_runs impl_1]]
  foreach rpt $impl_reports {
      set_property IS_ENABLED false [get_report_config -of_object [get_runs impl_1] $rpt]
  }

  # Launch implementation
  launch_runs impl_1 -jobs $parallel_jobs
  wait_on_run impl_1

  if {[get_property PROGRESS [get_runs impl_1]] != "100%"} {
    puts "### ERROR: Implementation returned with progress lower than 100%."
    exit 1
  }
} ]} {
  puts "### ERROR: Implementation failed."
  exit 1
}

if {[catch {
  puts "--- - Creating bitstream"
  reset_run impl_1 -from_step write_bitstream
  launch_runs impl_1 -to_step write_bitstream -jobs $parallel_jobs
  wait_on_run impl_1
} ]} {
  puts "### ERROR: Bitstream generation failed."
  exit 1
}

puts "--- - Exporting HDF file."
set binaries ../binaries
set impl_dir $project_dir/${proj_name}.runs/impl_1

file mkdir $binaries
file copy -force $impl_dir/${design_name}_wrapper.sysdef $binaries/${design_name}_wrapper.hdf

puts "--- - Exporting Bitstream file."
file copy -force $impl_dir/${design_name}_wrapper.bit $binaries/

puts "### Done."
exit 0
