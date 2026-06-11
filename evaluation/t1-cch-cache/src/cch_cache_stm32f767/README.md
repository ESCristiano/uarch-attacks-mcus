# TF-M_ICache-Covert_sce1_STM32L552

## Schematic

[](./test_material/imgs/img_TF_M_arch.svg) 

## 1 - BUILD & FLASH 
0. ``0_upate_covert_src.sh`` -> To copy the covert code from the common dir (i.e., uTEEs/tools) to the TA and NS app code in this dir
1.  ``setup_code.sh`` -> Copy the test' code to the build_dir and the TA code to the tf-m secure partition dir
2.  ``compile.sh`` -> To compile the project issuing automatically
3. ``flash_board.sh`` -> to flash the board


**Connect the board to a terminal** ->  You should see life signals from her.


## 2 - Scripts to Generate the channel matrix

Script  ``generate_matrix.sh``  will generate the output matrix.

**Thinhs needed for this process**
1. Obtain a samples file with the following structure and call it ``samples``
	````
	LINE 1		TIME X
	LINE 2		TIME Y
	LINE .		TIME .
	LINE .		TIME .
	LINE N		TIME Z
	````
	- put that file under ``TEEx_Attackx_TestScenariox_Boardx/results/``
2. Run the script ``generate_matrix.sh`` 
	- The script will ask you for the name of the test. No restrictions, but a advice you for coherence purposes to put the name of the test' path plus the name of the specific test, like. TEEx_Attackx_TestScenariox_Boardx**_test_name**
3. The output of the test will be shown in the system image viewer and the test files, are stored in ``./results/TEEx_Attackx_TestScenariox_Boardx_test_name``

## NOTES
1. The TA and NS app must be changed in this directory  
2. The covert_src code has to be changed in the common directory (``uTEE_TF_M/Tools/covert_src``)
