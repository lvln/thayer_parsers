%{
	#define YYDEBUG 1
	int yylex(void);
	void yyerror(char *s);
%}

%token X00


%% /* The JSON Grammar definition -- no actions are performed */

number: integer fraction exponent ;

integer: digit | onenine digits | '-' digit | '-' onenine digits ;

fraction: /* empty */ | '.' digits ;

exponent: /* empty */ | 'E' sign digits | 'e' sign digits ;

sign: /* empty */ | '+' | '-' ;

digits: digit | digits digit ;

digit: '0' | onenine ;

onenine: '1' | '2' | '3' | '4' | '5'| '6' | '7' | '8' | '9' ;

%% /* C-Code */
