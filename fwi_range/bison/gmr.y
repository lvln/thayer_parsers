%{
  #define YYDEBUG 1
  int yylex(void);
  void yyerror(char *s);
%}
%token X00
%%
P : zerotothree | twototen | onetothree | tentofifteen ;

zerotothree :       uir16le__0 ;

twototen :     ir64be__0 ;

onetothree :       ir32le__0 ;

tentofifteen :     uir8be__0 ;
/* uint16 Little Endian Range Expansions */
uir16le__0 : 
  X00 X00  | '\x01' X00  | '\x02' X00  | '\x03' X00 ;

/* int32 Little Endian Range Expansions */
ir32le__0 : 
  '\xff' '\xff' '\xff' '\xff' | X00 X00 X00 X00  | '\x01' X00 X00 X00  | '\x02' X00 X00 X00  | '\x03' X00 X00 X00 ;

/* uint8 Big Endian Range Expansions */
uir8be__0 : 
  '\x0a' | '\x0b' | '\x0c' | '\x0d' | '\x0e' | '\x0f';

/* int64 Big Endian Range Expansions */
ir64be__0 : 
  '\xff' '\xff' '\xff' '\xff' '\xff' '\xff' '\xff' '\xfe' | '\xff' '\xff' '\xff' '\xff' '\xff' '\xff' '\xff' '\xff' | X00 X00 X00 X00 X00 X00 X00 X00  | X00 X00 X00 X00 X00 X00 X00 '\x01' | X00 X00 X00 X00 X00 X00 X00 '\x02' | X00 X00 X00 X00 X00 X00 X00 '\x03' | X00 X00 X00 X00 X00 X00 X00 '\x04' | X00 X00 X00 X00 X00 X00 X00 '\x05' | 
  X00 X00 X00 X00 X00 X00 X00 '\x06' | X00 X00 X00 X00 X00 X00 X00 '\x07' | X00 X00 X00 X00 X00 X00 X00 '\x08' | X00 X00 X00 X00 X00 X00 X00 '\x09' | X00 X00 X00 X00 X00 X00 X00 '\x0a';
