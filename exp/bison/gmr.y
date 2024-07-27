%{
  #define YYDEBUG 1
  extern int yylex(void);
  void yyerror(char *s);
%}
%token X00
%%
P : optMessage ;

optMessage : /* empty */ | optMessage message ;

message : WC3 A WC WC ;

WC3 : WC WC WC ;

A : 'a' ;
/* Consume byte for wildcard */
WC : { int c = yylex(); if (c == 0) YYABORT; } ;
