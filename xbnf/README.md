

# xbnf -- A preprocessor, written in Bison, that extends Bison BNF.

**xbnf** is a Bison parser that extends Bison's BNF to include
notations for parsing ascii and binary formats. The following notations are provided:

__'\x00'__ -- The terminal symbol for zero in hexadecimal.

__[t1-t2]__ -- A range of values between terminal t1 and t2, where t2>t1.

__[t1,t2,...,tn]__ -- An enumerated set of terminal values t1 to tn, where n>1.

__"\<string\>"__ -- A string value.

__\*__ -- Signifying any byte '\x00' to '\xFF'.


## Usage: xbnf \<infile\> [ -h -d -o \<outfile\>]

Parses the input grammer in _\<infile\>_ and produces a Bison grammar in
_\<outfile\>_ (default: _output.y_). The output of xbnf is a Bison grammar
that can be fed to Bison directly. Flags: -h provides usage, -d passed on to bison.


## Examples


__P : [ '\x05' - '\x1c' ] ;__ -- accepts any binary value between
  _x05_ and _x1c_.

__P : ['a'-'f'] ;__ -- accepts the characters between _a_ and  _f_.

__P : "true" | "false" | "null" ;__ -- accepts the string _true_, or _false_, or _null_.

__P : ['\x1f', '\x00', 'a', ']', '\0xfe'] ;__ -- accepts the hexadecimal
  values _x1f_, _x00_, _xfe_ and the characters _a_ and _]_.

__P : ['a' - 'f'] * '\x00' ;__ -- accepts a sequence: any value in the range
  _a_ and _f_, followed by anybyte (_x00_ to _xff_), followed by a
  zero byte (_x00_).

Note that whitespace is allowed in any range or enumeration.

