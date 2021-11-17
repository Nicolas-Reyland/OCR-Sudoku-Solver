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
export src_path=extra/test.c
if [ -n "$1" ]; then
	src_path="$1"
	echo "custom src path: $src_path"
fi
gcc -o "$(basename $src_path).out" "$src_path" "${args[@]}" -Isrc $FLAGS -lm
