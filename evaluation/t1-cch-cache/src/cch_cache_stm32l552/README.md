# uTEEx_Testx_Boardx

##  BUILD & FLASH 
1.  ``setup_code.sh`` -> Copy the test' code to the build_dir and the TA code to the tf-m secure partition dir
2.  ``compile.sh`` -> To compile the project
3. ``flash_board.sh`` -> To flash the board

**Connect the board to a terminal** ->  You should see life signals from her.

## Folder Structure
**\src** -> This structure can vary from test to test, and is up to the dev to define that and adapt the scripts under `uTEEx_Testx_Boardx` to it
- **NS_code** -> Some Code that will Run in the NS world
- **S_code**  -> Some Code that will Run in the S world (a.k.a, TAs)

**\results** -> This folder is where will be placed the raw data from tests as well as the process data and respective graphs
- **samples.txt** -> raw data 
