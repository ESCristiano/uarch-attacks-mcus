# Tool To Generate the channel matrix 
- Tool to **generate a channel matrix** from a matrix sample file with input x output  -> tool from https://ts.data61.csiro.au/software/TS/channel_tools/index.html
	- Blog https://ts.data61.csiro.au/projects/TS/timeprotection/arch-mitigation.html of results from the paper **"Your Processor Leaks Information – and There’s Nothing You Can Do About It"** by Qian Ge
- **Gnuplot** : To generate the channel matrix
	- This tool is very interesting. Is a command line "matlab-alike" tool that let us plot a lot of graphs 
	- It cal also be used in Latex. Good for using in papers  

## Commands
IN -> Users/cris9/Downloads/channel_tools-2019-02-14/channel_tools/test
````bash
../channel_matrix samples.cm < samples
../extract_plot samples.cm 7709 256 > samples.plot
````
-> **7709** and **256** is the maximum Y value (7709) and the maximum X value(256) of the channel matrix. It is output by the channel_matrix command

IN -> gnuplot
````bash
plot '/Users/cris9/Downloads/channel_tools-2019-02-14/channel_tools/test/samples.plot' with image
````


## Scripts to Generate the channel matrix

In the test directories under the **TEEx_Attackx_TestScenariox_Boardx**/test_material dir, there is a script called ``generate_matrix.sh`` that will generate the output matrix.

**Thinhs needed for this process**
1. Obtain a samples file with the following structure and call it ``samples``
	````
	LINE 1      TIME X
	LINE 2		TIME Y
	LINE .		TIME .
	LINE .		TIME .
	LINE N      TIME Z
	````
	- put that file under ``TEEx_Attackx_TestScenariox_Boardx/test_material/results/``
2. Run the script ``generate_matrix.sh`` 
	- The script will ask you for the name of the test. No restrictions, but a advice you for coherence purposes to put the name of the test' path plus the name of the specific test, like. TEEx_Attackx_TestScenariox_Boardx**_test_name**
3. The output of the test will be shown in the system image viewer and the test files, are stored in ``./results/TEEx_Attackx_TestScenariox_Boardx_test_name``