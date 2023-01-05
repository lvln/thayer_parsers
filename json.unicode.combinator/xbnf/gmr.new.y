%{
  #define YYDEBUG 1
  int yylex(void);
  void yyerror(char *s);
%}
%token X00
%token R__1
%token R__2
%token R__3
%token R__4
%token R__5
%token R__6
%token R__7
%token R__8
%token R__9
%token R__10
%token R__11
%token R__12
%token R__13
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
r__1 : 
  R__1 ;
r__2 : 
  R__2 ;
r__3 : 
  R__3 ;
r__4 : 
  R__4 ;
r__5 : 
  R__5 ;
r__6 : 
  R__6 ;
r__7 : 
  R__7 ;
r__8 : 
  R__8 ;
r__9 : 
  R__9 ;
r__10 : 
  R__10 ;
r__11 : 
  R__11 ;
r__12 : 
  R__12 ;
r__13 : 
  R__13 ;

/* String Expansions */
s__0 : 
 't' 'r' 'u' 'e' ;
s__1 : 
 'f' 'a' 'l' 's' 'e' ;
s__2 : 
 'n' 'u' 'l' 'l' ;
