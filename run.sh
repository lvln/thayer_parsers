#!/bin/bash
#
# usage: run.sh <dir> [-s -O1]
#   -d  : display compilations during make
#   -Ox : apply optimization x=0,1,2
#
pushd () {
    command pushd "$@" > /dev/null
}

popd () {
    command popd "$@" > /dev/null
}

if [ $# == 1 ] && [ $1 == "-h" ]; then
		echo "usage: run.sh <parser> [-d]"
		echo "  -d -- display compilation (default=quiet)"
		exit
fi

DIR=$1
if [ ! -d ${DIR} ]; then
		echo "[no such directory: ${DIR}]"
		exit
fi

# by default mode be silent on compiles
MODE=-s
if [ $# == 2 ] && [ $2 == "-d" ]; then
		unset MODE
fi

if [ -d ${DIR}/bison ]; then
		pushd ./${DIR}/bison
		echo "[bison: ${DIR}/bison]"
		make ${MODE} run
		popd
		echo "====="
fi

if [ -d ${DIR}/hmr ]; then
		pushd ./${DIR}/hmr
		echo "[hmr: ${DIR}/hmr]"
		make ${MODE} run
		popd
		echo "====="
fi

if [ -d ${DIR}/hdcode ]; then
		pushd ./${DIR}/hdcode
		echo "[hdcode: ${DIR}/hdcode]"
		make ${MODE} run
		popd
		echo "====="
fi
