%{
	#define YYDEBUG 1
	int yylex(void);
	void yyerror(char *s);
%}

%token X00

%%
p : WC ;

WC : { int c = yylex(); if (c == 0) YYABORT; } ;

%%
