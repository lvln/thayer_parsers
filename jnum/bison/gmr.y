%{
  #define YYDEBUG 1
  int yylex(void);
  void yyerror(char *s);
%}
%token X00
%%
number: integer fraction exponent ;

integer: digit | onenine digits | '-' digit | '-' onenine digits ;

fraction: /* empty */ | '.' digits ;

exponent: /* empty */ | 'E' sign digits | 'e' sign digits ;

sign: /* empty */ | '+' | '-' ;

digits: digit | digits digit ;

digit: '0' | onenine ;

onenine:     r__1 ; 
/* Range Expansions */
r__1 : 
  '\x31' | '\x32' | '\x33' | '\x34' | '\x35' | '\x36' | '\x37' | '\x38' | 
  '\x39' ;
