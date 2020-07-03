#!/usr/bin/env bash
################################################################################
# File        : run.sh
# Author      : Michael Wurm <michael.wurm@students.fh-hagenberg.at>
#               Sebastian Kaupper <sebastian.kaupper@students.fh-hagenberg.at>
# Description : Abstraction of main actions on the Vivado project
################################################################################

set -euo pipefail

ARG=${1:-"unset"}
GUI=${2:-"unset"}

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
  if [ "$GUI" == "1" ]; then
    xsdk -workspace ../sdk
  fi
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

  # Unpack the IP zip package
  IP_PACKAGE=$PROJ_DIR/**/**/impl/ip/*.zip
  IP_DESTINATION=$IP_DIR/cordic
  rm -r $IP_DESTINATION/*
  unzip -o $IP_PACKAGE -d $IP_DESTINATION
  # Add a custom IP Logo for the Block Design
  #cp ./src/img/logo.png $IP_DESTINATION/misc/logo.png

  cd -
  exit 0
else
  echo "===================================================================================="
  echo "(ERROR) Wrong or invalid argument."
  echo "Usage: ./run.sh <project_vivado|synth_vivado|sdk|project_hls|sim_hls|synth_hls|export_hls>"
  echo "===================================================================================="
  exit 1
fi
