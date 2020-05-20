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

PROJ_DIR="$SCRIPT_PATH/../$VIVADO_PROJECT_NAME"
ARTIFACTS_DIR="$PROJ_DIR/artifacts"

if [ "$ARG" == "project" ]; then
  $VIVADO_BATCH -source create_project.tcl
  exit 0
elif [ "$ARG" == "synth" ]; then
  $VIVADO_BATCH -source synth.tcl
  exit 0
elif [ "$ARG" == "sdk" ]; then
  xsdk -batch -source open_sdk.tcl
  xsdk -workspace ../sdk #-eclipseargs -perspective org.eclipse.cdt.ui.CPerspective
  exit 0
else
  echo "========================================================================"
  echo "(ERROR) Wrong or invalid argument."
  echo "Usage: ./run.sh <project|synth|sdk>"
  echo "========================================================================"
  exit 1
fi
