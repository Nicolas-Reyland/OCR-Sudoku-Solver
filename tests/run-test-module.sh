#!/bin/bash

# - Setup test environment -
# Load utils functions
SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
. $SCRIPT_DIR/.test_env_utils
# Setup script env behaviour
set +e
trap "exit 1" TERM
export TOP_PID=$$
# Setup variables
export TEST_ENV_VAR=1
export EXIT_ON_WARNING=false
export EXIT_ON_ERROR=true
export EXIT_ON_PROBLEM=true
export TEST_NAME=$1
# Setup the Test Environment
export TEST_STATUS=3
## Check args
if [ $# -ne 3 ]; then
	echo Usage: ./runtest.sh test-name test-root-path project-root-path
	test_problem "Missing or too many arguments : $# != 3"
fi
TEST_STATUS=0
## Check test root path
export test_root_path=$2
if [ ! -d $test_root_path ]; then
	test_problem "Test root path (\$2 : $2) is not a directory (from `pwd`)"
	exit 1
fi
## Check project root path
export project_root_path=$3
if [ ! -d $project_root_path ]; then
	test_problem "Project root path (\$3 : $3) is not a directory (from `pwd`)"
	exit 1
fi
## Add tmp dir path
export tmp_dir="/tmp/ocr-tests"
export perm_tmp_dir="/tmp/ocr-tests-perm"
mkdir -p $tmp_dir $perm_tmp_dir

# - Running the test -
# Source the file
. $test_root_path/test-descr.bash
export TOTAL_STEPS=${#TEST_STEPS_DESCR[@]}

## Prepare the steps
fn_exists prepare_steps && prepare_steps

## Execute all the steps
#TEST_OUTPUT_INDENT_LVL=1
for step_index in ${!TEST_STEPS_DESCR[@]}
do
	num_sub_steps=${TEST_STEPS_DESCR[$step_index]}
	# once the step_index is used as an index, we can increment it
	((step_index++))
	announce_step_start $step_index
	# ssc : sub step counter
	((TEST_OUTPUT_INDENT_LVL++))
	for ((ssc=1;ssc<=$num_sub_steps;ssc++))
	do
		announce_sub_step_start $step_index $ssc
		eval "step_$step_index $ssc"
	done
	((TEST_OUTPUT_INDENT_LVL--))
	announce_step_end $step_index
done

# clean test env
rm -rf $tmp_dir

# call end of test
test_finished
exit 0

#
