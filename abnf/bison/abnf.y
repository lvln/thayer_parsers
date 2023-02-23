%{
	#define YYDEBUG 1
	int yylex(void);
	void yyerror(char *s);
%}

%token X00

%%
/* The ABNF Grammar */
// rulelist = 1*( rule / (*c-wsp c-nl) )
rulelist:
    rulelis | rulelist rulelis 
    ;

rulelis:
    rule | rulelists c-nl
    ;

rulelists:
    /* empty */ | rulelists c-wsp
    ;

// rule = rulename defined-as elements c-nl
rule:
    rulename defined-as elements c-nl
    ;

// rulename = ALPHA *(ALPHA / DIGIT / "-")
rulename:
    ALPHA rulenames
    ;

rulenames:
    /* empty */ | rulenames rulenam
    ;

rulenam:
    ALPHA | DIGIT | '-'
    ;

// defined-as = *c-wsp ("=" / "=/") *c-wsp
defined-as:
    defined-ass defined-a
    ;

defined-a:
    '=' defined-ass | "=/" defined-ass
    ;

defined-ass:
    /* empty */ | defined-ass c-wsp
    ;

// elements = alternation *c-wsp
elements:
    alternation elementss
    ;

elementss:
    /* empty */ | elementss c-wsp
    ;

// c-wsp = WSP / (c-nl WSP)
c-wsp:
    WSP | c-nl WSP
    ;

// c-nl = comment / CRLF
c-nl:
    comment | CRLF
    ;

// comment = ";" *(WSP / VCHAR) CRLF
comment:
    ';' comments CRLF
    ;

comments:
    /* empty */ | comments commen
    ;

commen:
    WSP | VCHAR
    ;

// alternation = concatenation *(*c-wsp "/" *c-wsp concatenation)
alternation:
    concatenation alternations
    ;

alternations:
    /* empty */ | alternations alternatio
    ;

alternatio:
    alternationss '/' alternationss concatenation
    ;

alternationss:
    /* empty */ | alternationss c-wsp
    ;

// concatenation = repetition *(1*c-wsp repetition)
concatenation:
    repetition concatenations
    ;

concatenations:
    /* empty */ | concatenations concatenatio
    ;
    
concatenatio:
    concatenationss repetition
    ;

concatenationss:
    c-wsp | concatenationss c-wsp
    ;

// repetition = [repeat] element
repetition:
    repetitions element
    ;

repetitions:
    /* empty */ | repeat
    ;
// Note:
// Rule change FROM
// repeat = 1*DIGIT / (*DIGIT "*" *DIGIT)
// TO
// repeat = *DIGIT / (*DIGIT "*" *DIGIT)
// resolves 50 shift/reduce errors
repeat:
    repeatss | repeatss '*' repeatss
    ;

//repeats:
//    /* empty */ | repeatss DIGIT
//    ;

repeatss:
    DIGIT | repeatss DIGIT
    ;

// element = rulename / group / option / char-val / num-val / prose-val
element:
   rulename | group | option | char-val | num-val | prose-val
   ;

// group = "(" *c-wsp alternation *c-wsp ")"
group:
    '(' groups alternation groups ')'
    ;

groups:
    /* empty */ | groups c-wsp
    ;

// option = "[" *c-wsp alternation *c-wsp "]"
option:
    '[' options alternation options ']'
    ;

options:
    /* empty */ | options c-wsp
    ;

// char-val = DQUOTE *(%x20-21 / %x23-7E) DQUOTE
char-val:
    DQUOTE char-vals DQUOTE
    ;

char-vals:
    /* empty */ | char-vals char-va
    ;

char-va:
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

// num-val = "%" (bin-val / dec-val / hex-val)
num-val:
    '%' bin-val | '%' dec-val | '%' hex-val
    ;

// bin-val = "b" 1*BIT [ 1*("." 1*BIT) / ("-" 1*BIT) ]
bin-val:
    'b' bin-vals bin-valssss
    ;

bin-valssss:
    /* empty */ | bin-va
    ;

bin-va:
    bin-valsss | '-' bin-vals
    ;

bin-valsss:
    '.' bin-vals | bin-valsss '.' bin-vals
    ;

bin-vals:
    BIT | bin-vals BIT
    ;

// dec-val = "d" 1*DIGIT [ 1*("." 1*DIGIT) / ("-" 1*DIGIT) ]
dec-val:
    'd' dec-vals dec-valssss
    ;

dec-valssss:
    /* empty */ | dec-va
    ;

dec-va:
    dec-valsss | '-' dec-vals
    ;

dec-valsss:
    '.' dec-vals | dec-valsss '.' dec-vals
    ;

dec-vals:
    DIGIT | dec-vals DIGIT
    ;

// hex-val = "x" 1*HEXDIG [ 1*("." 1*HEXDIG) / ("-" 1*HEXDIG) ]
hex-val:
    'x' hex-vals hex-valssss
    ;

hex-valssss:
    /* empty */ | hex-va
    ;

hex-va:
    hex-valsss | '-' hex-vals
    ;

hex-valsss:
    '.' hex-vals | hex-valsss '.' hex-vals
    ;

hex-vals:
    HEXDIG | hex-vals HEXDIG
    ;

// prose-val = "<" *(%x20-3D / %x3F-7E) ">"
prose-val:
    '<' prose-vals '>'
    ;

prose-vals:
    /* empty */ | prose-vals prose-va
    ;

prose-va:
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

ALPHA:
	'\x41' | '\x42' | '\x43' | '\x44' | '\x45' | '\x46' | '\x47' | '\x48' | '\x49' | '\x4A' |
	'\x4B' | '\x4C' | '\x4D' | '\x4E' | '\x4F' | '\x50' | '\x51' | '\x52' | '\x53' | '\x54' |
	'\x55' | '\x56' | '\x57' | '\x58' | '\x59' | '\x5A' | '\x61' | '\x62' | '\x63' | '\x64' |
	'\x65' | '\x66' | '\x67' | '\x68' | '\x69' | '\x6A' | '\x6B' | '\x6C' | '\x6D' | '\x6E' |
	'\x6F' | '\x70' | '\x71' | '\x72' | '\x73' | '\x74' | '\x75' | '\x76' | '\x77' | '\x78' |
	'\x79' | '\x7A'
	;

BIT:
    '0' | '1'
    ;

CR:
	'\x0D'
	;

CRLF:
    CR LF
    ;

DIGIT:
	'\x30' | '\x31' | '\x32' | '\x33' | '\x34' | '\x35' | '\x36' | '\x37' | '\x38' | '\x39'
	;

DQUOTE:
	'\x22'
	;

HEXDIG:
    DIGIT | 'A' | 'B' | 'C' | 'D' | 'E' | 'F'
    ;

HTAB:
	'\x09'
	;

LF:
	'\x0A'
	;

SP:
	'\x20'
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

WSP:
    SP | HTAB
    ;

%%
