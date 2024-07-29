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

# Check arguments are valid
if [ $# == 1 ] && [ $1 != "-v" ]; then
		echo "usage: mavlinktest.sh [-v]"
		exit
fi

# This is the source directory path.
SRCDIR="../mavlink_source_files"

# Create array of message IDs currently in the parser
msgIDs=(0 1 4 8 22 24 29 30 31 32 33 42 46 65 74 77 83 85 87 109 141 147 230 241 242 245 253 340 380 410 411 12901 12904)

# Loop through all message IDs
for id in "${msgIDs[@]}"; do
		# Make the tests for the given message code after removing all previous tests
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

		echo "Message ID ${id} tests complete"
done

# Clean out the tests directory.
pushd ../tests
make clean > /dev/null
popd

# Create a test file for full flights with the drone with the drone.
if [ -e ${SRCDIR}/run1.pcap ]; then
		pushd ${SRCDIR}
		make clean > /dev/null
		make > /dev/null

		# Extract the MAVLink portion of the messages from run1
		./makemav ./run1.pcap ./pass.10000 > /dev/null

		# Move test file to tests directory
		mv ./pass.10000 ../tests > /dev/null

		make clean > /dev/null
		popd

		# Run the parser on the test file
		CMD="./gmr ../tests/pass.10000"
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
fi

# Create a test file for full flights with the drone with the drone.
if [ -e ${SRCDIR}/run2.pcap ]; then
		pushd ${SRCDIR}
		make clean > /dev/null
		make > /dev/null
		
		# Extract the MAVLink portion of the messages from run1
		./makemav ./run2.pcap ./pass.10001 > /dev/null

		# Move test file to tests directory
		mv ./pass.10001 ../tests > /dev/null

		make clean > /dev/null
		popd

		# Run the parser on the test file
		CMD="./gmr ../tests/pass.10001"
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
fi

# Clean out all of the tests
pushd ../tests
make clean > /dev/null
popd

# Extract each individual message from a drone flight and place it in its own file
let msgNum=1

if [ -e ${SRCDIR}/run1.pcap ]; then
		pushd ${SRCDIR}
		make clean > /dev/null
		make > /dev/null

		# Count the number of messages in run1
		./countmessages ./run1.pcap > foo
		NUMMESS=$(cat foo)
		rm foo > /dev/null

		# Create a unique passing test file for each message
		for (( i = 1; i <= NUMMESS; i++ )); do
				./extractbymessagenumber ./run1.pcap pass.${msgNum} ${i} > /dev/null
				let msgNum++
		done
		
		make clean > /dev/null
		popd
fi

if [ -e ${SRCDIR}/run2.pcap ]; then
		pushd ${SRCDIR}
		make clean > /dev/null
		make > /dev/null

		# Count the number of messages in run2
		./countmessages ./run2.pcap > foo
		NUMMESS=$(cat foo)
		rm foo > /dev/null

		# Create a unique test file for each message
		for (( i = 1; i <= NUMMESS; i++ )); do
				./extractbymessagenumber ./run2.pcap pass.${msgNum} ${i} > /dev/null
				let msgNum++
		done

		# Clean all executables in the mavlink source directory
		make clean > /dev/null
		popd
fi

# More the newly generated tests to the tests directory
mv ${SRCDIR}/pass.* ../tests/ > /dev/null

# Run the tests on these newly created test files
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

# Clean out all of the test files
pushd ../tests
make clean > /dev/null
popd
