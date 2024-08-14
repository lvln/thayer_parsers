#!/bin/bash

pushd () {
    command pushd "$@" > /dev/null
}

popd () {
    command popd "$@" > /dev/null
}

# Check number of arguments
if [ $# != 1 ] && [ $# != 0 ]; then
		echo "usage: mavlinktest.sh [-v|-h]"
		exit
fi

# Check arguments are valid
if [ $# == 1 ] && ( [ $1 != "-v" ] && [ $1 != "-h" ] ); then
		echo "usage: mavlinktest.sh [-v|-h]"
		exit
fi

# Print out a help statement if -h flag was used.
if [ $# == 1 ] && [ $1 == "-h" ]; then
		echo "usage: mavlinktest.sh [-v|-h]"
		echo "[-v] flag prints out results of passing and failing tests; absence only prints out results of failing tests"
		exit
fi

# This is the source directory path
SRCDIR="../../mavlink/mavlink_source_files"

# Move into tests direcotry for the purpose of generating tests
pushd ../tests

# Clean the current tests out and make the executables
make clean > /dev/null
make > /dev/null

# Generate tests for SCALED_PRESSURE message
./tv 29 1 1

# Return to the xbnf directory
popd

# Run the passing tests
shopt -s nullglob
for f in ../tests/pass.*; do
		# Run the parser on each passing test
		CMD="./gmr ${f}"
		{ ${CMD} >& /dev/null ; } >& /dev/null
		
		# Get the result from running the parser
		RESVAL=$?
		
		# Print only when tests fail if not in verbose mode
		if [ ${RESVAL} == 0 ]; then
				if [ $# == 1 ]; then
						echo -e "[PASS: ${CMD}]"
				fi
		else
				echo -e "===>>> [FAIL: ${CMD}]"
		fi
done

# Run the failing tests
shopt -s nullglob
for f in ../tests/fail.*; do
		# Run the parser on each passing test
		CMD="./gmr ${f}"
		{ ${CMD} >& /dev/null ; } >& /dev/null
		
		# Get the result from running the parser
		RESVAL=$?
		
		# Print only when tests fail if not in verbose mode
		if [ ${RESVAL} != 0 ]; then
				if [ $# == 1 ]; then
						echo -e "[PASS: ${CMD}]"
				fi
		else
				echo -e "===>>> [FAIL: ${CMD}]"
		fi
done

# Clean out the tests and executables from tests directory
pushd ../tests/
make clean > /dev/null
cp ../tests.src/pass.* ../tests.src/fail.* . &> /dev/null
popd
