%{
	#define YYDEBUG 1
	int yylex(void);
	void yyerror(char *s);
%}

%token X00
																		
%%
p : WC | p WC ;

WC : { yylex(); } ;

%%
