%{
	#define YYDEBUG 1
	int yylex(void);
	void yyerror(char *s);
%}

%token X00
																		
%%
p : WC | WC WC | WC WC WC ;

WC : { yylex(); } ;

%%
