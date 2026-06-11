# TF-M Build For Nordic NRF9160 Level 1

## CONFIG BUILD DIR

````batch
//in build dir:
cmake .. -DTFM_ISOLATION_LEVEL=2 -DTFM_PSA_API=ON -DCMAKE_BUILD_TYPE=Debug -DTFM_PLATFORM=C:/paper_tees_study/tees/TF-M/platform/ext/target/nordic_nrf/nrf9160dk_nrf9160 -DTFM_TOOLCHAIN_FILE=C:/paper_tees_study/tees/TF-M/toolchain_GNUARM.cmake -G"Unix Makefiles"
````

 - **DTFM_PSA_API**=ON -> built in IPC mode 
 - **DTFM_ISOLATION_LEVEL**=2 (isolation level 2,  Isolation within the SPE, SPM separated from Secure Applications running in unpriv mode )

## BUILD & FLASH
## Go to the the ``Test Direct``  and RUN the following scripts 
1.  ``setup_code.sh`` -> Copy the test' code to the build_dir and the TA code to the tf-m secure partition dir
2.  ``compile.sh`` -> To compile the project issuing automatically
3. ``flash_board.sh`` -> to flash the board


**Connect the board to a terminal** ->  You should see life signals from her.


## Change in source files
Nordic just lets config the cache in the secure world and in TF-M v1.3 there are no configuration/activation of the cache. However, in tf-m v1.4 they changed it and config/enable the cache by default in the Secure world. So, if you are running TF-M v1.3 you need to add the code line below to the file ``trusted-firmware-m/platform/ext/target/nordic_nrf/common/nrf9160`` (line 233) or changed the file itself by the file provided by tf-m v1.4.

````C
/* Enable the instruction cache (this can be done only from secure code;
     * that's why it is placed here).
     */
    nrf_nvmc_icache_config_set(NRF_NVMC, NRF_NVMC_ICACHE_ENABLE);
````
