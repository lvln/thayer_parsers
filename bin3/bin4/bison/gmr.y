%{
  #define YYDEBUG 1
  extern int yylex(void);
  void yyerror(char *s);
%}
%token X00
%%
/* Here is the grammar */
P : WC ;

/* Consume byte for wildcard */
WC : { int c = yylex(); if (c == 0) YYABORT; } ;
