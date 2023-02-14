%{
	#define YYDEBUG 1
	int yylex(void);
	void yyerror(char *s);
%}

%token X00

%%
/* The ABNF Grammar */
rulelist:
	ruleli
	| rulelist ruleli
	;

ruleli:
	rule
	| rulelis
	;

rulelis:
	rulelists c_nl
	;

rulelists:
	/*empty*/
	| rulelists c_wsp
	;

rule:
	rulename defined_as elements c_nl
	;

rulename:
	ALPHA rulenames
	;

rulenames:
	/*empty*/
	| rulenames rulenam
	;

rulenam:
	ALPHA
	| DIGIT
	| '-'
	;

defined_as:
	defined_ass defined_a
	;

defined_a:
	'=' defined_ass
	| "=/" defined_ass
	;

defined_ass:
	/* empty */
	| defined_ass c_wsp
	;

elements:
	alternation elementss
	;

elementss:
	/* empty */
	| elementss c_wsp
	;

c_wsp:
	WSP
	| c_nl WSP
	;

c_nl:
	comment
	| CRLF
	;

comment:
	';' comments CRLF
	;

comments:
	/*empty*/
	| comments commen
	;

commen:
	WSP
	| VCHAR
	;

alternation:
    concatenation alternations
    ;

alternations:
    /* empty */
    | alternations alternatio
    ;

alternatio:
    alternati '/' alternati concatenation
    ;

alternati:
    /* empty */
    | alternati c_wsp
    ;

concatenation:
    repetition concatenations
    ;

concatenations:
    /*empty*/
	| concatenations concatenatio
	;

concatenatio:
    concatenati repetition
    ;

concatenati:
    c_wsp
	| concatenati c_wsp
	;

repetition:
	repetitions element
	;

repetitions:
	/*empty*/
	| repeat
	;

repeat:
	repeats
	| repeats '*' repeats
	;

repeats:
	DIGIT
	| repeats DIGIT 
	;

element:
	rulename
	| group
	| option
	| char_val
	| num_val
	| prose_val
	;

group:
	'(' groups alternation groups ')'
	;

groups:
	/*empty*/
	| groups c_wsp
	;

option:
	'[' options alternation options ']'
	;

options:
	/*empty*/
	| options c_wsp
	;

char_val:
	DQUOTE char_vals DQUOTE
	;

char_vals:
	/*empty*/
	| char_vals char_va
	;

char_va:
	'\x20' | '\x21' | '\x23' | '\x24' | '\x25' | '\x26' | '\x27' | '\x28' | '\x29' | '\x2A' |
	'\x2B' | '\x2C' | '\x2D' | '\x2E' | '\x2F' | '\x30' | '\x31' | '\x32' | '\x33' | '\x34' |
	'\x35' | '\x36' | '\x37' | '\x38' | '\x39' | '\x3A' | '\x3B' | '\x3C' | '\x3D' | '\x3E' |
	'\x3F' | '\x40' | '\x41' | '\x42' | '\x43' | '\x44' | '\x45' | '\x46' | '\x47' | '\x48' |
	'\x49' | '\x4A' | '\x4B' | '\x4C' | '\x4D' | '\x4E' | '\x4F' | '\x50' | '\x51' | '\x52' |
	'\x53' | '\x54' | '\x55' | '\x56' | '\x57' | '\x58' | '\x59' | '\x5A' | '\x5B' | '\x5C' |
	'\x5D' | '\x5E' | '\x5F' | '\x60' | '\x61' | '\x62' | '\x63' | '\x64' | '\x65' | '\x66' |
	'\x67' | '\x68' | '\x69' | '\x6A' | '\x6B' | '\x6C' | '\x6D' | '\x6E' | '\x6F' | '\x70' |
	'\x71' | '\x72' | '\x73' | '\x74' | '\x75' | '\x76' | '\x77' | '\x78' | '\x79' | '\x7A' |
	'\x7B' | '\x7C' | '\x7D' | '\x7E'
	;

num_val:
	'%' bin_val
	| '%' dec_val
	| '%' hex_val
	;

bin_val:
	'b' bin_vals bin_valsss
	;

bin_valsss:
	/*empty*/
	| bin_valssss
	;

bin_valssss:
	bin_valss
	| '_' bin_vals
	;

bin_valss:
	'.' bin_vals
	| bin_valss '.' bin_vals
	;

bin_vals:
	BIT
	| bin_vals BIT
	;

dec_val:
	'd' dec_vals dec_valsss
	;

dec_valsss:
	/*empty*/
	| dec_valssss
	;

dec_valssss:
	dec_valss
	| '_' dec_vals
	;

dec_valss:
	'.' dec_vals
	| dec_valss '.' dec_vals
	;

dec_vals:
	DIGIT
	| dec_vals DIGIT
	;

hex_val:
	'x' hex_vals hex_valsss
	;

hex_valsss:
	/*empty*/
	| hex_valssss
	;

hex_valssss:
	hex_valss
	| '_' hex_vals
	;

hex_valss:
	'.' hex_vals
	| hex_valss '.' hex_vals
	;

hex_vals:
	HEXDIG
	| hex_vals HEXDIG
	;

prose_val:
	'<' prose_vals '>'
	;

prose_vals:
	/*empty*/
	| prose_vals prose_va
	;

prose_va:
	'\x20' | '\x21' | '\x22' | '\x23' | '\x24' | '\x25' | '\x26' | '\x27' | '\x28' | '\x29' |
	'\x2A' | '\x2B' | '\x2C' | '\x2D' | '\x2E' | '\x2F' | '\x30' | '\x31' | '\x32' | '\x33' |
	'\x34' | '\x35' | '\x36' | '\x37' | '\x38' | '\x39' | '\x3A' | '\x3B' | '\x3C' | '\x3D' |
	'\x3F' | '\x40' | '\x41' | '\x42' | '\x43' | '\x44' | '\x45' | '\x46' | '\x47' | '\x48' |
	'\x49' | '\x4A' | '\x4B' | '\x4C' | '\x4D' | '\x4E' | '\x4F' | '\x50' | '\x51' | '\x52' |
	'\x53' | '\x54' | '\x55' | '\x56' | '\x57' | '\x58' | '\x59' | '\x5A' | '\x5B' | '\x5C' |
	'\x5D' | '\x5E' | '\x5F' | '\x60' | '\x61' | '\x62' | '\x63' | '\x64' | '\x65' | '\x66' |
	'\x67' | '\x68' | '\x69' | '\x6A' | '\x6B' | '\x6C' | '\x6D' | '\x6E' | '\x6F' | '\x70' |
	'\x71' | '\x72' | '\x73' | '\x74' | '\x75' | '\x76' | '\x77' | '\x78' | '\x79' | '\x7A' |
	'\x7B' | '\x7C' | '\x7D' | '\x7E'
	;

CRLF:
	CR LF
	;

CR:
	'\x0D'
	;

LF:
	'\x0A'
	;

HEXDIG:
	DIGIT | 'A' | 'B' | 'C' | 'D' | 'E' | 'F'
	;

ALPHA:
	'\x41' | '\x42' | '\x43' | '\x44' | '\x45' | '\x46' | '\x47' | '\x48' | '\x49' | '\x4A' |
	'\x4B' | '\x4C' | '\x4D' | '\x4E' | '\x4F' | '\x50' | '\x51' | '\x52' | '\x53' | '\x54' |
	'\x55' | '\x56' | '\x57' | '\x58' | '\x59' | '\x5A' | '\x61' | '\x62' | '\x63' | '\x64' |
	'\x65' | '\x66' | '\x67' | '\x68' | '\x69' | '\x6A' | '\x6B' | '\x6C' | '\x6D' | '\x6E' |
	'\x6F' | '\x70' | '\x71' | '\x72' | '\x73' | '\x74' | '\x75' | '\x76' | '\x77' | '\x78' |
	'\x79' | '\x7A'
	;

BIT:
	'0'
	| '1'
	;

WSP:
	SP
	| HTAB
	;

SP:
	'\x20'
	;

HTAB:
	'\x09'
	;

DIGIT:
	'\x30' | '\x31' | '\x32' | '\x33' | '\x34' | '\x35' | '\x36' | '\x37' | '\x38' | '\x39'
	;

VCHAR:
	'\x21' | '\x22' | '\x23' | '\x24' | '\x25' | '\x26' | '\x27' | '\x28' | '\x29' | '\x2A' |
	'\x2B' | '\x2C' | '\x2D' | '\x2E' | '\x2F' | '\x30' | '\x31' | '\x32' | '\x33' | '\x34' |
	'\x35' | '\x36' | '\x37' | '\x38' | '\x39' | '\x3A' | '\x3B' | '\x3C' | '\x3D' | '\x3E' |
	'\x3F' | '\x40' | '\x41' | '\x42' | '\x43' | '\x44' | '\x45' | '\x46' | '\x47' | '\x48' |
	'\x49' | '\x4A' | '\x4B' | '\x4C' | '\x4D' | '\x4E' | '\x4F' | '\x50' | '\x51' | '\x52' |
	'\x53' | '\x54' | '\x55' | '\x56' | '\x57' | '\x58' | '\x59' | '\x5A' | '\x5B' | '\x5C' |
	'\x5D' | '\x5E' | '\x5F' | '\x60' | '\x61' | '\x62' | '\x63' | '\x64' | '\x65' | '\x66' |
	'\x67' | '\x68' | '\x69' | '\x6A' | '\x6B' | '\x6C' | '\x6D' | '\x6E' | '\x6F' | '\x70' |
	'\x71' | '\x72' | '\x73' | '\x74' | '\x75' | '\x76' | '\x77' | '\x78' | '\x79' | '\x7A' |
	'\x7B' | '\x7C' | '\x7D' | '\x7E'
	;

DQUOTE:
	'\x22'
	;
%%