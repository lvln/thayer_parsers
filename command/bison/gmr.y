%{
  #define YYDEBUG 1
  int yylex(void);
  void yyerror(char *s);
%}
%token X00
%%
/*
 * command parser
 */
P : AOR | EZ | T ;			/* three tyes of command */
	
AOR : '\x10' C C any ;    /* aor, coord, coord, msgid*/

EZ : '\x20' C C any ;     /* ez, coord, coord , msgid */

T : '\x30' C W any ;      /* target coord, wpnid msgid*/

W : '\x01' | '\x02' ;   /* wpnid */

C : LAT D D LONG D D ;  /* lat min sec long min sec */
 
D : fr0_60 ;       /* any uint8 0-60, represents min and sec in degrees */

LAT : fr166_255 | fr0_90 ;   /* from -90 to 90 */	

LONG : NEG | POS ;              /* long -180 to 180 */

POS: fr0_180 X00 ;      /* equivalent to int16 0 to 180 */

NEG:  fr76_255 '\xff' ; /* -180 to -1 using 2's complement */


any: fr0_60 | fr61_75 | fr76_255 ;

fr0_180: fr0_90 | fr91_165 | fr166_180 ; /* uint8 0-180*/

fr0_90: fr0_60 | fr61_75 | fr76_90 ;

fr76_255: fr76_90 | fr91_165 | fr166_255 ; /*2's complement of -180 to -1 */

fr166_255: fr166_180 | fr181_255 ; /*2's complement of -90 to -1 */


fr181_255: uir8be__0 ;

fr166_180: uir8be__1 ;

fr91_165: uir8be__2 ;

fr76_90: uir8be__3 ;

fr61_75:    uir8be__4 ;

fr0_60:    uir8be__5 ; 
/* uint8 Big Endian Range Expansions */
uir8be__0 : 
  '\xb5' | '\xb6' | '\xb7' | '\xb8' | '\xb9' | '\xba' | '\xbb' | '\xbc' | 
  '\xbd' | '\xbe' | '\xbf' | '\xc0' | '\xc1' | '\xc2' | '\xc3' | '\xc4' | 
  '\xc5' | '\xc6' | '\xc7' | '\xc8' | '\xc9' | '\xca' | '\xcb' | '\xcc' | 
  '\xcd' | '\xce' | '\xcf' | '\xd0' | '\xd1' | '\xd2' | '\xd3' | '\xd4' | 
  '\xd5' | '\xd6' | '\xd7' | '\xd8' | '\xd9' | '\xda' | '\xdb' | '\xdc' | 
  '\xdd' | '\xde' | '\xdf' | '\xe0' | '\xe1' | '\xe2' | '\xe3' | '\xe4' | 
  '\xe5' | '\xe6' | '\xe7' | '\xe8' | '\xe9' | '\xea' | '\xeb' | '\xec' | 
  '\xed' | '\xee' | '\xef' | '\xf0' | '\xf1' | '\xf2' | '\xf3' | '\xf4' | 
  '\xf5' | '\xf6' | '\xf7' | '\xf8' | '\xf9' | '\xfa' | '\xfb' | '\xfc' | 
  '\xfd' | '\xfe' | '\xff';
uir8be__1 : 
  '\xa6' | '\xa7' | '\xa8' | '\xa9' | '\xaa' | '\xab' | '\xac' | '\xad' | 
  '\xae' | '\xaf' | '\xb0' | '\xb1' | '\xb2' | '\xb3' | '\xb4';
uir8be__2 : 
  '\x5b' | '\x5c' | '\x5d' | '\x5e' | '\x5f' | '\x60' | '\x61' | '\x62' | 
  '\x63' | '\x64' | '\x65' | '\x66' | '\x67' | '\x68' | '\x69' | '\x6a' | 
  '\x6b' | '\x6c' | '\x6d' | '\x6e' | '\x6f' | '\x70' | '\x71' | '\x72' | 
  '\x73' | '\x74' | '\x75' | '\x76' | '\x77' | '\x78' | '\x79' | '\x7a' | 
  '\x7b' | '\x7c' | '\x7d' | '\x7e' | '\x7f' | '\x80' | '\x81' | '\x82' | 
  '\x83' | '\x84' | '\x85' | '\x86' | '\x87' | '\x88' | '\x89' | '\x8a' | 
  '\x8b' | '\x8c' | '\x8d' | '\x8e' | '\x8f' | '\x90' | '\x91' | '\x92' | 
  '\x93' | '\x94' | '\x95' | '\x96' | '\x97' | '\x98' | '\x99' | '\x9a' | 
  '\x9b' | '\x9c' | '\x9d' | '\x9e' | '\x9f' | '\xa0' | '\xa1' | '\xa2' | 
  '\xa3' | '\xa4' | '\xa5';
uir8be__3 : 
  '\x4c' | '\x4d' | '\x4e' | '\x4f' | '\x50' | '\x51' | '\x52' | '\x53' | 
  '\x54' | '\x55' | '\x56' | '\x57' | '\x58' | '\x59' | '\x5a';
uir8be__4 : 
  '\x3d' | '\x3e' | '\x3f' | '\x40' | '\x41' | '\x42' | '\x43' | '\x44' | 
  '\x45' | '\x46' | '\x47' | '\x48' | '\x49' | '\x4a' | '\x4b';
uir8be__5 : 
  X00  | '\x01' | '\x02' | '\x03' | '\x04' | '\x05' | '\x06' | '\x07' | 
  '\x08' | '\x09' | '\x0a' | '\x0b' | '\x0c' | '\x0d' | '\x0e' | '\x0f' | 
  '\x10' | '\x11' | '\x12' | '\x13' | '\x14' | '\x15' | '\x16' | '\x17' | 
  '\x18' | '\x19' | '\x1a' | '\x1b' | '\x1c' | '\x1d' | '\x1e' | '\x1f' | 
  '\x20' | '\x21' | '\x22' | '\x23' | '\x24' | '\x25' | '\x26' | '\x27' | 
  '\x28' | '\x29' | '\x2a' | '\x2b' | '\x2c' | '\x2d' | '\x2e' | '\x2f' | 
  '\x30' | '\x31' | '\x32' | '\x33' | '\x34' | '\x35' | '\x36' | '\x37' | 
  '\x38' | '\x39' | '\x3a' | '\x3b' | '\x3c';
