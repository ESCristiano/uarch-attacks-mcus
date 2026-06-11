#!/bin/bash
BUILD_DIR=../../../../tees/TF-M/TF-M_Build_MuscaB1_Level_3

srec_cat $BUILD_DIR/bin/bl2.bin -Binary -offset 0xA000000 $BUILD_DIR/bin/tfm_s_ns_signed.bin -Binary -offset 0xA020000 -o tfm.hex -Intel

# # windows 
# cmd.exe << EOF
# 	xcopy .\tfm.hex E:
# EOF

cat ./README_flash.txt