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

# Clean and build vector
pushd ../../utils/
make clean > /dev/null
make > /dev/null
popd

# Clean and build utils
pushd ../../mavlink/utils/
make clean > /dev/null
make > /dev/null
popd

# This is the source directory path.
SRCDIR="../../mavlink/mavlink_source_files"

# Array of message IDs currently in the parser
msgIDs=(0 1 2 4 8 11 20 21 22 23)

# Initial values for pass and fail seed
let passSeed=1
let failSeed=1

# Loop through all message IDs
for id in "${msgIDs[@]}"; do
		# Move into tests directory for the purpose of generating test files
		pushd ../tests
		
		# Clean the current tests out and make the executables
		make clean > /dev/null
		make > /dev/null
		
		# Generate the tests for the given message code
		echo "Generating tests for message ID ${id}"
		./tv ${id} ${passSeed} ${failSeed} > /dev/null

		popd
		
		echo "Testing message ID ${id}"

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

		# Find the values of pass and fail seed
		let passSeed=($(ls ../tests/pass.* 2>/dev/null | awk -F. '{print $NF}' | sort -n | tail -1) + 1)
		let failSeed=($(ls ../tests/fail.* 2>/dev/null | awk -F. '{print $NF}' | sort -n | tail -1) + 1)
done

pushd ../../mavlink/utils/
make clean > /dev/null
popd

pushd ../../utils/
make clean > /dev/null
popd

pushd ../tests/
make clean > /dev/null
popd

pushd ../tests.src/
./tv.sh
popd

