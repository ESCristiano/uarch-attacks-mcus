#!/bin/bash
BUILD_DIR=../../../../tees/TF-M/TF-M_Build_nRF9160_Level_2

cp -R src/NS/* $BUILD_DIR/lib/ext/tfm_test_repo-src/app/
cp -R src/uArchChannel_lib $BUILD_DIR/lib/ext/tfm_test_repo-src/

cp -R src/TAs/covert/* $BUILD_DIR/../secure_fw/partitions/covert
cp -R src/uArchChannel_lib $BUILD_DIR/../secure_fw/partitions/covert
# cp -R src/TAs/appRoT/* $BUILD_DIR/../secure_fw/partitions/appRoT


