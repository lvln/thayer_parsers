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
    // NA | WORKS!!!
    rule | c_wsp_star c_nl
    ;

// *c-wsp
c_wsp_star:
    /* empty */
    | c_wsp_star c_wsp
    ;

// rule = rulename defined-as elements c-nl
rule:
    rulename defined_as elements // c_nl

// rulename = ALPHA *(ALPHA / DIGIT / "-")
rulename:
    ALPHA rulename_star
    ;

rulename_star:
    /* empty */ | rulename_star rulenam
    ;

rulenam:
    ALPHA | DIGIT | '-'
    ;

// defined-as = *c-wsp ("=" / "=/") *c-wsp
defined_as:
    c_wsp_star defined_a
    ;

defined_a:
    '=' c_wsp_star | "=/" c_wsp_star
    ;

// elements = alternation *c-wsp
elements:
    alternation c_wsp_star
    ;

// c-wsp = WSP / (c-nl WSP)
c_wsp:
    WSP | c_nl WSP
    ;

// c-nl = comment / CRLF
c_nl:
    comment | CRLF
    ;

// comment = ";" *(WSP / VCHAR) CRLF
comment:
    ';' comment_star CRLF
    ;

comment_star:
    /* empty */ | comment_star commen
    ;

commen:
    WSP | VCHAR
    ;

// alternation = concatenation *(*c-wsp "/" *c-wsp concatenation)
alternation:
    concatenation alternation_star
    ;

alternation_star:
    /* empty */ | alternation_star alternatio
    ;

alternatio:
    c_wsp_star '/' c_wsp_star concatenation
    ;

// concatenation = repetition *(1*c-wsp repetition)
concatenation:
    repetition concatenation_star
    ;
    
concatenatio:
    c_wsp_plus repetition
    ;

concatenation_star:
    /* empty */
    | concatenation_star concatenatio
    ;

// 1*c-wsp
c_wsp_plus:
    c_wsp | c_wsp_plus c_wsp
    ;

// repetition = [repeat] element
repetition:
     repetition_query element
     ;

repetition_query:
    /* empty */ | repeat
    ;

// repeat = 1*DIGIT / (*DIGIT "*" *DIGIT)
repeat:
    DIGIT_plus | DIGIT_star '*' DIGIT_star
    ;

// *DIGIT
DIGIT_star:
    /* empty */ | DIGIT_star DIGIT
    ;

// 1*DIGIT
DIGIT_plus:
    DIGIT | DIGIT_plus DIGIT
    ;

// element = rulename / group / option / char-val / num-val / prose-val
element:
   rulename | group | option | char_val | num_val | prose_val
   ;

// group = "(" *c-wsp alternation *c-wsp ")"
group:
    '(' c_wsp_star alternation c_wsp_star ')'
    ;

// option = "[" *c-wsp alternation *c-wsp "]"
option:
    '[' c_wsp_star alternation c_wsp_star ']'
    ;

// char-val = DQUOTE *(%x20-21 / %x23-7E) DQUOTE
char_val:
    DQUOTE char_val_star DQUOTE
    ;

char_val_star:
    /* empty */
    | char_val_star char_va
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

// num-val = "%" (bin-val / dec-val / hex-val)
num_val:
    '%' bin_val | '%' dec_val | '%' hex_val
    ;

// bin-val = "b" 1*BIT [ 1*("." 1*BIT) / ("-" 1*BIT) ]
bin_val:
    'b' BIT_plus bin_val_query
    ;

bin_val_query:
    /* empty */ | bin_va
    ;

bin_va:
    bin_val_plus | '-' BIT_plus
    ;

bin_val_plus:
    '.' BIT_plus | bin_val_plus '.' BIT_plus
    ;

// 1*BIT
BIT_plus:
    BIT | BIT_plus BIT
    ;

// dec-val = "d" 1*DIGIT [ 1*("." 1*DIGIT) / ("-" 1*DIGIT) ]
dec_val:
    'd' DIGIT_plus dec_val_query
    ;

dec_val_query:
    /* empty */ | dec_va
    ;

dec_va:
    dec_val_plus | '-' DIGIT_plus
    ;

dec_val_plus:
    '.' DIGIT_plus | dec_val_plus '.' DIGIT_plus
    ;

// hex-val = "x" 1*HEXDIG [ 1*("." 1*HEXDIG) / ("-" 1*HEXDIG) ]
hex_val:
    'x' HEXDIG_plus hex_val_query
    ;

hex_val_query:
    /* empty */ | hex_va
    ;

hex_va:
    hex_val_plus | '-' HEXDIG_plus
    ;

hex_val_plus:
    '.' HEXDIG_plus | hex_val_plus '.' HEXDIG_plus
    ;

// 1*HEXDIG
HEXDIG_plus:
    HEXDIG | HEXDIG_plus HEXDIG
    ;

// prose-val = "<" *(%x20-3D / %x3F-7E) ">"
prose_val:
    '<' prose_val_star '>'
    ;

prose_val_star:
    /* empty */
    | prose_val_star prose_va
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
