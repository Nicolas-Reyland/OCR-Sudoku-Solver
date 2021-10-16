#!/bin/bash
# Test file for ...

# Testing ... what you are testing ...
# Steps :
# 1. test output
# Sub-steps :
# 1.
#  - ...
#  - ...

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
	abs_program_path="$tmp_dir/nn-feed-test-test.out"
	if [ -f $abs_program_path ]; then
		rm -f $abs_program_path
	fi
}

# Step 1
function step_1 {
	case $1 in
		1)
			$(gcc "$test_root_path"/test.c "${dot_c_files[@]}" -I"$project_root_path"/src -lm -o $abs_program_path > /dev/null 2>&1) || test_error "Compilation failed"
			;;
		2)
			$abs_program_path > $tmp_dir/nn-feeding-test-result.txt
			diff $tmp_dir/nn-feeding-test-result.txt $test_root_path/output.txt > $tmp_dir/diff-output.txt
			if [[ -s $tmp_dir/diff-output.txt ]]; then
				test_error "Output does not match attended answer: `cat $tmp_dir/diff-output.txt`"
			fi
			;;
		*)
			;;
	esac
}