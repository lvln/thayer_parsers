%{
	#define YYDEBUG 1
	int yylex(void);
	void yyerror(char *s);
%}

%token X00

%%

/* Here is the grammar */
/* Here is the grammar */
P : X00 '\x22' '\x5d' ;

%%

