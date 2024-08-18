<img src="logo.png" alt="Thayer Scool of Engineering" width="250"/>

# Parser Experimentation Repository

This repository explores and compares traditional grammars, described
with BNF and implemented through
[Bison](https://www.gnu.org/software/bison), with equivalent grammars
expressed and implemented through
[Hammer](https://gitlab.special-circumstanc.es/hammer/hammer), a
parser combinator framework under development through the
[DARPA SafeDocs](https://www.darpa.mil/program/safe-documents) program. In
addition, an extended version of BNF -- xBNF -- is also explored that
captures core concepts from Hammer. It is implemented as a
preprocessor, written in BNF, implemented with Bison.

All parsers listed here are operational; others represent works in
progress. A set of test vectors is provided for each. In general test
vectors are generated automatically where possible.

The exploration restricts attention based on the following high-value
attributes:

* **Format.** We seek to parse, for the purpose of validation, both
ASCII and binary files and communication protocols.

* **LALR.** All grammars are Look-Ahead, Left-to-Right
(LALR). Consequently, they can parsed with a single stack and need not
involve forking in the associated automaton.

* **XML.** The implementation provides a complete, machine-readable
description of the associated push-down automaton in XML allowing the
state-machines to be compared. This is readily available in Bison; In
Hammer we have compensated by scanning Hammers internal data
structures and generating the XML ourselves. Source code for the
scanner (c.f. **ppxml**) is provided.

* **Flex.** All Bison parsers are constucted without sophisticated
lexical analysis through Flex. Instead they pass binary values directly
to Bison consistent with Hammers byte-by-byte input stream handling.

The repository is structured to allow other emerging **SafeDocs**
technologies to be integrated for comparison and leveraged where the
above attributes are present.


## Acknowledgements ##

This repository is an active collaboration with a number of Dartmouth
students: _Ellie Baker_ on **ppxml**; _Sarah Korb_ on unicode
extensions; _Anthony_ and _Brandon Guzman_ on binary parsers.

We also thank our collaborators at Lockheed-Martin ATL -- Andrew
Holmes, Chris Miller, and Chuck Winters -- for their many valuable
inputs and insights.



## License ##

The software provided in this repository is open source and released
under the standard [MIT software licence](https://mit-license.org)
provided in the file LICENSE-MIT.md.

The file **ppxml.c** contained in the directory **ppxml** is released
without licensing attached by us as it is derived from the
[Hammer](https://gitlab.special-circumstanc.es/hammer/hammer) project
and licenced there under the GPL v2 Licence. A copy of the Hammer GPL
v2 licence is provided in the file LICENCE-GPL in this repository.




## Setup ##

Assuming Hammer is installed in some system location
(e.g. _/usr/local_) and the source is present at some other location
designated  _< hammer >_ : 

* Copy _< hammer >/src/backends/lr.h_ to _/usr/local/include/hammer/backends/_

* Copy _< hammer >/src/cfgrammar.h_ to _/usr/local/include/hammer/_

These files are needed by **ppxml** and are not available by default.

The regression testing scripts, contained in the _scripts_ directory,
must be copied into some directory present in the users PATH
(e.g. _~/bin_).

Add the following entries to your .bashrc file:  
export PATH=.:~/bin:$PATH  
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH


## Regression Testing:

&emsp;**./cleanall.sh** -- clean out all the parsers  
&emsp;**./makeall.sh** -- build all the parsers  
&emsp;**./runall.sh** -- run all the parsers (results in RESULTS directory)  
&emsp;**./run.sh <parser>** -- run one parser (e.g. run.sh jnum)  


## Parser Directories:

#### <ins>Simple Examples</ins>

&emsp;**gmr0** -- trivial 5-rule ASCII grammar (subset of gmr1)  
&emsp;**gmr1** -- 9-rule ASCII grammar described by Satya Kiran Popuri [_here_](https://www.cs.uic.edu/~spopuri/cparser.html)  
&emsp;**usps** -- simple ASCII grammar for US Mail addresses  
&emsp;**bin0** -- trivial binary test -- a sequence of uint8 values 0,34,93  
&emsp;**bin1** -- a sequence containing every binary unint8 value 0 to 255  
&emsp;**bin2** -- parsing any uint8 byte as a wildcard  
&emsp;**command** -- simple binary command parser with wildcards  
&emsp;**response** -- simple binary response parser with wildcards  

#### <ins>JSON parser tutorial</ins> -- a sequence of parsers that incrementally build to a full [_JSON_](http://www.json.org) grammar

&emsp;**jnum** -- json number parser  
&emsp;**jstring** -- json string parser  
&emsp;**junicode** -- binary unicode parser  
&emsp;**json** -- json parser with everything but unicode  
&emsp;**json.unicode** -- binary json with unicode parser  

#### <ins>MAVLINK parser tutorial</ins> -- a sequence of parsers that incrementally build toward a MAVLINK parser

&emsp;**mavlink_standalone_message** -- MAVLink single message parser for one message type  
&emsp;**mavlink_single_message_repeat** -- MAVLink parser for zero or more messages of one message type  
&emsp;**mavlink_three_messages** -- MAVLink parser for three dirrerent message types  
&emsp;**mavlink** -- MAVLink parser as well as all MAVLink-related source files  

#### <ins>Other Non-trivial Examples<ins>

&emsp;**http** -- parser for Hypertext Transfer Protocol as per RFC 7230  
&emsp;**url** -- parser for Uniform Resource Locators as per RFC 3986  
&emsp;**J1939** -- parser for J1939 protocol  


### Parser Subdirectories

Each parser directory may contain subdirectories:  

&emsp;**bison** -- the working bison version of the grammar  
&emsp;**xbnf** -- the working xbnf version of the grammar  
&emsp;**hmr** -- the hammer verion of the grammar  
&emsp;**tests** -- test vectors (pass.N for valid inputs; fail.N for invalid inputs)  
&emsp;**tests.src** -- a directory for generating and installing test vectors in tests  

## Other Directories

&emsp;**ppxml** -- a routine to generate an xml description of a Hammer parser  
&emsp;**xbnf** -- the xbnf pre-processor, written in BNF for Bison  
&emsp;**scripts** -- useful bash scripts -- copy them to ~/bin and place in your path  
&emsp;**RESULTS** -- each time _runall.sh_ is executed, it places a results file here  
&emsp;**bugs** -- Hammer implementation issues  

## Building a new Grammar (e.g. foo)

Follow the established pattern:

**cp -rf gmr0 foo** -- copy the gmr0 directory to a new grammer (foo)  

**In foo/tests:**  
-- create a set of foo test vector files  

**In foo/bison:**  
edit gmr.y -- implement the bison grammar  
make clean -- clean the old files  
make -- make the bison parser  
make run -- run it on the foo test vector files  

**In foo/xbnf:**  
edit gmr.xbnf -- implement the xbnf grammar  
make clean -- clean the old files  
make -- make the xbnf parser  
make run -- run it on the foo test vector files  

**In foo/hmr:**  
edit hmr.c -- implement the hammer grammar  
make clean -- clean out old files  
make -- make the hammer parser to run directly  
make run -- run hammer paser on the test vector files  
make xml -- generate xml  

**In Repo Directory:**  
Add entries to the _clearall_, _makeall_, _run_, and _runall_ scripts.  



