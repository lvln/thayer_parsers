%{
	#define YYDEBUG 1
	int yylex(void);
	void yyerror(char *s);
%}

%token X00
%start cmds

%%
/* The ABNF Grammar alternate universe */

/* lines of commands */
cmds	: cmd
    	| cmds cmd
	;

/* a line could be a command or a comment line */
cmd	: word whitesp EQ whitesp elements NL
     	| ncomment NL
     	;

/* words are anything but the element commands below */
word	: SPCHAR
     	| word SPCHAR
     	;

words	: word
      	| words whitesp word
	;

/* Precidence order so group, etc. picked up before terminals */
elements: string
	| group
	| option
	| prose
	| ncomment
	| terminals
	;

terminals: terminal
	| terminals terminal
	;

// num-val
terminal: '%''b' binary
	| '%''d' decimal
	| '%''x' hex
	;

binary	: BIT
       	| binary BIT
      	;

decimal	: DIGIT
	| decimal DIGIT
	;

hex	: HEXDIG
    	| hex HEXDIG
    	;

string	: '"' words '"'
       	;

group	: '(' words ')'
       	;

option	: '[' words ']'
       	;

prose	: '<' words '>'
       	;

whitesp	: WSP
	| whitesp WSP
	;

ncomment: ';' WSP words
	;

NL	: '\n'
   	| '\r'
	;

// end new

BIT:
    '0' | '1'
    ;

DIGIT:
	'\x30' | '\x31' | '\x32' | '\x33' | '\x34' | '\x35' | '\x36' | '\x37' | '\x38' | '\x39'
	;

EQ:	'='
  	;

HEXDIG:
    DIGIT | 'A' | 'B' | 'C' | 'D' | 'E' | 'F'
    ;

HTAB:
	'\x09'
	;

SP:
	'\x20'
	;

/* Character set missing 'command' characters like <, (, ;, etc. */
SPCHAR:
	'\x21' | '\x23' | '\x24' | '\x25' | '\x26' | '\x27' | '\x2A' |
	'\x2B' | '\x2C' | '\x2D' | '\x2E' | '\x2F' | '\x30' | '\x31' | '\x32' | '\x33' | '\x34' |
	'\x35' | '\x36' | '\x37' |  '\x38' | '\x39' | '\x3A' | '\x3D' |
	'\x3F' | '\x40' | '\x41' | '\x42' | '\x43' | '\x44' | '\x45' | '\x46' | '\x47' | '\x48' |
	'\x49' | '\x4A' | '\x4B' | '\x4C' | '\x4D' | '\x4E' | '\x4F' | '\x50' | '\x51' | '\x52' |
	'\x53' | '\x54' | '\x55' | '\x56' | '\x57' | '\x58' | '\x59' | '\x5A' | '\x5B' | '\x5C' |
	'\x5D' | '\x5E' | '\x5F' | '\x60' | '\x61' | '\x62' | '\x63' | '\x64' | '\x65' | '\x66' |
	'\x67' | '\x68' | '\x69' | '\x6A' | '\x6B' | '\x6C' | '\x6D' | '\x6E' | '\x6F' | '\x70' |
	'\x71' | '\x72' | '\x73' | '\x74' | '\x75' | '\x76' | '\x77' | '\x78' | '\x79' | '\x7A' |
	'\x7B' | '\x7C' | '\x7D' | '\x7E'
	;

WSP:
    SP | HTAB
    ;

%%
