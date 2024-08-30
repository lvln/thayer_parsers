%{
  #define YYDEBUG 1
  int yylex(void);
  void yyerror(char *s);
%}
%token X00
%%
P : stleUS | stleS | ttleUS | ttleS | sfleUS | sfleS | stbeUS | stbeS | ttbeUS | ttbeS | sfbeUS | sfbeS ;

stleUS :  '\xb0' '\x04' ;

stleS :  '\x50' '\xfb' ;

ttleUS :  '\x14' '\x05' X00 X00  ;

ttleS :  '\xec' '\xfa' '\xff' '\xff' ;

sfleUS :  '\x03' X00 X00 X00 X00 X00 X00 X00  ;

sfleS :  '\x01' X00 X00 X00 X00 X00 X00 X00  ;

stbeUS : '\x04' '\xb0' ;

stbeS : '\xfb' '\x50' ;

ttbeUS : X00 X00 '\x05' '\x14' ;

ttbeS : '\xff' '\xff' '\xfa' '\xec' ;

sfbeUS : X00 X00 X00 X00 X00 X00 X00 '\x03' ;

sfbeS : X00 X00 X00 X00 X00 X00 X00 '\x01' ;
