%{
	#define YYDEBUG 1
	int yylex(void);
	void yyerror(char *s);
%}

%token X00

%%

json: element ;                                                                                                      
                                                                                                                     
value: object | array | STRING | number | TRUE | FALSE | NULL ;                                                      
                                                                                                                     
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

twobyte: c2d | eight2b ;

c2d: xc | xd ;

threebyte: xe eight2b eight2b ;

fourbyte: xf eight2b eight2b ;

eight2b: x8 | x9 | xa | xb ; //range x80 -> xBF 

number: integer fraction exponent ;                                                                                  
                                                                                                                     
integer: digit | onenine digits | '-' digit | '-' onenine digits ;                                                   
                                                                                                                     
fraction: /* empty */ | '.' digits ;                                                                                 
                                                                                                                     
exponent: /* empty */ | 'E' sign digits | 'e' sign digits ;                                                          
                                                                                                                     
sign: /* empty */ | '+' | '-' ;                                                                                      
                                                                                                                     
digits: digit | digits digit ;                                                                                       
                                                                                                                     
digit: '0' | onenine ;

onenine: '1' | '2' | '3' | '4' | '5'| '6' | '7' | '8' | '9' ;

ws: /* empty */ | ws ' ' | ws '\n' | ws '\t' | ws '\r' ;                                                             
                                                                                                                     
escchar: 'b' | 'f' | 'n' | 'r' | 't' | '"' | '\\' | '/' | 'u' hex hex hex hex ;

punct:  ' ' | '!' | '#' | '$' | '%' | '&' | '\''|'(' | ')' | '*' | '+'
      | ',' | '-' | '.' | ':' | ';' | '<' | '=' | '>' | '?' | '@'
      | '[' | '/' | ']' | '^' | '_' | '`' | '{' | '|' | '}' | '~' | '\x7f' ;

lchar: lhex | 'g' | 'h' | 'i' | 'j'                                                                                    
      | 'k' | 'l' | 'm' | 'n' | 'o' | 'p' | 'q' | 'r' | 's' | 't'                                                    
      | 'u' | 'v' | 'w' | 'x' | 'y' | 'z' ;

uchar: uhex | 'G' | 'H' | 'I' | 'J'                                                                                    
      | 'K' | 'L' | 'M' | 'N' | 'O' | 'P' | 'Q' | 'R' | 'S' | 'T'                                                    
      | 'U' | 'V' | 'W' | 'X' | 'Y' | 'Z' ;

uhex: 'A' | 'B' | 'C' | 'D' | 'E' | 'F' ;

lhex: 'a'| 'b' | 'c' | 'd' | 'e' | 'f' ; 
                                                                                                                     
hex: digit | uhex | lhex ;  

TRUE: 't' 'r' 'u' 'e'  ;

FALSE: 'f' 'a' 'l' 's' 'e' ;

NULL: 'n' 'u' 'l' 'l' ;  


x8 : '\x80'|'\x81'|'\x82'|'\x83'|'\x84'|'\x85'|'\x86'|'\x87'|'\x88'|'\x89'|'\x8a'|'\x8b'|'\x8c'|'\x8d'|'\x8e'|'\x8f' ;

x9 : '\x90'|'\x91'|'\x92'|'\x93'|'\x94'|'\x95'|'\x96'|'\x97'|'\x98'|'\x99'|'\x9a'|'\x9b'|'\x9c'|'\x9d'|'\x9e'|'\x9f' ;

xa : '\xa0'|'\xa1'|'\xa2'|'\xa3'|'\xa4'|'\xa5'|'\xa6'|'\xa7'|'\xa8'|'\xa9'|'\xaa'|'\xab'|'\xac'|'\xad'|'\xae'|'\xaf' ;

xb : '\xb0'|'\xb1'|'\xb2'|'\xb3'|'\xb4'|'\xb5'|'\xb6'|'\xb7'|'\xb8'|'\xb9'|'\xba'|'\xbb'|'\xbc'|'\xbd'|'\xbe'|'\xbf' ;

xc : '\xc2'|'\xc3'|'\xc4'|'\xc5'|'\xc6'|'\xc7'|'\xc8'|'\xc9'|'\xca'|'\xcb'|'\xcc'|'\xcd'|'\xce'|'\xcf' ;

xd : '\xd0'|'\xd1'|'\xd2'|'\xd3'|'\xd4'|'\xd5'|'\xd6'|'\xd7'|'\xd8'|'\xd9'|'\xda'|'\xdb'|'\xdc'|'\xdd'|'\xde'|'\xdf' ;

xe : '\xe0'|'\xe1'|'\xe2'|'\xe3'|'\xe4'|'\xe5'|'\xe6'|'\xe7'|'\xe8'|'\xe9'|'\xea'|'\xeb'|'\xec'|'\xed'|'\xee'|'\xef' ;

xf : '\xf0'|'\xf1'|'\xf2'|'\xf3'|'\xf4'|'\xf5'|'\xf6'|'\xf7'|'\xf8'|'\xf9'|'\xfa'|'\xfb'|'\xfc'|'\xfd'|'\xfe'|'\xff' ; 

%%
