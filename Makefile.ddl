#
# THE STANDARD DAEDALUS MAKEFILE -- DO NOT CHANGE OR MOVE THIS FILE
# This file is used to run all DAEDALUS parsers
#

run: spec.ddl
	ddltest.sh

clean:
	rm -f *~






