#!/bin/bash

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
	test_problem "[?] Missing or too many arguments"
fi
TEST_STATUS=0
## Check test root path
export test_root_path=$2
if [ ! -d $test_root_path ]; then
	echo "[?] Test root path (\$2 : $2) is not a directory (from `pwd`)"
	exit 1
fi
## Check project root path
export project_root_path=$3
if [ ! -d $project_root_path ]; then
	echo "[?] Project root path (\$3 : $3) is not a directory (from `pwd`)"
	exit 1
fi

. $test_root_path/run.sh
