%{
  #define YYDEBUG 1
  int yylex(void);
  void yyerror(char *s);
%}
%token X00
%%

L : L ';' E | E ;

E : E ',' P | P ;

P : 'a' | '(' M ')' ;

M : /* nothing */ | L ; 

