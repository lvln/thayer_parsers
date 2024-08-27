%{
  #define YYDEBUG 1
  extern int yylex(void);
  void yyerror(char *s);
%}
%token X00
%token BYTE
%%

L : L ';' E | E ;

E : E ',' P | P ;

P : 'a' | '(' M ')' ;

M : /* nothing */ | L ; 

