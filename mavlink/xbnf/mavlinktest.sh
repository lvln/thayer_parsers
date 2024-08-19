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

# This is the source directory path.
SRCDIR="../../mavlink/mavlink_source_files"

# Array of message IDs currently in the parser
msgIDs=(0 1 2 4 8 11 20 21 22 23 24 29 30 31 32 33 40 42 43 44 45 46 47 49 51 65 69 73 74 76 77 83 85 87 109 132 141 147 148 230 241 242 245 253 300 340 380 397 410 411 12901 12904)

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

pushd ../tests/
make clean > /dev/null
cp ../tests.src/pass.* ../tests.src/fail.* ./ > /dev/null
popd
