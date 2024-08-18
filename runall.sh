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

# All grammars not listed here are experimental and unused
# NOTE: DO NOT PUT xbnf or abnf in this list
for DIR in gmr0 gmr1 usps bin0 bin1 bin2 bin3 command response \
				 jnum jstring json junicode json.unicode \
				 mavlink_standalone_message mavlink_single_message_repeat mavlink_three_messages mavlink \
				 J1939 url http
do
		echo "[${DIR}]"
		./run.sh ${DIR} >> ./RESULTS/run.${TODAY}
done
