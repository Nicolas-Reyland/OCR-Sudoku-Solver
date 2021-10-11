#!/bin/sh
if [ -e main ]; then
	rm -f main
fi
for object_file in $(find src/ -type f -name "*.o")
do
	rm -f "$object_file"
done
