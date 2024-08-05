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

# This is the source directory path
SRCDIR="../../mavlink/mavlink_source_files"

# Move into tests direcotry for the purpose of generating tests
pushd ../tests

# Clean the current tests out and make the executables
make clean > /dev/null
make > /dev/null

# Generate tests for SCALED_PRESSURE message
./tv 29

# Start counter for the individual message file names
let msgNum=4

if [ -e ${SRCDIR}/run1.pcap ]; then
		pushd ${SRCDIR}
		make clean > /dev/null
		make > /dev/null
		
		# Extract all SCALED_PRESURE messages and store them in a temporary file
		./extractbymessageid run1.pcap temp.mav 29 > /dev/null

		# Count messages in the temporary file
		./countmessages ./temp.mav > foo
		NUMMESS=$(cat foo)
		rm foo > /dev/null

		# Create a test in a unique file for each message
		for (( i = 1; i <= NUMMESS; i++ )); do
				./extractbymessagenumber ./temp.mav pass.${msgNum} ${i} > /dev/null
				let msgNum++
		done

		# Clean executables fron source directory
		make clean > /dev/null

		# Copy over all SCALED_PRESSURE messages from run1.pcap
		mv temp.mav ../../mavlink_single_message_repeat/tests/pass.1 > /dev/null
		popd
fi

if [ -e ${SRCDIR}/run2.pcap ]; then
		pushd ${SRCDIR}
		make clean > /dev/null
		make > /dev/null
		
		# Extract all SCALED_PRESURE messages and store them in a temporary file
		./extractbymessageid run2.pcap temp.mav 29 > /dev/null
		
		# Count messages in the temporary file
		./countmessages ./temp.mav > foo
		NUMMESS=$(cat foo)
		rm foo > /dev/null

		# Create a test in a unique file for each message
		for (( i = 1; i <= NUMMESS; i++ )); do
				./extractbymessagenumber ./temp.mav pass.${msgNum} ${i} > /dev/null
				let msgNum++
		done

		# Remove executables from source files directiory
		make clean > /dev/null

		# Copy over all SCALED_PRESSURE messages from run2.pcap
		mv temp.mav ../../mavlink_single_message_repeat/tests/pass.2 > /dev/null
		popd
fi

# Move passing tests to tests directory
mv ${SRCDIR}/pass.* . > /dev/null

# Create an empty file
touch pass.3

# Bring over an EVENT message and place it in a failing test file
if [ -e ${SRCDIR}/run1.pcap ]; then
		pushd ${SRCDIR}
		make clean > /dev/null
		make > /dev/null
		
		# Extract an EVENT message
		./extractbymessagenumber run1.pcap fail.1 65 > /dev/null
		
		# Move message over to tests directory
		mv fail.1 ../../mavlink_single_message_repeat/tests/ > /dev/null

		# Clean source files directory
		make clean > /dev/null
		popd
fi

# Bring over a SCALED_PRESSURE message followed by an EVENT message
if [ -e ${SRCDIR}/run1.pcap ]; then
		pushd ${SRCDIR}
		make clean > /dev/null
		make > /dev/null
		
		# Extract aSCALED_PRESSURE message followed by an EVENT message and store it in a temporary file
		./extractbymessagenumber run1.pcap fail.2 33 65 > /dev/null
		
		# Move message over to tests directory
		mv fail.2 ../../mavlink_single_message_repeat/tests/ > /dev/null

		# Clean source files directory
		make clean > /dev/null
		popd
fi

# Generate fialing test cases
./tvshort > /dev/null

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
popd
