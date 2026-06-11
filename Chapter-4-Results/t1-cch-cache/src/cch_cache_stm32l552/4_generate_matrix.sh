#!/usr/bin/env bash
ROOT="$(realpath "$(dirname "$0")")"

CHANNEL_TOOLS=${ROOT}/../channel_tools
RESULTS=${ROOT}/Results

TEST_NAME=STM32L552_native_channel

rm -rf ${RESULTS}/${TEST_NAME}

# To create the TEST_NAME dir
mkdir ${RESULTS}/${TEST_NAME}
cp ${RESULTS}/samples.txt ${RESULTS}/${TEST_NAME}/${TEST_NAME}.samples

# Call the channel tools to creat the matrix and put it in a .plot file to be used by the gnuplot
${CHANNEL_TOOLS}/channel_matrix ${RESULTS}/${TEST_NAME}/${TEST_NAME}.cm < ${RESULTS}/${TEST_NAME}/${TEST_NAME}.samples
${CHANNEL_TOOLS}/extract_plot ${RESULTS}/${TEST_NAME}/${TEST_NAME}.cm 8000 256 > ${RESULTS}/${TEST_NAME}/${TEST_NAME}.plot

#Call gnuplot to create the matix
gnuplot << EOF
	set terminal png size 1500,800
	set output '${RESULTS}/${TEST_NAME}/${TEST_NAME}.png'
	set xrange[0:255]
	set yrange[1556:7700]
	plot '${RESULTS}/${TEST_NAME}/${TEST_NAME}.plot' with image
EOF

#Show the matrix
#xdg-open ${RESULTS}/${TEST_NAME}/${TEST_NAME}.png
