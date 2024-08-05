%{
  #define YYDEBUG 1
  extern int yylex(void);
  void yyerror(char *s);
%}
%token X00
%%
P : optMessage ;

optMessage : /* empty */ | message optMessage ;

message : WC3 A WC WC ;

WC3 : WC WC WC ;

WC : { int c = yylex(); if (c == 0) YYABORT; } ;

A : 'a' ;
