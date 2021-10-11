#!/bin/bash
clear
args=()
for file in $(find src/nn/ -type f -name "*.c")
do
	args+=("$file")
done
for file in $(find src/mem/ -type f -name "*.c")
do
	args+=("$file")
done
gcc -o "test.out" -lm test.c "${args[@]}" -Isrc
