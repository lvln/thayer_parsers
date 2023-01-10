%{
	#define YYDEBUG 1
	int yylex(void);
	void yyerror(char *s);
%}

%token X00

%% /* The ABNF Grammar */

ALPHA: 'a'| 'b' | 'c' | 'd' | 'e' | 'f'  | 'g' | 'h' | 'i' | 'j'
		| 'k' | 'l' | 'm' | 'n' | 'o' | 'p' | 'q' | 'r' | 's' | 't'
		| 'u' | 'v' | 'w' | 'x' | 'y' | 'z' |'A' | 'B' | 'C' | 'D'
		| 'E' | 'F' | 'G' | 'H' | 'I' | 'J' | 'K' | 'L' | 'M' | 'N'
		| 'O' | 'P' | 'Q' | 'R' | 'S' | 'T' | 'U' | 'V' | 'W' | 'X'
		| 'Y' | 'Z' ;

BIT: '0' | '1' ;

CHAR: ALPHA | CR | CTL | DIGIT | DQUOTE | HTAB | LF | VCHAR ;

CR: '\r' ;

CRLF: CR LF ;

CTL: '\a' | '\b' | '\e' | '\f' | '\v' | '\\' ;

DIGIT: '0' | '1' | '2' | '3' | '4' | '5'| '6' | '7' | '8' | '9' ;

DQUOTE: '\"' ;

HEXDIG: DIGIT | 'A' | 'B' | 'C' | 'D' | 'E' | 'F' ;

HTAB: '\t' ;

LF: '\n' ;

LWSP: WSP | CRLF WSP ;

OCTET: BIT BIT BIT BIT BIT BIT BIT BIT ;

SP: ' ' ;

VCHAR: '!' | '#' | '$' | '%' | '&' | '\'' | '(' | ')' | '*' | '+'
		| ',' | '-' | '.' | '/' | ':' | ';' | '<' | '=' | '>' | '\?' | '@' | '['
		| '\\\\' | '^' | '_' | ']' | '`' | '{' | '}' | '+' | '~' ;

WSP: SP | HTAB ;

%% /* C-Code */