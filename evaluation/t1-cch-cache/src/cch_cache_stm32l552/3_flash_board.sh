#!/bin/bash

BUILD_DIR=../../../../tees/TF-M/TF-M_Build_STM32L552_Level_2

# postbuild.sh  -> To know where the binaries should be placed
$BUILD_DIR/postbuild.sh
# regression.sh -> To prepare/config the board
$BUILD_DIR/regression.sh
# TFM_UPDATE.sh -> To flash the NS, S and SBSFU binaries  to the board
$BUILD_DIR/TFM_UPDATE.sh


# # windows 
# cmd.exe << EOF
# 	xcopy .\tfm.hex D:
# EOF