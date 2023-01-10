%{
	#define YYDEBUG 1
	int yylex(void);
	void yyerror(char *s);
%}

%token X00


%% /* The ABNF HTTP Grammar definition -- TODO */


%% /* C-Code */
