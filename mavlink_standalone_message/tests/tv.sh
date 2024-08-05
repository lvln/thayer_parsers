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

# Clean the current tests out and make the executables
make clean > /dev/null
make > /dev/null

if [ $# == 1 ]; then
		# Generate tests for SCALED_PRESSURE message
		./tv 29
fi

# Start a counter for the message number
let msgNum=1

if [ -e ${SRCDIR}/run1.pcap ]; then
		pushd ${SRCDIR}
		make clean > /dev/null
		make > /dev/null
		
		# Extract all SCALED_PRESURE messages and store them in a temporary file
		./extractbymessageid run1.pcap temp.mav 29 > /dev/null

		# Count messages in the temporary file
		./countmessages ./temp.mav > foo
		MUMMESS=$(cat foo)
		rm foo > /dev/null

		# Create a test in a unique file for each message
		for (( i = 1; i <= NUMMESS; i++ )); do
				./extractbymessagenumber ./temp.mav pass.${msgNum} ${i} > /dev/null
				let msgNum++
		done

		# Remove temporary files and clean directory
		make clean > /dev/null
		rm temp.mav > /dev/null

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

		# Remove temporary files and clean directory
		make clean > /dev/null
		rm temp.mav > /dev/null
		popd
fi

# Move passing tests to tests directory
mv ${SRCDIR}/pass.* . > /dev/null

# Create an empty file
touch fail.1

# Bring over an EVENT message and place it in a failing test file
if [ -e ${SRCDIR}/run1.pcap ]; then
		pushd ${SRCDIR}
		make clean > /dev/null
		make > /dev/null
		
		# Extract EVENT message
		./extractbymessagenumber run1.pcap fail.2 65 > /dev/null

		# Move message over
		mv fail.2 ../../mavlink_standalone_message/tests/ > /dev/null

		# Clean out directory
		make clean > /dev/null
		popd
fi

# Create fialing test file with more than 1 SCALED_PRESSURE message
if [ -e ${SRCDIR}/run1.pcap ]; then
		pushd ${SRCDIR}
		make clean > /dev/null
		make > /dev/null
		
		# Extract EVENT message
		./extractbymessageid run1.pcap fail.3 29 > /dev/null

		# Move messages over
		mv fail.3 ../../mavlink_standalone_message/tests/ > /dev/null

		# Clean out directory
		make clean > /dev/null
		popd
fi

# Generate failing test cases
./tvshort > /dev/null
