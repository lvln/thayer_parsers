%{
  #define YYDEBUG 1
  int yylex(void);
  void yyerror(char *s);
%}
%token X00 258
%token R_xC2xDF 500
%token R_xE0xEF 501
%token R_xF0xFF 502 
%token R_x80xBF 503
%token R_x31x39 504
%token R_x23x2F 505
%token R_x3Ax40 506
%token R_x5Dx60 507
%token R_x7Bx7F 508
%token R_x67x7A 509
%token R_x47x5A 510 
%token R_x41x46 511
%token R_x61x66 512 


%%
json: element ;

value: object | array | STRING | number | s__0 | s__1 | s__2 ;

object: '{' ws '}' | '{' members '}' ;

members: member | members ',' member ;
member: ws STRING ws ':' element ;

array: '[' ws ']' | '[' elements ']' ;

elements: element | elements ',' element ;
element: ws value ws ;

STRING: '"' chars '"' ;
chars: /* empty */ | chars CHAR ;
CHAR: uchar | lchar | digit | unicodechar | punct | '\\' escchar ;

unicodechar: twobyte | threebyte | fourbyte ;
twobyte: r__1 r ;
threebyte:  r__2 r r ;
fourbyte: r__3 r r r ;
r: r__4 ;  

number: integer fraction exponent ;
integer: digit | onenine digits | '-' digit | '-' onenine digits ;
fraction: /* empty */ | '.' digits ;
exponent: /* empty */ | 'E' sign digits | 'e' sign digits ;
sign: /* empty */ | '+' | '-' ;
digits: digit | digits digit ;
digit: '0' | onenine ;
onenine: r__5 ;

ws: /* empty */ | ws ' ' | ws '\n' | ws '\t' | ws '\r' ;

escchar: 'b' | 'f' | 'n' | 'r' | 't' | '"' | '\\' | '/' | 'u' hex hex hex hex ;
punct: ' ' | '!' | r__6 | r__7 | '[' | r__8 | r__9 ;
lchar: lhex |  r__10 ;
uchar: uhex | r__11 ;
uhex: r__12 ;
lhex: r__13 ; 
hex: digit | uhex | lhex ;  


/* Range Expansions */
r__1 : R_xC2xDF ;
r__2 : R_xE0xEF ;
r__3 : R_xF0xFF ; 
r__4 : R_x80xBF ;
r__5 : R_x31x39 ;
r__6 : R_x23x2F ;
r__7 : R_x3Ax40 ;
r__8 : R_x5Dx60 ;
r__9 : R_x7Bx7F ;
r__10 : R_x67x7A ;
r__11 : R_x47x5A ; 
r__12 : R_x41x46 ;
r__13 : R_x61x66 ; 

/* String Expansions */
s__0 : 
 't' 'r' 'u' 'e' ;
s__1 : 
 'f' 'a' 'l' 's' 'e' ;
s__2 : 
 'n' 'u' 'l' 'l' ;
