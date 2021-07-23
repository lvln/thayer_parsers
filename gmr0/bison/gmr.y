%{
	#define YYDEBUG 1
	int yylex(void);
	void yyerror(char *s);
%}

%token X00

%%

/* Here is the grammar */

P : 'a'  | '(' M ')' ;

M : /* nothing */   | P ;
%%
