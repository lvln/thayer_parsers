%{
	#define YYDEBUG 1
	int yylex(void);
	void yyerror(char *s);
%}

%token X00

%%
p : 'a' WC 'c' { printf("%c %c %c\n", $1, $2, $3); } ;

WC : { int c = yylex(); printf("c = %c\n", c); if (c == 0) YYABORT; } ;

%%
