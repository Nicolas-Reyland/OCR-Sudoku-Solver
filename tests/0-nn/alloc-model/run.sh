#!/bin/bash
# Test file for nn/conv

# Testing the allocation and freeing of the nn_Model struct
# Steps :
# 1. compile a C src file which allocates, then frees a nn_Model struct
# 2. running the compiled program
# Checks :
# 1.
#  - compilation without errors
#  - compilation with warning
# 2.
#  - program exits with a status of 1
#  - program has no memory leaks (using valgrind)


# Step 1
export abs_program_path="/tmp/nn-alloc-test-test.out"
if [ -f $abs_program_path ]; then
	rm $abs_program_path
fi
if [[ -f $abs_program_path.2 ]]; then
	rm $abs_program_path.2
fi
if [[ -f $abs_program_path.3 ]]; then
	rm $abs_program_path.3
fi

export dot_c_files=()
cd "$project_root_path"
for file in $(find src/{nn,utils} -type f -name "*.c")
do
	dot_c_files+=("$file")
done
nowarn_compilation=$(gcc -o $abs_program_path "$test_root_path"/test.c "${dot_c_files[@]}" -Isrc -lm 2>&1) || test_error "Compilation failed"
fullwarn_compilation=$(gcc -o $abs_program_path.2 "$test_root_path"/test.c "${dot_c_files[@]}" -Isrc -Wall -Wextra -lm 2>&1)
werror_compilation=$(gcc -o $abs_program_path.3 "$test_root_path"/test.c "${dot_c_files[@]}" -Isrc -Wall -Wextra -Werror -lm 2>&1) || test_warn "Could not compile with ""-Werror"" flag"
## check for compilation result
# if [[ ! -f $abs_program_path ]]; then
# 	test_error "Compilation failed"
# fi

if [[ -n "$fullwarn_compilation" ]]; then
	test_warn "Output in compiling file without flags :"
	echo $nowarn_compilation
fi
# if [[ ! -f $abs_program_path.3 ]]; then
# 	test_warn "Could not compile with -Werror :"
# 	echo $werror_compilation
# fi

# Step 2

## Check program return status
$abs_program_path > /dev/null 2>&1 || test_error "Program exits with non-zero status. Binaries are at ""$abs_program_path"""

## Valgrind memory check
if ! command -v valgrind &> /dev/null; then
	test_problem "You must install 'valgrind' for this step to be completed"
fi

export valgrind_memcheck_output=`valgrind --tool=memcheck --quiet $abs_program_path`

if [ -n $valgrind_memcheck_output ]; then
	test_warn "Valgrind is not happy with your program: `$valgrind_memcheck_output`"
fi



# End the test
test_finished

#
