%{
  #define YYDEBUG 1
  extern int yylex(void);
  void yyerror(char *s);
%}
%token X00
%token BYTE
%%
/* Here is the grammar */
P : optMessage ;

optMessage : /* empty */ | optMessage message ;

message : WC A WC 'c' ;

A : 'a' | WC ;
/* Consume byte for wildcard */
WC : BYTE ;
