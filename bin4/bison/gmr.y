%{
	#define YYDEBUG 1
	int yylex(void);
	void yyerror(char *s);
%}

%token X00

%%
// Top-level rule
P : bytes ;

bytes : 
    WC 
    | bytes WC
    ;

// Consume byte for wildcard
WC : { int c = yylex(); if (c == 0) YYABORT; } ;

%%
