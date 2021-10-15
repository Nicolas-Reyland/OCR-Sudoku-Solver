#!/bin/bash

# main program
if [ -e main ]; then
	rm -f main
fi
# object files everywhere in src/
for object_file in $(find src/ -type f -name "*.o")
do
	rm -f "$object_file"
done
# bmp files for the gui
list=$(echo src/gui/{grayscale,{blurred,binarised}_image}.bmp)
for bmp_img in $list
do
	rm -f $bmp_img
done