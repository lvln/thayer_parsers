%{
  #define YYDEBUG 1
  extern int yylex(void);
  void yyerror(char *s);
%}
%token X00
%token BYTE
%%
/* Here is the grammar */
P : rule1 rule2 ;

rule1 : WC 'a' WC ;

rule2 : /* empty */ | WC ;
/* Consume byte for wildcard */
WC : BYTE ;
