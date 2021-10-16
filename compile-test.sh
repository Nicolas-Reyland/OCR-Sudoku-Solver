#!/bin/bash
clear
args=()
for file in $(find src/{nn,utils} -type f -name "*.c")
do
	args+=("$file")
done
gcc -o "test.out" tests/0-nn/alloc-model/test.c "${args[@]}" -Isrc -Wall -Wextra -lm
