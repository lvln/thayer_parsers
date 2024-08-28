%{
  #define YYDEBUG 1
  int yylex(void);
  void yyerror(char *s);
%}
%token X00
%%

P : 'a' | '(' M ')' ;

M : /* nothing */ | P ;


