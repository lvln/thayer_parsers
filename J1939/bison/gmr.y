%{
  #define YYDEBUG 1
  extern int yylex(void);
  void yyerror(char *s);
%}
%token X00
%token BYTE
%%
can :		canSTD | canEXT ;
canSTD :	X00 any_11bit X00 X00 X00 DLC_4b_data any_15bit '\x01' '\x01' '\x01' '\x7f' ;
canEXT :	X00 any_11bit '\x01' '\x01' any_18bit X00 '\x01' '\x01' DLC_4b_data any_15bit '\x01' low_OR_high '\x01' '\x7f' ;

DLC_4b_data :	X00 | 
		'\x01' any_byte | 
		'\x02' any_byte any_byte | 
		'\x03' any_byte any_byte any_byte | 
		'\x04' any_byte any_byte any_byte any_byte | 
		'\x05' any_byte any_byte any_byte any_byte any_byte | 
		'\x06' any_byte any_byte any_byte any_byte any_byte any_byte | 
		'\x07' any_byte any_byte any_byte any_byte any_byte any_byte any_byte | 
		'\x08' any_byte any_byte any_byte any_byte any_byte any_byte any_byte any_byte ;

any_18bit :	    r__0 any_byte any_byte ;
any_15bit :	any_7bit any_byte ;
any_11bit :	    r__1 any_byte ;

any_byte :	    WC ;
any_7bit :	    r__2 ;
low_OR_high :	X00 | '\x01' ;

/* Consume byte for wildcard */
WC : BYTE ;

/* Range Expansions */
r__0 : 
  X00 | '\x01' | '\x02' | '\x03' ;
r__1 : 
  X00 | '\x01' | '\x02' | '\x03' | '\x04' | '\x05' | '\x06' | '\x07' ;
r__2 : 
  X00 | '\x01' | '\x02' | '\x03' | '\x04' | '\x05' | '\x06' | '\x07' | 
  '\x08' | '\x09' | '\x0a' | '\x0b' | '\x0c' | '\x0d' | '\x0e' | '\x0f' | 
  '\x10' | '\x11' | '\x12' | '\x13' | '\x14' | '\x15' | '\x16' | '\x17' | 
  '\x18' | '\x19' | '\x1a' | '\x1b' | '\x1c' | '\x1d' | '\x1e' | '\x1f' | 
  '\x20' | '\x21' | '\x22' | '\x23' | '\x24' | '\x25' | '\x26' | '\x27' | 
  '\x28' | '\x29' | '\x2a' | '\x2b' | '\x2c' | '\x2d' | '\x2e' | '\x2f' | 
  '\x30' | '\x31' | '\x32' | '\x33' | '\x34' | '\x35' | '\x36' | '\x37' | 
  '\x38' | '\x39' | '\x3a' | '\x3b' | '\x3c' | '\x3d' | '\x3e' | '\x3f' | 
  '\x40' | '\x41' | '\x42' | '\x43' | '\x44' | '\x45' | '\x46' | '\x47' | 
  '\x48' | '\x49' | '\x4a' | '\x4b' | '\x4c' | '\x4d' | '\x4e' | '\x4f' | 
  '\x50' | '\x51' | '\x52' | '\x53' | '\x54' | '\x55' | '\x56' | '\x57' | 
  '\x58' | '\x59' | '\x5a' | '\x5b' | '\x5c' | '\x5d' | '\x5e' | '\x5f' | 
  '\x60' | '\x61' | '\x62' | '\x63' | '\x64' | '\x65' | '\x66' | '\x67' | 
  '\x68' | '\x69' | '\x6a' | '\x6b' | '\x6c' | '\x6d' | '\x6e' | '\x6f' | 
  '\x70' | '\x71' | '\x72' | '\x73' | '\x74' | '\x75' | '\x76' | '\x77' | 
  '\x78' | '\x79' | '\x7a' | '\x7b' | '\x7c' | '\x7d' | '\x7e' | '\x7f' ;
