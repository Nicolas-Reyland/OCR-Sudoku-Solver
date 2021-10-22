#!/bin/bash
# Test file for ...

# Testing ... what you are testing ...
# Steps :
# 1. check training of neural network
# Sub-steps :
# 1.
#  - compile back-and-forth.c
#  - compare back-and-forth.c compiled program execution output to output.txt

# Test Description
export TEST_STEPS_DESCR=(2)

# Declare variabes & functions
# ...

# Steps preparation
# Declare variables
export abs_program_path=""
export dot_c_files=()

# Steps preparation
function prepare_steps {
	# list of .c files in src/{nn,utils}
	dot_c_files=()
	for file in $(find $project_root_path/src/{nn,utils} -type f -name "*.c")
	do
		dot_c_files+=("$file")
	done
	# absolute program path
	abs_program_path="$tmp_dir/nn-back-and-forth.out"
	if [ -f $abs_program_path ]; then
		rm -f $abs_program_path
	fi
}

# Step 1
function step_1 {
	case $1 in
		1)
			$(gcc "$test_root_path"/back-and-forth.c "${dot_c_files[@]}" -I"$project_root_path"/src -lm -o $abs_program_path > /dev/null 2>&1) || test_error "Compilation failed"
			;;
		2)
			$abs_program_path "$project_root_path" > $perm_tmp_dir/nn-back-and-forth-result.txt 2>&1
			diff $perm_tmp_dir/nn-back-and-forth-result.txt $test_root_path/output.txt > $tmp_dir/diff-output.txt
			if [[ -s $tmp_dir/diff-output.txt ]]; then
				print_warn_text "[I]" " Output file at '$perm_tmp_dir/nn-back-and-forth-result.txt'"
				test_error "Output does not match attended answer: `print_normal $(cat $tmp_dir/diff-output.txt)`"
			fi
			;;
		*)
			;;
	esac
}
