# TF-M Build For STM32L552 Level 1

## CONFIG BUILD DIR

````batch
//in build dir:
cmake .. -DTFM_ISOLATION_LEVEL=2 -DTFM_PSA_API=ON -DCMAKE_BUILD_TYPE=Debug -DTFM_PLATFORM=C:/paper_tees_study/tees/TF-M/platform/ext/target/stm/nucleo_l552ze_q -DTFM_TOOLCHAIN_FILE=C:/paper_tees_study/tees/TF-M/toolchain_GNUARM.cmake -G"Unix Makefiles"
````

 - **DTFM_PSA_API**=ON -> built in IPC mode 
 - **DTFM_ISOLATION_LEVEL**=2 (isolation level 2,  Isolation within the SPE, SPM separated from Secure Applications running in unpriv mode )
 - **DTFM_PLATFORM**=C:/uTEEs_paper/uTEE_TF_M/TF_M/platform/ext/target/stm/nucleo_l552ze_q -> Full path for the plataform config files 

## BUILD & FLASH
## Go to the the ``Test Direct``  and RUN the following scripts 
1.  ``setup_code.sh`` -> Copy the test' code to the build_dir and the TA code to the tf-m secure partition dir
2.  ``compile.sh`` -> To compile the project issuing automatically
3. ``flash_board.sh`` -> to flash the board


**Connect the board to a terminal** ->  You should see life signals from her.
