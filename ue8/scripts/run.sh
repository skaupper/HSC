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
VIVADO_HLS_BATCH="vivado_hls -f"
VIVADO_PROJECT_NAME="basic_cordic_system"

GENERAL_OUT_DIR=$SCRIPT_PATH/../generated
PROJ_DIR=$GENERAL_OUT_DIR/cordic_cc
IP_DIR=$GENERAL_OUT_DIR/ips


mkdir -p $PROJ_DIR
mkdir -p $IP_DIR

cd $PROJ_DIR


if [ "$ARG" == "project_vivado" ]; then
  $VIVADO_BATCH -source $SCRIPT_PATH/create_project.tcl
  exit 0
elif [ "$ARG" == "synth_vivado" ]; then
  $VIVADO_BATCH -source $SCRIPT_PATH/synth.tcl
  exit 0
elif [ "$ARG" == "sdk" ]; then
  xsdk -batch -source $SCRIPT_PATH/create_sdk.tcl
  xsdk -workspace ../sdk
  exit 0
elif [ "$ARG" == "project_hls" ]; then
  $VIVADO_HLS_BATCH $SCRIPT_PATH/create_hls_project.tcl
  exit 0
elif [ "$ARG" == "sim_hls" ]; then
  $VIVADO_HLS_BATCH $SCRIPT_PATH/sim_hls.tcl
  exit 0
elif [ "$ARG" == "synth_hls" ]; then
  $VIVADO_HLS_BATCH $SCRIPT_PATH/synth_hls.tcl
  exit 0
elif [ "$ARG" == "export_hls" ]; then
  $VIVADO_HLS_BATCH $SCRIPT_PATH/export_hls.tcl
  mkdir -p $IP_DIR/cordic
  cp $PROJ_DIR/**/**/impl/ip/*.zip $IP_DIR/cordic
  cd $IP_DIR/cordic
  unzip *.zip
  cd -
  exit 0
else
  echo "===================================================================================="
  echo "(ERROR) Wrong or invalid argument."
  echo "Usage: ./run.sh <project_vivado|synth_vivado|sdk|project_hls|sim_hls|synth_hls|export_hls>"
  echo "===================================================================================="
  exit 1
fi
