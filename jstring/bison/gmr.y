%{
	#define YYDEBUG 1
	int yylex(void);
	void yyerror(char *s);
%}

%token X00

%% /* The JSON Grammar definition -- no actions are performed */


STRING: '"' chars '"' ;

chars: /* empty */ | chars CHAR ;

CHAR: uchar | lchar | digit | punct | '\\' escchar ;

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

digit: '0' | onenine ;

onenine: '1' | '2' | '3' | '4' | '5'| '6' | '7' | '8' | '9' ;   

%% /* C-Code */
