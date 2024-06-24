#!/bin/bash
pushd () {
    command pushd "$@" > /dev/null
}

popd () {
    command popd "$@" > /dev/null
}

# by default mode be silent on compiles
if [ $# == 1 ] && [ $1 == "-h" ]; then
		echo "usage: runall.sh [-h | -O0 | -O1 | -O2 ]"
		exit
fi

TODAY=`date +"%m%d%Y"`

rm -f ./RESULTS/run.${TODAY}
touch ./RESULTS/run.${TODAY}

# 34 runs -- first 5 are hmr and bison, rest are hmr only
for DIR in gmr0 gmr1 jnum jstring json bin0 bin1 bin2 unicode command response json.unicode url usps can can/can.pr J1939 http xml mavlink
do
		echo "[${DIR}]"
		./run.sh ${DIR} >> ./RESULTS/run.${TODAY}
done
