%{
  #define YYDEBUG 1
  int yylex(void);
  void yyerror(char *s);
%}
%token X00
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
  '\xc2' | '\xc3' | '\xc4' | '\xc5' | '\xc6' | '\xc7' | '\xc8' | '\xc9' | 
  '\xca' | '\xcb' | '\xcc' | '\xcd' | '\xce' | '\xcf' | '\xd0' | '\xd1' | 
  '\xd2' | '\xd3' | '\xd4' | '\xd5' | '\xd6' | '\xd7' | '\xd8' | '\xd9' | 
  '\xda' | '\xdb' | '\xdc' | '\xdd' | '\xde' | '\xdf' ;
r__2 : 
  '\xe0' | '\xe1' | '\xe2' | '\xe3' | '\xe4' | '\xe5' | '\xe6' | '\xe7' | 
  '\xe8' | '\xe9' | '\xea' | '\xeb' | '\xec' | '\xed' | '\xee' | '\xef' ;
r__3 : 
  '\xf0' | '\xf1' | '\xf2' | '\xf3' | '\xf4' | '\xf5' | '\xf6' | '\xf7' | 
  '\xf8' | '\xf9' | '\xfa' | '\xfb' | '\xfc' | '\xfd' | '\xfe' | '\xff' ;
r__4 : 
  '\x80' | '\x81' | '\x82' | '\x83' | '\x84' | '\x85' | '\x86' | '\x87' | 
  '\x88' | '\x89' | '\x8a' | '\x8b' | '\x8c' | '\x8d' | '\x8e' | '\x8f' | 
  '\x90' | '\x91' | '\x92' | '\x93' | '\x94' | '\x95' | '\x96' | '\x97' | 
  '\x98' | '\x99' | '\x9a' | '\x9b' | '\x9c' | '\x9d' | '\x9e' | '\x9f' | 
  '\xa0' | '\xa1' | '\xa2' | '\xa3' | '\xa4' | '\xa5' | '\xa6' | '\xa7' | 
  '\xa8' | '\xa9' | '\xaa' | '\xab' | '\xac' | '\xad' | '\xae' | '\xaf' | 
  '\xb0' | '\xb1' | '\xb2' | '\xb3' | '\xb4' | '\xb5' | '\xb6' | '\xb7' | 
  '\xb8' | '\xb9' | '\xba' | '\xbb' | '\xbc' | '\xbd' | '\xbe' | '\xbf' ;
r__5 : 
  '\x31' | '\x32' | '\x33' | '\x34' | '\x35' | '\x36' | '\x37' | '\x38' | 
  '\x39' ;
r__6 : 
  '\x23' | '\x24' | '\x25' | '\x26' | '\x27' | '\x28' | '\x29' | '\x2a' | 
  '\x2b' | '\x2c' | '\x2d' | '\x2e' | '\x2f' ;
r__7 : 
  '\x3a' | '\x3b' | '\x3c' | '\x3d' | '\x3e' | '\x3f' | '\x40' ;
r__8 : 
  '\x5d' | '\x5e' | '\x5f' | '\x60' ;
r__9 : 
  '\x7b' | '\x7c' | '\x7d' | '\x7e' | '\x7f' ;
r__10 : 
  '\x67' | '\x68' | '\x69' | '\x6a' | '\x6b' | '\x6c' | '\x6d' | '\x6e' | 
  '\x6f' | '\x70' | '\x71' | '\x72' | '\x73' | '\x74' | '\x75' | '\x76' | 
  '\x77' | '\x78' | '\x79' | '\x7a' ;
r__11 : 
  '\x47' | '\x48' | '\x49' | '\x4a' | '\x4b' | '\x4c' | '\x4d' | '\x4e' | 
  '\x4f' | '\x50' | '\x51' | '\x52' | '\x53' | '\x54' | '\x55' | '\x56' | 
  '\x57' | '\x58' | '\x59' | '\x5a' ;
r__12 : 
  '\x41' | '\x42' | '\x43' | '\x44' | '\x45' | '\x46' ;
r__13 : 
  '\x61' | '\x62' | '\x63' | '\x64' | '\x65' | '\x66' ;

/* String Expansions */
s__0 : 
 't' 'r' 'u' 'e' ;
s__1 : 
 'f' 'a' 'l' 's' 'e' ;
s__2 : 
 'n' 'u' 'l' 'l' ;
