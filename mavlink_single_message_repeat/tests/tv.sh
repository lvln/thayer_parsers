#!/bin/bash

pushd () {
    command pushd "$@" > /dev/null
}

popd () {
    command popd "$@" > /dev/null
}

# Check number of arguments
if [ $# != 0 ] && [ $# != 1 ] ; then
		echo "usage: tv.sh [-l]"
		exit
fi

# Check that second argument is -l
if [ $# == '1' ]; then
		if [ $1 != '-l']; then
				echo "usage: tv.sh [-l]"
				exit
		fi
fi

# This is the source directory path
SRCDIR="../../mavlink/mavlink_source_files"

# Clean the current tests out and build the executables
make clean > /dev/null
make > /dev/null

if [ $# == 1 ]; then
		# Generate tests for SCALED_PRESSURE message
		./tv 29
fi

# Start a counter for the message number pass files
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

		# Clean out executables in source files directory
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

		# Clean out executables in source files directory
		make clean > /dev/null

		# Copy over all SCALED_PRESSURE messages from run2.pcap
		mv temp.mav ../../mavlink_single_message_repeat/tests/pass.2 > /dev/null
		popd
fi

# Move passing tests to tests directory
mv ${SRCDIR}/pass.* . > /dev/null

# Create an empty file
touch pass.3 > /dev/null

# Bring over an EVENT message and place it in a failing test file
if [ -e ${SRCDIR}/run1.pcap ]; then
		pushd ${SRCDIR}
		make clean > /dev/null
		make > /dev/null
		
		# Extract EVENT message
		./extractbymessagenumber run1.pcap fail.1 65 > /dev/null

		# Move message over to tests directory
		mv fail.1 ../../mavlink_single_message_repeat/tests/ > /dev/null

		# Clean executables in source directory
		make clean > /dev/null
		popd
fi

# Bring over a SCALED_PRESSURE message followed by an EVENT message
if [ -e ${SRCDIR}/run1.pcap ]; then
		pushd ${SRCDIR}
		make clean > /dev/null
		make > /dev/null
		
		# Extract a single SCALED_PRESSURE message and follow it by an EVENT message
		./extractbymessagenumber run1.pcap fail.2 33 65 > /dev/null

		# Move messages over to tests directory
		mv fail.2 ../../mavlink_single_message_repeat/tests/ > /dev/null

		# Clean executables in source directory
		make clean > /dev/null
		popd
fi

# Generate fialing test cases
./tvshort > /dev/null
