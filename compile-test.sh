#!/bin/bash
clear
export FLAGS="-Wall -Wextra"
if [ -n "$2" ]; then
	if [[ "$2" == "nowarn" ]]; then
		echo "no warning"
		unset FLAGS
	fi
fi
args=()
for file in $(find src/{nn,utils} -type f -name "*.c")
do
	args+=("$file")
done
export src_path=tests/nn/loading-data/loading-data.c
if [ -n "$1" ]; then
	src_path=$1
	echo "custom src path: $src_path"
fi
gcc -o "test.out" "$src_path" "${args[@]}" -Isrc $FLAGS -lm
