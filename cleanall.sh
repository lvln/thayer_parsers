#!/bin/bash

pushd () {
    command pushd "$@" > /dev/null
}

popd () {
    command popd "$@" > /dev/null
}

if [ $# == 1 ] && [ $1 == "-h" ]; then
		echo "usage: makeall.sh [-h | -s]"
		echo "   -h : help"
		echo "   -s : *show* compilation"
		exit
fi

# quiet by default
MODE=-s
if [ $# == 1 ] && [ $1 == "-s" ]; then
		unset MODE
fi

# make the ppxl library
pushd ./ppxml
echo [ppxml]
make ${MODE} clean
popd

pushd ./xbnf/bison
echo [xbnf]
make ${MODE} clean
popd

# make the hmr grammars
for DIR in gmr0 gmr1 jnum jstring json bin0 bin1 bin2 unicode command response json.unicode abnf url usps can can/can.pr J1939 http
do
		if [ -d ./${DIR}/bison ] ; then
				pushd ./${DIR}/bison
				echo [$DIR/bison]
				make ${MODE} clean
				popd
		fi

		if [ -d ./${DIR}/hmr ] ; then
				pushd ./${DIR}/hmr
				echo [$DIR/hmr]
				make ${MODE} clean
				popd
		fi

		if [ -d ./${DIR}/xbnf ] ; then
				pushd ./${DIR}/xbnf
				echo [$DIR/xbnf]
				make ${MODE} clean
				popd
		fi

done

