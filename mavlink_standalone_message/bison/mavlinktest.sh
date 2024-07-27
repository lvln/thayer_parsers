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

pushd ../tests/

# Clean the current tests out and make the executables
make clean > /dev/null
make > /dev/null

# Generate tests for SCALED_PRESSURE message
./tv 29

if [ -e ${SRCDIR}/run1.pcap ]; then
		pushd ${SRCDIR}
		make clean > /dev/null
		make > /dev/null
		
		# Extract all SCALED_PRESURE messages and store them in a temporary file
		CMD="./extractbymessageid run1.pcap temp.mav 29"
		{ ${CMD} >& /dev/null ; } >& /dev/null

		# Count messages in the temporary file
		CMD="./countmessages ./temp.mav"

		# Save the number of messages to a temporary file
		${CMD} > foo
		numMess=$(cat foo)
		rm foo > /dev/null

		# Create a test in a unique file for each message
		for (( i = 1; i <= numMess; i++ )); do
				CMD="./extractbymessagenumber ./temp.mav pass.${msgNum} ${i}"
				{ ${CMD} >& /dev/null ; } >& /dev/null
				let msgNum++
		done
		make clean > /dev/null
		rm temp.mav > /dev/null
		popd
fi

if [ -e ${SRCDIR}/run2.pcap ]; then
		pushd ${SRCDIR}
		make clean > /dev/null
		make > /dev/null
		
		# Extract all SCALED_PRESURE messages and store them in a temporary file
		CMD="./extractbymessageid run2.pcap temp.mav 29"
		{ ${CMD} >& /dev/null ; } >& /dev/null

		# Count messages in the temporary file
		CMD="./countmessages ./temp.mav"

		# Save the number of messages to a temporary file
		${CMD} > foo
		numMess=$(cat foo)
		rm foo > /dev/null

		# Create a test in a unique file for each message
		for (( i = 1; i <= numMess; i++ )); do
				CMD="./extractbymessagenumber ./temp.mav pass.${msgNum} ${i}"
				{ ${CMD} >& /dev/null ; } >& /dev/null
				let msgNum++
		done
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
		
		# Extract all SCALED_PRESURE messages and store them in a temporary file
		CMD="./extractbymessagenumber run1.pcap fail.2 65"
		{ ${CMD} >& /dev/null ; } >& /dev/null

		# Move message over
		mv fail.2 ../../mavlink_standalone_message/tests/ > /dev/null

		make clean > /dev/null
		popd
fi

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

pushd ../tests/
make clean > /dev/null
popd
