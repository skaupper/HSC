#!/usr/bin/env bash
################################################################################
# File        : run.sh
# Author      : Michael Wurm <michael.wurm@students.fh-hagenberg.at>
#               Sebastian Kaupper <sebastian.kaupper@students.fh-hagenberg.at>
# Description : Abstraction of main actions on the Vivado HLS project
################################################################################

set -euo pipefail

ARG=${1:-"unset"}

SCRIPT_PATH=$(realpath $(dirname $BASH_SOURCE))
cd $SCRIPT_PATH

VIVADO_HLS_BATCH="vivado_hls -f"
PROJ_DIR=$SCRIPT_PATH/../generated/

mkdir -p $PROJ_DIR
cd $PROJ_DIR


if [ "$ARG" == "project_hls" ]; then
  $VIVADO_HLS_BATCH $SCRIPT_PATH/create_hls_project.tcl
  exit 0
elif [ "$ARG" == "synth_hls" ]; then
  $VIVADO_HLS_BATCH $SCRIPT_PATH/synth_hls.tcl
  exit 0
elif [ "$ARG" == "export_hls" ]; then
  $VIVADO_HLS_BATCH $SCRIPT_PATH/export_hls.tcl
  exit 0
else
  echo "========================================================================"
  echo "(ERROR) Wrong or invalid argument."
  echo "Usage: ./run.sh <project_hls|synth_hls|export_hls>"
  echo "========================================================================"
  exit 1
fi
