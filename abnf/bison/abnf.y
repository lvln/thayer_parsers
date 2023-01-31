%{
	#define YYDEBUG 1
	int yylex(void);
	void yyerror(char *s);
%}

%token X00

%% /* The ABNF Grammar */

/* rulelist = 1*( rule | (*c_wsp c_nl) ) ; */
rulelist: ruleli | ruleli rulelist ;
ruleli: rule | rulelis ;
rulelis: rulelists c_nl ;
rulelists: /*empty*/ | c_wsp rulelists ;

/* rule = rulename defined_as elements c_nl ; */
rule: rulename defined_as elements c_nl ;

/* defined_as = *c_wsp ("=" | "=/") *c_wsp ; */
defined_as: defined_ass '=' defined_ass | defined_ass "=/" defined_ass ;
defined_ass: /*empty*/ | c_wsp defined_ass ;

/* elements = alternation *c_wsp ; */
elements: alternation elementss ;
elementss: /*empty*/ | c_wsp elementss ;

/* element: rulename | group | option | char_val | num_val | prose_val ; */
element: rulename | group | option | char_val | num_val | prose_val ;

/* rulename = ALPHA *(ALPHA | DIGIT | "-") ; */
rulename: ALPHA rulenames ;
rulenames: /*empty*/ | rulenam rulenames ;
rulenam: ALPHA | DIGIT | '-' ;

/* group: "(" *c_wsp alternation *c_wsp ")" ; */
group: '(' groups alternation groups ')' ;
groups: /*empty*/ | c_wsp groups ;

/* option: "[" *c_wsp alternation *c_wsp "]" ; */
option: '[' options alternation options ']' ;
options: /*empty*/ | c_wsp options ;

/* alternation: concatenation *(*c_wsp "/" *c_wsp concatenation) ; */
alternation: concatenation alternationss ;
alternationss: /*empty*/ | alternatio alternationss ;
alternatio: alternations '/' alternations concatenation ;
alternations: /*empty*/ | c_wsp alternations ;

/* concatenation: repetition *(1*c_wsp repetition) ; */
concatenation: repetition concatenationss ;
concatenationss: /*empty*/ | concatenatio concatenationss ;
concatenatio: concatenations repetition ;
concatenations: c_wsp | c_wsp concatenations ;

/* repetition: [repeat] element ; */
repetition: repetitions element ;
repetitions: /*empty*/ | repeat ;

/* repeat: 1*DIGIT | (*DIGIT "*" *DIGIT) ; */
repeat: repeats | repeatss '*' repeatss ;
repeats: DIGIT | DIGIT repeats ;
repeatss: /*empty*/ | DIGIT repeatss ;

/* c_wsp = WSP | (c_nl WSP) ; */
c_wsp: WSP | c_nl WSP ;

/* c_nl = comment | CRLF ; */
c_nl: comment | CRLF ;

/* comment = ";" *(WSP | VCHAR) CRLF ; */
comment: ';' comments CRLF ;
comments: /*empty*/ | commen comments ;
commen: WSP | VCHAR ;

/* char_val: DQUOTE *(%x20_21 | %x23_7E) DQUOTE ; */
char_val: DQUOTE char_vals DQUOTE ;
char_vals: /*empty*/ | char_va char_vals ;
char_va: '\x20' | '\x21' | '\x23' | '\x24' | '\x25' | '\x26' | '\x27' | '\x28' | '\x29' | '\x2A' | '\x2B' | '\x2C' | '\x2D' | '\x2E' | '\x2F' | '\x30' | '\x31' | '\x32' | '\x33' | '\x34' | '\x35' | '\x36' | '\x37' | '\x38' | '\x39' | '\x3A' | '\x3B' | '\x3C' | '\x3D' | '\x3E' | '\x3F' | '\x40' | '\x41' | '\x42' | '\x43' | '\x44' | '\x45' | '\x46' | '\x47' | '\x48' | '\x49' | '\x4A' | '\x4B' | '\x4C' | '\x4D' | '\x4E' | '\x4F' | '\x50' | '\x51' | '\x52' | '\x53' | '\x54' | '\x55' | '\x56' | '\x57' | '\x58' | '\x59' | '\x5A' | '\x5B' | '\x5C' | '\x5D' | '\x5E' | '\x5F' | '\x60' | '\x61' | '\x62' | '\x63' | '\x64' | '\x65' | '\x66' | '\x67' | '\x68' | '\x69' | '\x6A' | '\x6B' | '\x6C' | '\x6D' | '\x6E' | '\x6F' | '\x70' | '\x71' | '\x72' | '\x73' | '\x74' | '\x75' | '\x76' | '\x77' | '\x78' | '\x79' | '\x7A' | '\x7B' | '\x7C' | '\x7D' | '\x7E' ;

/* num_val: "%" (bin_val | dec_val | hex_val) ; */
num_val: '%' bin_val | '%'  dec_val | '%'  hex_val ;

/* bin_val: "b" 1*BIT [ 1*("." 1*BIT) | ("_" 1*BIT) ] ; */
bin_val: 'b' bin_vals bin_valsss
bin_valsss: /*empty*/ | bin_valssss ;
bin_valssss: bin_valss | '_' bin_vals ;
bin_valss: '.' bin_vals | '.' bin_vals bin_valss ;
bin_vals: BIT | BIT bin_vals ;

/* dec_val: "d" 1*DIGIT [ 1*("." 1*DIGIT) | ("_" 1*DIGIT) ] ; */
dec_val: 'd' dec_vals dec_valsss
dec_valsss: /*empty*/ | dec_valssss ;
dec_valssss: dec_valss | '_' dec_vals ;
dec_valss: '.' dec_vals | '.' dec_vals dec_valss ;
dec_vals: DIGIT | DIGIT dec_vals ;

/* hex_val: "x" 1*HEXDIG [ 1*("." 1*HEXDIG) | ("_" 1*HEXDIG) ] ; */
hex_val: 'x' hex_vals hex_valsss
hex_valsss: /*empty*/ | hex_valssss ;
hex_valssss: hex_valss | '_' hex_vals ;
hex_valss: '.' hex_vals | '.' hex_vals hex_valss ;
hex_vals: HEXDIG | HEXDIG hex_vals ;

/* prose-val: "<" *(%x20-3D / %x3F-7E) ">" ; */
prose_val: '<' prose_vals '>' ;
prose_vals: /*empty*/ | prose_va prose_vals ;
prose_va: '\x20' | '\x21' | '\x22' | '\x23' | '\x24' | '\x25' | '\x26' | '\x27' | '\x28' | '\x29' | '\x2A' | '\x2B' | '\x2C' | '\x2D' | '\x2E' | '\x2F' | '\x30' | '\x31' | '\x32' | '\x33' | '\x34' | '\x35' | '\x36' | '\x37' | '\x38' | '\x39' | '\x3A' | '\x3B' | '\x3C' | '\x3D' | '\x3F' | '\x40' | '\x41' | '\x42' | '\x43' | '\x44' | '\x45' | '\x46' | '\x47' | '\x48' | '\x49' | '\x4A' | '\x4B' | '\x4C' | '\x4D' | '\x4E' | '\x4F' | '\x50' | '\x51' | '\x52' | '\x53' | '\x54' | '\x55' | '\x56' | '\x57' | '\x58' | '\x59' | '\x5A' | '\x5B' | '\x5C' | '\x5D' | '\x5E' | '\x5F' | '\x60' | '\x61' | '\x62' | '\x63' | '\x64' | '\x65' | '\x66' | '\x67' | '\x68' | '\x69' | '\x6A' | '\x6B' | '\x6C' | '\x6D' | '\x6E' | '\x6F' | '\x70' | '\x71' | '\x72' | '\x73' | '\x74' | '\x75' | '\x76' | '\x77' | '\x78' | '\x79' | '\x7A' | '\x7B' | '\x7C' | '\x7D' | '\x7E' ;

CRLF: CR LF ;

CR: '\x0D' ;

LF: '\x0A' ;

HEXDIG: DIGIT | 'A' | 'B' | 'C' | 'D' | 'E' | 'F' ;

ALPHA: '\x41' | '\x42' | '\x43' | '\x44' | '\x45' | '\x46' | '\x47' | '\x48' | '\x49' | '\x4A' | '\x4B' | '\x4C' | '\x4D' | '\x4E' | '\x4F' | '\x50' | '\x51' | '\x52' | '\x53' | '\x54' | '\x55' | '\x56' | '\x57' | '\x58' | '\x59' | '\x5A' | '\x61' | '\x62' | '\x63' | '\x64' | '\x65' | '\x66' | '\x67' | '\x68' | '\x69' | '\x6A' | '\x6B' | '\x6C' | '\x6D' | '\x6E' | '\x6F' | '\x70' | '\x71' | '\x72' | '\x73' | '\x74' | '\x75' | '\x76' | '\x77' | '\x78' | '\x79' | '\x7A' ;

BIT: '0' | '1' ;

WSP: SP | HTAB ;

SP: '\x20' ;

HTAB: '\x09' ;

DIGIT: '\x30' | '\x31' | '\x32' | '\x33' | '\x34' | '\x35' | '\x36' | '\x37' | '\x38' | '\x39' ;

VCHAR: '\x21' | '\x22' | '\x23' | '\x24' | '\x25' | '\x26' | '\x27' | '\x28' | '\x29' | '\x2A' | '\x2B' | '\x2C' | '\x2D' | '\x2E' | '\x2F' | '\x30' | '\x31' | '\x32' | '\x33' | '\x34' | '\x35' | '\x36' | '\x37' | '\x38' | '\x39' | '\x3A' | '\x3B' | '\x3C' | '\x3D' | '\x3E' | '\x3F' | '\x40' | '\x41' | '\x42' | '\x43' | '\x44' | '\x45' | '\x46' | '\x47' | '\x48' | '\x49' | '\x4A' | '\x4B' | '\x4C' | '\x4D' | '\x4E' | '\x4F' | '\x50' | '\x51' | '\x52' | '\x53' | '\x54' | '\x55' | '\x56' | '\x57' | '\x58' | '\x59' | '\x5A' | '\x5B' | '\x5C' | '\x5D' | '\x5E' | '\x5F' | '\x60' | '\x61' | '\x62' | '\x63' | '\x64' | '\x65' | '\x66' | '\x67' | '\x68' | '\x69' | '\x6A' | '\x6B' | '\x6C' | '\x6D' | '\x6E' | '\x6F' | '\x70' | '\x71' | '\x72' | '\x73' | '\x74' | '\x75' | '\x76' | '\x77' | '\x78' | '\x79' | '\x7A' | '\x7B' | '\x7C' | '\x7D' | '\x7E' ;

DQUOTE: '\x22' ;

%%
