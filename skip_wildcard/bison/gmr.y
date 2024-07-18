%{
  #include <stdint.h>
	#define YYDEBUG 1
	extern int yylex(void);
	void yyerror(char *s);
%}

%token X00
												
%%
P : /*empty */ | WC ;

WC : { uint8_t c = yylex(); if (c == 0) YYABORT; } ;

%%
