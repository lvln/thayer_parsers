%{
  #define YYDEBUG 1
  extern int yylex(void);
  void yyerror(char *s);
%}
%token X00
%token BYTE
%%

P : 'a' | '(' M ')' ;

M : /* nothing */ | P ;


