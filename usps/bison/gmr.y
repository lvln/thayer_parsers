%{
  #define YYDEBUG 1
  int yylex(void);
  void yyerror(char *s);
%}
%token X00
%%
rule: sentence CRLF address CRLF sentence multi_digits ;

address: multi_digits sentence multi_digits | multi_digits sentence SP '#' multi_digits ;

multi_digits: /* empty */ | multi_digit WSP | WSP multi_digit ;

multi_digit: DIGIT | multi_digit DIGIT ;

sentence: word | sentence WSP word ;

word: ALPHA | word ALPHA ;

ALPHA:     r__1 |     r__2 ;

DIGIT:     r__3 ;

WSP: WSPS | WSP WSPS ;

WSPS: SP | HTAB ;

HTAB: '\t' ;

SP: ' ' ;

CRLF: CR | LF ;

CR: '\r' ;

LF: '\n' ;
/* Range Expansions */
r__1 : 
  '\x61' | '\x62' | '\x63' | '\x64' | '\x65' | '\x66' | '\x67' | '\x68' | 
  '\x69' | '\x6a' | '\x6b' | '\x6c' | '\x6d' | '\x6e' | '\x6f' | '\x70' | 
  '\x71' | '\x72' | '\x73' | '\x74' | '\x75' | '\x76' | '\x77' | '\x78' | 
  '\x79' | '\x7a' ;
r__2 : 
  '\x41' | '\x42' | '\x43' | '\x44' | '\x45' | '\x46' | '\x47' | '\x48' | 
  '\x49' | '\x4a' | '\x4b' | '\x4c' | '\x4d' | '\x4e' | '\x4f' | '\x50' | 
  '\x51' | '\x52' | '\x53' | '\x54' | '\x55' | '\x56' | '\x57' | '\x58' | 
  '\x59' | '\x5a' ;
r__3 : 
  '\x30' | '\x31' | '\x32' | '\x33' | '\x34' | '\x35' | '\x36' | '\x37' | 
  '\x38' | '\x39' ;
