#!/usr/bin/env bash
################################################################################
# File        : run.sh
# Author      : Michael Wurm <michael.wurm@students.fh-hagenberg.at>
#               Sebastian Kaupper <sebastian.kaupper@students.fh-hagenberg.at>
# Description : Abstraction of main actions on the Vivado project
################################################################################

set -euo pipefail

ARG=${1:-"unset"}

SCRIPT_PATH=$(realpath $(dirname $BASH_SOURCE))
cd $SCRIPT_PATH

VIVADO_BATCH="vivado -mode batch -nolog -nojournal -notrace"
VIVADO_PROJECT_NAME="basic_system"

PROJ_DIR="$SCRIPT_PATH/../../$VIVADO_PROJECT_NAME"
ARTIFACTS_DIR="$PROJ_DIR/artifacts"

if [ "$ARG" == "project" ]; then
  $VIVADO_BATCH -source create_project.tcl
  exit 0
elif [ "$ARG" == "synth" ]; then
  $VIVADO_BATCH -source synth.tcl

  #  echo "--- - Extracting Vivado artifacts"
  #  mkdir -p $ARTIFACTS_DIR

  #  echo "Copying files into artifacts."
  #  mv $PROJ_DIR/${VIVADO_PROJECT_NAME}.runs/impl_1/${VIVADO_PROJECT_NAME}_wrapper.bit $ARTIFACTS_DIR
  #  mv $PROJ_DIR/${VIVADO_PROJECT_NAME}.sdk/${VIVADO_PROJECT_NAME}_wrapper.hdf $ARTIFACTS_DIR
  exit 0
elif [ "$ARG" == "sdk" ]; then
  xsdk -batch -source open_sdk.tcl
  xsdk -workspace $PROJ_DIR/sdk #-eclipseargs -perspective org.eclipse.cdt.ui.CPerspective
  exit 0
elif [ "$ARG" == "format" ]; then
  folders_to_lint="../sdk/src ../sdk/standalone_bsp_0_Passthrough_A53_1"

  for folder in $folders_to_lint; do
    for file in $(git ls-files "$folder/*.[ch]"); do
      echo "Formatting: '$file'"
      clang-format-6.0 -i $file
    done
  done
  echo "Done formatting all relevant C source and header files."
  exit 0
else
  echo "========================================================================"
  echo "(ERROR) Wrong or invalid argument."
  echo "Usage: ./run.sh <project|synth|sdk|format>"
  echo "========================================================================"
  exit 1
fi
