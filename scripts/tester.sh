#!/bin/bash
# tester.sh expected-result command
if [ $# != 2 ] ; then
		echo "usage: tester.sh [ pass | fail ] <command>"
		echo $#
		exit -1
fi
EXPECT=$1
CMD=$2
# run the command throwing away any output from the command and the shell
{ ${CMD} >& /dev/null ; } >& /dev/null
# get the result from the last command
RESVAL=$?
# decide what to print based on the result
if [ ${RESVAL} == 139 ] ; then
			 echo -e "[FLT : ${CMD}]"
elif [ ${RESVAL} == 0 ] ; then
		if [ ${EXPECT} == "fail" ] ; then
				echo -e "===>>> [FAIL: ${CMD}]"
		else
				echo -e "[PASS: ${CMD}]"
		fi
else
		if [ ${EXPECT} == "pass" ] ; then
				echo -e "===>>> [FAIL: ${CMD}]"
		else
				echo -e "[PASS: ${CMD}]"
		fi
fi
