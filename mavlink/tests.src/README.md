# Tests for MAVLink parser

## Contents:

* Test files `pass.1` - `pass.n` and `fail.1` - `fail.n` test all message types contained in the parser
* `tv.sh` generates passsing and failing tests for the parser and deposits them in `../tests/`
* `tv.c` is used by `tv.sh` to generate failing tests for the parser
* `tvshort.c` is used by `tv.sh` to generate tests for the parser for message types not obtained in actual drone flights
