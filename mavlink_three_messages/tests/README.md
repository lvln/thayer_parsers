# Tests for MAVLink parser

## Contents:

* Test files `pass.1` - `pass.n` and `fail.1` - `fail.n` are imported from `../tests.src/` and thoroughly test all message types in the parser
* `tv.sh` imports passing and failing tests from `../tests.src/`
* `tv.c` is used by `../xbnk/mavlinktest.sh` to fuzz test the parser
