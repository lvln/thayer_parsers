#!/bin/bash
# Just cleans all parsers

pushd () {
    command pushd "$@" > /dev/null
}

popd () {
    command popd "$@" > /dev/null
}

if [ $# == 1 ] && [ $1 == "-h" ]; then
		echo "usage: cleanall.sh [-h | -s]"
		echo "   -h : help"
		echo "   -s : *show* compilation"
		exit
fi

# quiet by default
MODE=-s
if [ $# == 1 ] && [ $1 == "-s" ]; then
		unset MODE
fi

# clean the ppxl library
pushd ./ppxml
echo [ppxml]
make ${MODE} clean
popd

# clean the xbnf pre-processor
pushd ./xbnf/bison
echo [xbnf]
make ${MODE} clean
popd

# make the abnf pre-processor -- not yet working
#pushd ./abnf/bison
#echo [abnf]
#make ${MODE} clean
#popd

# All grammars not listed here are experimental and unused
# NOTE: DO NOT PUT xbnf or abnf in this list
for DIR in gmr0 gmr1 usps bin0 bin1 bin2 command response fwi fwi_range \
				 jnum jstring json junicode json.unicode \
				 mavlink_1_msg mavlink_3_msgs mavlink_10_msgs mavlink_20_msgs mavlink_30_msgs mavlink_40_msgs mavlink \
				 spp \
				 J1939 url http
do
		if [ -d ./${DIR}/bison ] ; then
				pushd ./${DIR}/bison
				echo [$DIR/bison]
				rm -f Makefile
				ln -s ../../Makefile.bison ./Makefile
				make ${MODE} clean
				popd
		fi

		if [ -d ./${DIR}/hmr ] ; then
				pushd ./${DIR}/hmr
				echo [$DIR/hmr]
				rm -f Makefile
				ln -s ../../Makefile.hmr ./Makefile				
				make ${MODE} clean
				popd
		fi

		if [ -d ./${DIR}/xbnf ] ; then
				pushd ./${DIR}/xbnf
				echo [$DIR/xbnf]
				rm -f Makefile
				ln -s ../../Makefile.xbnf ./Makefile
				make ${MODE} clean
				popd
		fi
done

