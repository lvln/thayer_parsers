%{
	#define YYDEBUG 1
	int yylex(void);
	void yyerror(char *s);
%}

%token X00


%% /* The ABNF USPS Grammar definition -- TODO */


%% /* C-Code */
