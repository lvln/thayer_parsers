# Tests for Space Packet Protocol parser

## Contents:

* Test files `pass.1` - `pass.n` and `fail.1` - `fail.n` thoroughly test the parser
* `tv.sh` generates passsing and failing tests for the parser and deposits them in `../tests/`
* `tv.c` is used by `tv.sh` to generate failing tests for the parser
* `converttobyte.c` converts test files with bit field headers to byte field headers prior to depositing in `../tests/`
