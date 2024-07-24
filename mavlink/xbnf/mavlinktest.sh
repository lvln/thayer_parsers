#!/bin/bash

pushd () {
    command pushd "$@" > /dev/null
}

popd () {
    command popd "$@" > /dev/null
}

# Check number of arguments
if [ $# != 1 ] && [ $# != 0 ]; then
		echo "usage: mavlinktest.sh [-v]"
		exit
fi

# Check arguments
if [ $# == 1 ] && [ $1 != "-v" ]; then
		echo "usage: mavlinktest.sh [-v]"
		exit
fi

# Create array of message IDs currently in the parser
msgIDs=(0 1 4 8 22 24 29 30 31 32 33 42 46 65 74 77 83 85 87 109 141 147 230 241 242 245 253 340 380 410 411 12901 12904)

# Loop through all message IDs
for id in "${msgIDs[@]}"; do
		# Make the tests for the given message code
		pushd ../tests
		make clean > /dev/null
		make > /dev/null
		./tv ${id}
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

		echo "Message ID ${id} complete"
done

pushd ../tests
make clean > /dev/null
popd
