#!/bin/bash
# Test file for nn/alloc-model

# Testing the allocation and freeing of the nn_Model struct
# Steps :
# 1. compile a C src file which allocates, then frees a nn_Model struct
# 2. running the compiled program
# Sub-steps :
# 1.
#  - compilation
#  - compilation with warning
# 2.
#  - program exits with a status of 1
#  - program has no memory leaks (using valgrind)

# Test Description
export TEST_STEPS_DESCR=(2 2)

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
	abs_program_path="/tmp/nn-alloc-test-test.out"
	if [ -f $abs_program_path ]; then
		rm $abs_program_path
	fi
	if [[ -f $abs_program_path.2 ]]; then
		rm $abs_program_path.2
	fi
	if [[ -f $abs_program_path.3 ]]; then
		rm $abs_program_path.3
	fi
	# Check valgrind installation
	if ! command -v valgrind &> /dev/null; then
		test_problem "You must install 'valgrind' for this step to be completed (debian: sudo apt install valgrind -y)"
	fi
}

# Step 1
function step_1 {
	prefix_tabs=$(_prefix_indent)
	alias sed-stdout='sed -e "s/^/$prefix_tabs | /;"'
	alias compile='gcc '"$test_root_path"'/test.c '"${dot_c_files[@]}"' -I'"$project_root_path"'/src -lm'
	case $1 in
		1)
			# Compile without warnings
			nowarn_compilation=$(gcc "$test_root_path"/test.c "${dot_c_files[@]}" -I"$project_root_path"/src -lm -o $abs_program_path > /dev/null 2>&1) || test_error "Compilation failed"
			;;
		2)
			fullwarn_compilation=$(gcc "$test_root_path"/test.c "${dot_c_files[@]}" -I"$project_root_path"/src -lm -o $abs_program_path.2 -Wall -Wextra > /dev/null 2>&1 | sed -e "s/^/$prefix_tabs | /;")
			if [[ -n "$fullwarn_compilation" ]]; then
				test_warn "Output in compiling file without flags :"
				echo $nowarn_compilation
			fi
			werror_compilation=$(gcc "$test_root_path"/test.c "${dot_c_files[@]}" -I"$project_root_path"/src -lm -o $abs_program_path.3 -Wall -Wextra -Werror > /dev/null 2>&1 | sed -e "s/^/$prefix_tabs | /;") || test_warn "Could not compile with ""-Werror"" flag"
			;;
		*)
			;;
	esac
}

# Step 2
function step_2 {
	case $1 in
		1)
			# Check program return status
			prefix_tabs=$(_prefix_indent)
			$abs_program_path | sed -e "s/^/$prefix_tabs | /;" || test_error "Program exits with non-zero status. Binaries are at ""$abs_program_path"""
			;;
		2)
			# Valgrind memory check
			prefix_tabs=$(_prefix_indent)
			valgrind --tool=memcheck --quiet $abs_program_path q 2> /tmp/valgrind-output
			if [ -n "`cat /tmp/valgrind-output`" ]; then
				test_warn "Valgrind is not happy with your program:"
				cat /tmp/valgrind-output | sed -e "s/^/$prefix_tabs | /;"
			fi
			;;
		*)
			;;
	esac
}
