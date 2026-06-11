#!/bin/bash

BUILD_DIR=TF-M_Build_STM32L552_Level_2

srec_cat $BUILD_DIR/bin/tfm_s_ns_signed.bin -binary --offset=0x10000 -o $BUILD_DIR/bin/tfm_s_ns_signed.hex -intel

cmd.exe << EOF
	nrfjprog.exe --eraseall -f nrf91
	nrfjprog.exe --program ../../../../tees/TF-M/TF-M_Build_nRF9160_Level_2/bin/bl2.hex -f nrf91 --sectorerase
	nrfjprog.exe --program ../../../../tees/TF-M/TF-M_Build_nRF9160_Level_2/bin/tfm_s_ns_signed.hex -f nrf91 --sectorerase
	nrfjprog.exe --reset -f nrf91
EOF

#nrfjprog.exe