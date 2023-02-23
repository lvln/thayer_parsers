%{
	#define YYDEBUG 1
	int yylex(void);
	void yyerror(char *s);
%}

%token X00

%% /* The ABNF USPS Grammar definition -- EDIT */
// full-address
rule:
	// sentence CRLF address CRLF sentence '\x2C' WSP sentence WSP multi-digit rules // domestic
	// | sentence CRLF units CRLF sentence CRLF sentence '\x2C' WSP sentence WSP multi-digit CRLF sentence rules // military
	sentence CRLF sentence CRLF address// buisness
	;

rules:
	/* empty */
	| rules CRLF
	;

unit:
	sentence multi-digits
	;

units:
	unit
	| units WSP unit
        ;

// address
address:
	multi-digits sentence multi-digits
	;

multi-digits:
	/* empty */
	| multi-digit WSP
	| WSP multi-digit
	;

// postal-code
multi-digit:
	DIGIT
	| multi-digit DIGIT
	;

// country
// state
// city-towns:
// name
sentence:
	word
	| sentence WSP word

word:
	ALPHA
	| word ALPHA
	;

ALPHA:
	'\x41' | '\x42' | '\x43' | '\x44' | '\x45' | '\x46' | '\x47' | '\x48' | '\x49' | '\x4A' |
	'\x4B' | '\x4C' | '\x4D' | '\x4E' | '\x4F' | '\x50' | '\x51' | '\x52' | '\x53' | '\x54' |
	'\x55' | '\x56' | '\x57' | '\x58' | '\x59' | '\x5A' | '\x61' | '\x62' | '\x63' | '\x64' |
	'\x65' | '\x66' | '\x67' | '\x68' | '\x69' | '\x6A' | '\x6B' | '\x6C' | '\x6D' | '\x6E' |
	'\x6F' | '\x70' | '\x71' | '\x72' | '\x73' | '\x74' | '\x75' | '\x76' | '\x77' | '\x78' |
	'\x79' | '\x7A'
	;

DIGIT:
	'\x30' | '\x31' | '\x32' | '\x33' | '\x34' | '\x35' | '\x36' | '\x37' | '\x38' | '\x39'
	;

WSP:
	WSPS
	| WSP WSPS
	;

WSPS:
    SP | HTAB
    ;

HTAB:
	'\x09'
	;

SP:
	'\x20'
	;

CRLF:
    CR
	| LF
    ;

CR:
	'\x0D'
	;

LF:
	'\x0A'
	;
%% /* C-Code */
