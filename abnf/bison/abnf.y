%{
	#define YYDEBUG 1
	int yylex(void);
	void yyerror(char *s);
%}

%token X00

%% /* The ABNF Grammar */

/* rulelist = 1*( rule | (*c-wsp c-nl) ) ; */
element_1: rule | rulelists c-nl ;
rulelist: element_1 | rulelist element_1;
rulelists: /*empty*/ | rulelists c-wsp ;

/* rule = rulename defined-as elements c-nl ; */
rule: rulename defined-as elements c-nl ;

/* rulename = ALPHA *(ALPHA | DIGIT | "-") ; */
element_2: ALPHA | DIGIT | "-" ;
rulename: ALPHA rulenames ;
rulenames: /*empty*/ | rulenames element_2 ;

/* defined-as = *c-wsp ("=" | "=/") *c-wsp ; */
/* Definition for *c-wsp ; */
defined-as: defined-ass "=" | "=/" defined-ass ;
defined-ass: /*empty*/ | defined-ass c-wsp ;

/* elements = alternation *c-wsp ; */
elements: alternation defined-ass ;

/* c-wsp = WSP | (c-nl WSP) ; */
c-wsp: WSP | c-nl WSP ;

/* c-nl = comment | CRLF ; */
c-nl: comment | CRLF ;

/* comment = ";" *(WSP | VCHAR) CRLF ; */
element_3: WSP | VCHAR
comment: ";" comments CRLF ;
comments: /*empty*/ | comments element_3 ;

/* alternation: concatenation *(*c-wsp "/" *c-wsp concatenation) ; */
element_4: defined-ass "/" defined-ass concatenation
alternation: concatenation element_4 ;

/* concatenation: repetition *(1*c-wsp repetition) ; */
/* 1*c-wsp */
element_5: concatenations repetition ;
concatenation: repetition element_5 ;
concatenations: c-wsp | concatenations c-wsp ;

/* repetition: [repeat] element ; */
repetition: repetitions element ;
repetitions: /*empty*/ | repeat ;

/* repeat: 1*DIGIT | (*DIGIT "*" *DIGIT) ; */
repeat: repeatss | repeats "*" repeats ;
repeats: /*empty*/ | repeats DIGIT ;
repeatss: DIGIT | repeatss DIGIT ;

/* element: rulename | group | option | char-val | num-val | prose-val ; */
element: rulename | group | option | char-val | num-val | prose-val ;

/* group: "(" *c-wsp alternation *c-wsp ")" ; */
group: "(" defined-ass alternation defined-ass ")" ;

/* option: "[" *c-wsp alternation *c-wsp "]" ; */
option: "[" defined-ass alternation defined-ass "]" ;

/* char-val: DQUOTE *(%x20-21 | %x23-7E) DQUOTE ; */
element_6: '\x20' | '\x21' | '\x23' | '\x24' | '\x25' | '\x26' | '\x27' | '\x28' | '\x29' | '\x2A' | '\x2B' | '\x2C' | '\x2D' | '\x2E' | '\x2F' | '\x30' | '\x31' | '\x32' | '\x33' | '\x34' | '\x35' | '\x36' | '\x37' | '\x38' | '\x39' | '\x3A' | '\x3B' | '\x3C' | '\x3D' | '\x3E' | '\x3F' | '\x40' | '\x41' | '\x42' | '\x43' | '\x44' | '\x45' | '\x46' | '\x47' | '\x48' | '\x49' | '\x4A' | '\x4B' | '\x4C' | '\x4D' | '\x4E' | '\x4F' | '\x50' | '\x51' | '\x52' | '\x53' | '\x54' | '\x55' | '\x56' | '\x57' | '\x58' | '\x59' | '\x5A' | '\x5B' | '\x5C' | '\x5D' | '\x5E' | '\x5F' | '\x60' | '\x61' | '\x62' | '\x63' | '\x64' | '\x65' | '\x66' | '\x67' | '\x68' | '\x69' | '\x6A' | '\x6B' | '\x6C' | '\x6D' | '\x6E' | '\x6F' | '\x70' | '\x71' | '\x72' | '\x73' | '\x74' | '\x75' | '\x76' | '\x77' | '\x78' | '\x79' | '\x7A' | '\x7B' | '\x7C' | '\x7D' | '\x7E' ;
char-val: DQUOTE char-vals DQUOTE ;
char-vals: /*empty*/ | char-vals element_6 ;

/* num-val: "%" (bin-val | dec-val | hex-val) ; */
num-val: "%" bin-val | "%" dec-val | "%" hex-val ;

/* bin-val: "b" 1*BIT [ 1*("." 1*BIT) | ("-" 1*BIT) ] ; */
element_7: bin-valss | "-" bin-vals ;
bin-val: "b" bin-vals bin-valsss ;
bin-vals: BIT | bin-vals BIT ; /* 1*BIT */
bin-valss: "." bin-vals | bin-valss "." bin-vals ; /* 1*("." 1*BIT) */
bin-valsss: /*empty*/ | element_7 ; /* [ 1*("." 1*BIT) | ("-" 1*BIT) ] */

/* dec-val: "d" 1*DIGIT [ 1*("." 1*DIGIT) | ("-" 1*DIGIT) ] ; */
element_8: dec-valss | "-" dec-vals ; /* [ 1*("." 1*DIGIT) | ("-" 1*DIGIT) ] */
dec-val: "d" dec-vals dec-valsss ;
dec-vals: DIGIT | dec-vals DIGIT ; /* 1*DIGIT */
dec-valss: "." dec-vals | dec-valss "." dec-vals; /* 1*("." 1*DIGIT) */
dec-valsss: /*empty*/ | element_8 ; /* [ 1*("." 1*DIGIT) | ("-" 1*DIGIT) ] */

/* hex-val: "x" 1*HEXDIG [ 1*("." 1*HEXDIG) | ("-" 1*HEXDIG) ] ; */
element_9: hex-valss | "-" hex-vals /* [ 1*("." 1*HEXDIG) | ("-" 1*HEXDIG) ] */
hex-val: "x" hex-vals hex-valsss ;
hex-vals: HEXDIG | hex-vals HEXDIG ; /* 1*HEXDIG */
hex-valss: "." hex-vals | hex-valss "." hex-vals ; /* 1*("." 1*HEXDIG) */
hex-valsss: /*empty*/ | element_9 ; /* [ 1*("." 1*HEXDIG) | ("-" 1*HEXDIG) ] */

/* prose-val: "<" *(%x20-3D | %x3F-7E) ">" ; */
/* %x20-3D | %x3F-7E */
element_10: '\x20' | '\x21' | '\x22' | '\x23' | '\x24' | '\x25' | '\x26' | '\x27' | '\x28' | '\x29' | '\x2A' | '\x2B' | '\x2C' | '\x2D' | '\x2E' | '\x2F' | '\x30' | '\x31' | '\x32' | '\x33' | '\x34' | '\x35' | '\x36' | '\x37' | '\x38' | '\x39' | '\x3A' | '\x3B' | '\x3C' | '\x3D' | '\x3F' | '\x40' | '\x41' | '\x42' | '\x43' | '\x44' | '\x45' | '\x46' | '\x47' | '\x48' | '\x49' | '\x4A' | '\x4B' | '\x4C' | '\x4D' | '\x4E' | '\x4F' | '\x50' | '\x51' | '\x52' | '\x53' | '\x54' | '\x55' | '\x56' | '\x57' | '\x58' | '\x59' | '\x5A' | '\x5B' | '\x5C' | '\x5D' | '\x5E' | '\x5F' | '\x60' | '\x61' | '\x62' | '\x63' | '\x64' | '\x65' | '\x66' | '\x67' | '\x68' | '\x69' | '\x6A' | '\x6B' | '\x6C' | '\x6D' | '\x6E' | '\x6F' | '\x70' | '\x71' | '\x72' | '\x73' | '\x74' | '\x75' | '\x76' | '\x77' | '\x78' | '\x79' | '\x7A' | '\x7B' | '\x7C' | '\x7D' | '\x7E' ;
prose-val: "<" prose-vals ">" ;
prose-vals: /*empty*/ | prose-vals element_10 ; /* *element_10 */

ALPHA: '\x41' | '\x42' | '\x43' | '\x44' | '\x45' | '\x46' | '\x47' | '\x48' | '\x49' | '\x4A' | '\x4B' | '\x4C' | '\x4D' | '\x4E' | '\x4F' | '\x50' | '\x51' | '\x52' | '\x53' | '\x54' | '\x55' | '\x56' | '\x57' | '\x58' | '\x59' | '\x5A' | '\x61' | '\x62' | '\x63' | '\x64' | '\x65' | '\x66' | '\x67' | '\x68' | '\x69' | '\x6A' | '\x6B' | '\x6C' | '\x6D' | '\x6E' | '\x6F' | '\x70' | '\x71' | '\x72' | '\x73' | '\x74' | '\x75' | '\x76' | '\x77' | '\x78' | '\x79' | '\x7A' ;

BIT: '0' | '1' ;

CHAR: '\x01' | '\x02' | '\x03' | '\x04' | '\x05' | '\x06' | '\x07' | '\x08' | '\x09' | '\x0A' | '\x0B' | '\x0C' | '\x0D' | '\x0E' | '\x0F' | '\x10' | '\x11' | '\x12' | '\x13' | '\x14' | '\x15' | '\x16' | '\x17' | '\x18' | '\x19' | '\x1A' | '\x1B' | '\x1C' | '\x1D' | '\x1E' | '\x1F' | '\x20' | '\x21' | '\x22' | '\x23' | '\x24' | '\x25' | '\x26' | '\x27' | '\x28' | '\x29' | '\x2A' | '\x2B' | '\x2C' | '\x2D' | '\x2E' | '\x2F' | '\x30' | '\x31' | '\x32' | '\x33' | '\x34' | '\x35' | '\x36' | '\x37' | '\x38' | '\x39' | '\x3A' | '\x3B' | '\x3C' | '\x3D' | '\x3E' | '\x3F' | '\x40' | '\x41' | '\x42' | '\x43' | '\x44' | '\x45' | '\x46' | '\x47' | '\x48' | '\x49' | '\x4A' | '\x4B' | '\x4C' | '\x4D' | '\x4E' | '\x4F' | '\x50' | '\x51' | '\x52' | '\x53' | '\x54' | '\x55' | '\x56' | '\x57' | '\x58' | '\x59' | '\x5A' | '\x5B' | '\x5C' | '\x5D' | '\x5E' | '\x5F' | '\x60' | '\x61' | '\x62' | '\x63' | '\x64' | '\x65' | '\x66' | '\x67' | '\x68' | '\x69' | '\x6A' | '\x6B' | '\x6C' | '\x6D' | '\x6E' | '\x6F' | '\x70' | '\x71' | '\x72' | '\x73' | '\x74' | '\x75' | '\x76' | '\x77' | '\x78' | '\x79' | '\x7A' | '\x7B' | '\x7C' | '\x7D' | '\x7E' | '\x7F' ;

CR: '\x0D' ;

CRLF: CR LF ;

CTL: X00 | '\x01' | '\x02' | '\x03' | '\x04' | '\x05' | '\x06' | '\x07' | '\x08' | '\x09' | '\x0A' | '\x0B' | '\x0C' | '\x0D' | '\x0E' | '\x0F' | '\x10' | '\x11' | '\x12' | '\x13' | '\x14' | '\x15' | '\x16' | '\x17' | '\x18' | '\x19' | '\x1A' | '\x1B' | '\x1C' | '\x1D' | '\x1E' | '\x1F' | '\x7F' ;

DIGIT: '\x30' | '\x31' | '\x32' | '\x33' | '\x34' | '\x35' | '\x36' | '\x37' | '\x38' | '\x39' ;

DQUOTE: '\x22' ;

HEXDIG: DIGIT | 'A' | 'B' | 'C' | 'D' | 'E' | 'F' ;

HTAB: '\x09' ;

LF: '\x0A' ;

/* LWSP: *(WSP | CRLF WSP) ; */
element_11: WSP | CRLF WSP
LWSP: /*empty*/ | LWSP element_11 ;

OCTET: X00 | '\x01' | '\x02' | '\x03' | '\x04' | '\x05' | '\x06' | '\x07' | '\x08' | '\x09' | '\x0A' | '\x0B' | '\x0C' | '\x0D' | '\x0E' | '\x0F' | '\x10' | '\x11' | '\x12' | '\x13' | '\x14' | '\x15' | '\x16' | '\x17' | '\x18' | '\x19' | '\x1A' | '\x1B' | '\x1C' | '\x1D' | '\x1E' | '\x1F' | '\x20' | '\x21' | '\x22' | '\x23' | '\x24' | '\x25' | '\x26' | '\x27' | '\x28' | '\x29' | '\x2A' | '\x2B' | '\x2C' | '\x2D' | '\x2E' | '\x2F' | '\x30' | '\x31' | '\x32' | '\x33' | '\x34' | '\x35' | '\x36' | '\x37' | '\x38' | '\x39' | '\x3A' | '\x3B' | '\x3C' | '\x3D' | '\x3E' | '\x3F' | '\x40' | '\x41' | '\x42' | '\x43' | '\x44' | '\x45' | '\x46' | '\x47' | '\x48' | '\x49' | '\x4A' | '\x4B' | '\x4C' | '\x4D' | '\x4E' | '\x4F' | '\x50' | '\x51' | '\x52' | '\x53' | '\x54' | '\x55' | '\x56' | '\x57' | '\x58' | '\x59' | '\x5A' | '\x5B' | '\x5C' | '\x5D' | '\x5E' | '\x5F' | '\x60' | '\x61' | '\x62' | '\x63' | '\x64' | '\x65' | '\x66' | '\x67' | '\x68' | '\x69' | '\x6A' | '\x6B' | '\x6C' | '\x6D' | '\x6E' | '\x6F' | '\x70' | '\x71' | '\x72' | '\x73' | '\x74' | '\x75' | '\x76' | '\x77' | '\x78' | '\x79' | '\x7A' | '\x7B' | '\x7C' | '\x7D' | '\x7E' | '\x7F' | '\x80' | '\x81' | '\x82' | '\x83' | '\x84' | '\x85' | '\x86' | '\x87' | '\x88' | '\x89' | '\x8A' | '\x8B' | '\x8C' | '\x8D' | '\x8E' | '\x8F' | '\x90' | '\x91' | '\x92' | '\x93' | '\x94' | '\x95' | '\x96' | '\x97' | '\x98' | '\x99' | '\x9A' | '\x9B' | '\x9C' | '\x9D' | '\x9E' | '\x9F' | '\xA0' | '\xA1' | '\xA2' | '\xA3' | '\xA4' | '\xA5' | '\xA6' | '\xA7' | '\xA8' | '\xA9' | '\xAA' | '\xAB' | '\xAC' | '\xAD' | '\xAE' | '\xAF' | '\xB0' | '\xB1' | '\xB2' | '\xB3' | '\xB4' | '\xB5' | '\xB6' | '\xB7' | '\xB8' | '\xB9' | '\xBA' | '\xBB' | '\xBC' | '\xBD' | '\xBE' | '\xBF' | '\xC0' | '\xC1' | '\xC2' | '\xC3' | '\xC4' | '\xC5' | '\xC6' | '\xC7' | '\xC8' | '\xC9' | '\xCA' | '\xCB' | '\xCC' | '\xCD' | '\xCE' | '\xCF' | '\xD0' | '\xD1' | '\xD2' | '\xD3' | '\xD4' | '\xD5' | '\xD6' | '\xD7' | '\xD8' | '\xD9' | '\xDA' | '\xDB' | '\xDC' | '\xDD' | '\xDE' | '\xDF' | '\xE0' | '\xE1' | '\xE2' | '\xE3' | '\xE4' | '\xE5' | '\xE6' | '\xE7' | '\xE8' | '\xE9' | '\xEA' | '\xEB' | '\xEC' | '\xED' | '\xEE' | '\xEF' | '\xF0' | '\xF1' | '\xF2' | '\xF3' | '\xF4' | '\xF5' | '\xF6' | '\xF7' | '\xF8' | '\xF9' | '\xFA' | '\xFB' | '\xFC' | '\xFD' | '\xFE' | '\xFF' ;

SP: '\x20' ;

VCHAR: '\x21' | '\x22' | '\x23' | '\x24' | '\x25' | '\x26' | '\x27' | '\x28' | '\x29' | '\x2A' | '\x2B' | '\x2C' | '\x2D' | '\x2E' | '\x2F' | '\x30' | '\x31' | '\x32' | '\x33' | '\x34' | '\x35' | '\x36' | '\x37' | '\x38' | '\x39' | '\x3A' | '\x3B' | '\x3C' | '\x3D' | '\x3E' | '\x3F' | '\x40' | '\x41' | '\x42' | '\x43' | '\x44' | '\x45' | '\x46' | '\x47' | '\x48' | '\x49' | '\x4A' | '\x4B' | '\x4C' | '\x4D' | '\x4E' | '\x4F' | '\x50' | '\x51' | '\x52' | '\x53' | '\x54' | '\x55' | '\x56' | '\x57' | '\x58' | '\x59' | '\x5A' | '\x5B' | '\x5C' | '\x5D' | '\x5E' | '\x5F' | '\x60' | '\x61' | '\x62' | '\x63' | '\x64' | '\x65' | '\x66' | '\x67' | '\x68' | '\x69' | '\x6A' | '\x6B' | '\x6C' | '\x6D' | '\x6E' | '\x6F' | '\x70' | '\x71' | '\x72' | '\x73' | '\x74' | '\x75' | '\x76' | '\x77' | '\x78' | '\x79' | '\x7A' | '\x7B' | '\x7C' | '\x7D' | '\x7E' ;

WSP: SP | HTAB ;

%% /* C Code */
