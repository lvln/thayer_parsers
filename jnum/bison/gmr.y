%{
	#define YYDEBUG 1
	int yylex(void);
	void yyerror(char *s);
%}

%token X00


%% /* The JSON Grammar definition -- no actions are performed */

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

CHAR: uchar | lchar | digit | punct | '\\' escchar ;

number: integer fraction exponent ;

integer: digit | onenine digits | '-' digit | '-' onenine digits ;

fraction: /* empty */ | '.' digits ;

exponent: /* empty */ | 'E' sign digits | 'e' sign digits ;

sign: /* empty */ | '+' | '-' ;

digits: digit | digits digit ;

digit: '0' | onenine ;

onenine: '1' | '2' | '3' | '4' | '5'| '6' | '7' | '8' | '9' ;

ws: /* empty */	| ws ' ' | ws '\n' | ws '\t' | ws '\r' ;

escchar: 'b' | 'f' | 'n' | 'r' | 't' | '"' | '\\' | '/' | 'u' hex hex hex hex ;

punct:  ' ' | '!' | '#' | '$' | '%' | '&' | '\'' | '(' | ')' | '*' | '+' 
      | ',' | '-' | '.' | '/' | ':' | ';' | '<' | '=' | '>' | '?' | '@'
      | '[' | ']' | '^' | '_' | '`' | '{' | '|' | '}' | '~' ;

lchar: lhex | 'g' | 'h' | 'i' | 'j' 
      | 'k' | 'l' | 'm' | 'n' | 'o' | 'p' | 'q' | 'r' | 's' | 't' 
      | 'u' | 'v' | 'w' | 'x' | 'y' | 'z' ;

uchar: uhex | 'G' | 'H' | 'I' | 'J' 
      | 'K' | 'L' | 'M' | 'N' | 'O' | 'P' | 'Q' | 'R' | 'S' | 'T' 
      | 'U' | 'V' | 'W' | 'X' | 'Y' | 'Z' ;

uhex: 'A' | 'B' | 'C' | 'D' | 'E' | 'F' ;

lhex: 'a'| 'b' | 'c' | 'd' | 'e' | 'f' ;

hex: digit | uhex | lhex ;

TRUE: 't' 'r' 'u' 'e' ;

FALSE: 'f' 'a' 'l' 's' 'e' ;

NULL: 'n' 'u' 'l' 'l' ;

%% /* C-Code */
