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
	
AOR : '\x10' C C r__0 ;    /* aor, coord, coord, msgid*/

EZ : '\x20' C C r__0 ;     /* ez, coord, coord , msgid */

T : '\x30' C W r__0 ;      /* target coord, wpnid msgid*/

W : '\x01' | '\x02' ;   /* wpnid */

C : LAT D D LONG D D ;  /* lat min sec long min sec */

D :     r__1 ;       /* any uint8 0-60, represents min and sec in degrees */

LAT :   r__2 ;   /* from 0 to 90 */	

LONG :    rr__0 ;      /* long -180 to 180 */


	


/* Range Expansions */
r__0 : 
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
  '\x78' | '\x79' | '\x7a' | '\x7b' | '\x7c' | '\x7d' | '\x7e' | '\x7f' | 
  '\x80' | '\x81' | '\x82' | '\x83' | '\x84' | '\x85' | '\x86' | '\x87' | 
  '\x88' | '\x89' | '\x8a' | '\x8b' | '\x8c' | '\x8d' | '\x8e' | '\x8f' | 
  '\x90' | '\x91' | '\x92' | '\x93' | '\x94' | '\x95' | '\x96' | '\x97' | 
  '\x98' | '\x99' | '\x9a' | '\x9b' | '\x9c' | '\x9d' | '\x9e' | '\x9f' | 
  '\xa0' | '\xa1' | '\xa2' | '\xa3' | '\xa4' | '\xa5' | '\xa6' | '\xa7' | 
  '\xa8' | '\xa9' | '\xaa' | '\xab' | '\xac' | '\xad' | '\xae' | '\xaf' | 
  '\xb0' | '\xb1' | '\xb2' | '\xb3' | '\xb4' | '\xb5' | '\xb6' | '\xb7' | 
  '\xb8' | '\xb9' | '\xba' | '\xbb' | '\xbc' | '\xbd' | '\xbe' | '\xbf' | 
  '\xc0' | '\xc1' | '\xc2' | '\xc3' | '\xc4' | '\xc5' | '\xc6' | '\xc7' | 
  '\xc8' | '\xc9' | '\xca' | '\xcb' | '\xcc' | '\xcd' | '\xce' | '\xcf' | 
  '\xd0' | '\xd1' | '\xd2' | '\xd3' | '\xd4' | '\xd5' | '\xd6' | '\xd7' | 
  '\xd8' | '\xd9' | '\xda' | '\xdb' | '\xdc' | '\xdd' | '\xde' | '\xdf' | 
  '\xe0' | '\xe1' | '\xe2' | '\xe3' | '\xe4' | '\xe5' | '\xe6' | '\xe7' | 
  '\xe8' | '\xe9' | '\xea' | '\xeb' | '\xec' | '\xed' | '\xee' | '\xef' | 
  '\xf0' | '\xf1' | '\xf2' | '\xf3' | '\xf4' | '\xf5' | '\xf6' | '\xf7' | 
  '\xf8' | '\xf9' | '\xfa' | '\xfb' | '\xfc' | '\xfd' | '\xfe' | '\xff' ;
r__1 : 
  X00 | '\x01' | '\x02' | '\x03' | '\x04' | '\x05' | '\x06' | '\x07' | 
  '\x08' | '\x09' | '\x0a' | '\x0b' | '\x0c' | '\x0d' | '\x0e' | '\x0f' | 
  '\x10' | '\x11' | '\x12' | '\x13' | '\x14' | '\x15' | '\x16' | '\x17' | 
  '\x18' | '\x19' | '\x1a' | '\x1b' | '\x1c' | '\x1d' | '\x1e' | '\x1f' | 
  '\x20' | '\x21' | '\x22' | '\x23' | '\x24' | '\x25' | '\x26' | '\x27' | 
  '\x28' | '\x29' | '\x2a' | '\x2b' | '\x2c' | '\x2d' | '\x2e' | '\x2f' | 
  '\x30' | '\x31' | '\x32' | '\x33' | '\x34' | '\x35' | '\x36' | '\x37' | 
  '\x38' | '\x39' | '\x3a' | '\x3b' | '\x3c' ;
r__2 : 
  '\xa6' | '\xa7' | '\xa8' | '\xa9' | '\xaa' | '\xab' | '\xac' | '\xad' | 
  '\xae' | '\xaf' | '\xb0' | '\xb1' | '\xb2' | '\xb3' | '\xb4' | '\xb5' | 
  '\xb6' | '\xb7' | '\xb8' | '\xb9' | '\xba' | '\xbb' | '\xbc' | '\xbd' | 
  '\xbe' | '\xbf' | '\xc0' | '\xc1' | '\xc2' | '\xc3' | '\xc4' | '\xc5' | 
  '\xc6' | '\xc7' | '\xc8' | '\xc9' | '\xca' | '\xcb' | '\xcc' | '\xcd' | 
  '\xce' | '\xcf' | '\xd0' | '\xd1' | '\xd2' | '\xd3' | '\xd4' | '\xd5' | 
  '\xd6' | '\xd7' | '\xd8' | '\xd9' | '\xda' | '\xdb' | '\xdc' | '\xdd' | 
  '\xde' | '\xdf' | '\xe0' | '\xe1' | '\xe2' | '\xe3' | '\xe4' | '\xe5' | 
  '\xe6' | '\xe7' | '\xe8' | '\xe9' | '\xea' | '\xeb' | '\xec' | '\xed' | 
  '\xee' | '\xef' | '\xf0' | '\xf1' | '\xf2' | '\xf3' | '\xf4' | '\xf5' | 
  '\xf6' | '\xf7' | '\xf8' | '\xf9' | '\xfa' | '\xfb' | '\xfc' | '\xfd' | 
  '\xfe' | '\xff' | X00 | '\x01' | '\x02' | '\x03' | '\x04' | '\x05' | 
  '\x06' | '\x07' | '\x08' | '\x09' | '\x0a' | '\x0b' | '\x0c' | '\x0d' | 
  '\x0e' | '\x0f' | '\x10' | '\x11' | '\x12' | '\x13' | '\x14' | '\x15' | 
  '\x16' | '\x17' | '\x18' | '\x19' | '\x1a' | '\x1b' | '\x1c' | '\x1d' | 
  '\x1e' | '\x1f' | '\x20' | '\x21' | '\x22' | '\x23' | '\x24' | '\x25' | 
  '\x26' | '\x27' | '\x28' | '\x29' | '\x2a' | '\x2b' | '\x2c' | '\x2d' | 
  '\x2e' | '\x2f' | '\x30' | '\x31' | '\x32' | '\x33' | '\x34' | '\x35' | 
  '\x36' | '\x37' | '\x38' | '\x39' | '\x3a' | '\x3b' | '\x3c' | '\x3d' | 
  '\x3e' | '\x3f' | '\x40' | '\x41' | '\x42' | '\x43' | '\x44' | '\x45' | 
  '\x46' | '\x47' | '\x48' | '\x49' | '\x4a' | '\x4b' | '\x4c' | '\x4d' | 
  '\x4e' | '\x4f' | '\x50' | '\x51' | '\x52' | '\x53' | '\x54' | '\x55' | 
  '\x56' | '\x57' | '\x58' | '\x59' | '\x5a' ;

/* 16 Range Expansions */
rr__0 : 
  '\x4c' '\xff' | '\x4d' '\xff' | '\x4e' '\xff' | '\x4f' '\xff' | '\x50' '\xff' | '\x51' '\xff' | '\x52' '\xff' | '\x53' '\xff' | 
  '\x54' '\xff' | '\x55' '\xff' | '\x56' '\xff' | '\x57' '\xff' | '\x58' '\xff' | '\x59' '\xff' | '\x5a' '\xff' | '\x5b' '\xff' | 
  '\x5c' '\xff' | '\x5d' '\xff' | '\x5e' '\xff' | '\x5f' '\xff' | '\x60' '\xff' | '\x61' '\xff' | '\x62' '\xff' | '\x63' '\xff' | 
  '\x64' '\xff' | '\x65' '\xff' | '\x66' '\xff' | '\x67' '\xff' | '\x68' '\xff' | '\x69' '\xff' | '\x6a' '\xff' | '\x6b' '\xff' | 
  '\x6c' '\xff' | '\x6d' '\xff' | '\x6e' '\xff' | '\x6f' '\xff' | '\x70' '\xff' | '\x71' '\xff' | '\x72' '\xff' | '\x73' '\xff' | 
  '\x74' '\xff' | '\x75' '\xff' | '\x76' '\xff' | '\x77' '\xff' | '\x78' '\xff' | '\x79' '\xff' | '\x7a' '\xff' | '\x7b' '\xff' | 
  '\x7c' '\xff' | '\x7d' '\xff' | '\x7e' '\xff' | '\x7f' '\xff' | '\x80' '\xff' | '\x81' '\xff' | '\x82' '\xff' | '\x83' '\xff' | 
  '\x84' '\xff' | '\x85' '\xff' | '\x86' '\xff' | '\x87' '\xff' | '\x88' '\xff' | '\x89' '\xff' | '\x8a' '\xff' | '\x8b' '\xff' | 
  '\x8c' '\xff' | '\x8d' '\xff' | '\x8e' '\xff' | '\x8f' '\xff' | '\x90' '\xff' | '\x91' '\xff' | '\x92' '\xff' | '\x93' '\xff' | 
  '\x94' '\xff' | '\x95' '\xff' | '\x96' '\xff' | '\x97' '\xff' | '\x98' '\xff' | '\x99' '\xff' | '\x9a' '\xff' | '\x9b' '\xff' | 
  '\x9c' '\xff' | '\x9d' '\xff' | '\x9e' '\xff' | '\x9f' '\xff' | '\xa0' '\xff' | '\xa1' '\xff' | '\xa2' '\xff' | '\xa3' '\xff' | 
  '\xa4' '\xff' | '\xa5' '\xff' | '\xa6' '\xff' | '\xa7' '\xff' | '\xa8' '\xff' | '\xa9' '\xff' | '\xaa' '\xff' | '\xab' '\xff' | 
  '\xac' '\xff' | '\xad' '\xff' | '\xae' '\xff' | '\xaf' '\xff' | '\xb0' '\xff' | '\xb1' '\xff' | '\xb2' '\xff' | '\xb3' '\xff' | 
  '\xb4' '\xff' | '\xb5' '\xff' | '\xb6' '\xff' | '\xb7' '\xff' | '\xb8' '\xff' | '\xb9' '\xff' | '\xba' '\xff' | '\xbb' '\xff' | 
  '\xbc' '\xff' | '\xbd' '\xff' | '\xbe' '\xff' | '\xbf' '\xff' | '\xc0' '\xff' | '\xc1' '\xff' | '\xc2' '\xff' | '\xc3' '\xff' | 
  '\xc4' '\xff' | '\xc5' '\xff' | '\xc6' '\xff' | '\xc7' '\xff' | '\xc8' '\xff' | '\xc9' '\xff' | '\xca' '\xff' | '\xcb' '\xff' | 
  '\xcc' '\xff' | '\xcd' '\xff' | '\xce' '\xff' | '\xcf' '\xff' | '\xd0' '\xff' | '\xd1' '\xff' | '\xd2' '\xff' | '\xd3' '\xff' | 
  '\xd4' '\xff' | '\xd5' '\xff' | '\xd6' '\xff' | '\xd7' '\xff' | '\xd8' '\xff' | '\xd9' '\xff' | '\xda' '\xff' | '\xdb' '\xff' | 
  '\xdc' '\xff' | '\xdd' '\xff' | '\xde' '\xff' | '\xdf' '\xff' | '\xe0' '\xff' | '\xe1' '\xff' | '\xe2' '\xff' | '\xe3' '\xff' | 
  '\xe4' '\xff' | '\xe5' '\xff' | '\xe6' '\xff' | '\xe7' '\xff' | '\xe8' '\xff' | '\xe9' '\xff' | '\xea' '\xff' | '\xeb' '\xff' | 
  '\xec' '\xff' | '\xed' '\xff' | '\xee' '\xff' | '\xef' '\xff' | '\xf0' '\xff' | '\xf1' '\xff' | '\xf2' '\xff' | '\xf3' '\xff' | 
  '\xf4' '\xff' | '\xf5' '\xff' | '\xf6' '\xff' | '\xf7' '\xff' | '\xf8' '\xff' | '\xf9' '\xff' | '\xfa' '\xff' | '\xfb' '\xff' | 
  '\xfc' '\xff' | '\xfd' '\xff' | '\xfe' '\xff' | '\xff' '\xff' | X00 X00 | '\x01' X00 | '\x02' X00 | '\x03' X00 | 
  '\x04' X00 | '\x05' X00 | '\x06' X00 | '\x07' X00 | '\x08' X00 | '\x09' X00 | '\x0a' X00 | '\x0b' X00 | 
  '\x0c' X00 | '\x0d' X00 | '\x0e' X00 | '\x0f' X00 | '\x10' X00 | '\x11' X00 | '\x12' X00 | '\x13' X00 | 
  '\x14' X00 | '\x15' X00 | '\x16' X00 | '\x17' X00 | '\x18' X00 | '\x19' X00 | '\x1a' X00 | '\x1b' X00 | 
  '\x1c' X00 | '\x1d' X00 | '\x1e' X00 | '\x1f' X00 | '\x20' X00 | '\x21' X00 | '\x22' X00 | '\x23' X00 | 
  '\x24' X00 | '\x25' X00 | '\x26' X00 | '\x27' X00 | '\x28' X00 | '\x29' X00 | '\x2a' X00 | '\x2b' X00 | 
  '\x2c' X00 | '\x2d' X00 | '\x2e' X00 | '\x2f' X00 | '\x30' X00 | '\x31' X00 | '\x32' X00 | '\x33' X00 | 
  '\x34' X00 | '\x35' X00 | '\x36' X00 | '\x37' X00 | '\x38' X00 | '\x39' X00 | '\x3a' X00 | '\x3b' X00 | 
  '\x3c' X00 | '\x3d' X00 | '\x3e' X00 | '\x3f' X00 | '\x40' X00 | '\x41' X00 | '\x42' X00 | '\x43' X00 | 
  '\x44' X00 | '\x45' X00 | '\x46' X00 | '\x47' X00 | '\x48' X00 | '\x49' X00 | '\x4a' X00 | '\x4b' X00 | 
  '\x4c' X00 | '\x4d' X00 | '\x4e' X00 | '\x4f' X00 | '\x50' X00 | '\x51' X00 | '\x52' X00 | '\x53' X00 | 
  '\x54' X00 | '\x55' X00 | '\x56' X00 | '\x57' X00 | '\x58' X00 | '\x59' X00 | '\x5a' X00 | '\x5b' X00 | 
  '\x5c' X00 | '\x5d' X00 | '\x5e' X00 | '\x5f' X00 | '\x60' X00 | '\x61' X00 | '\x62' X00 | '\x63' X00 | 
  '\x64' X00 | '\x65' X00 | '\x66' X00 | '\x67' X00 | '\x68' X00 | '\x69' X00 | '\x6a' X00 | '\x6b' X00 | 
  '\x6c' X00 | '\x6d' X00 | '\x6e' X00 | '\x6f' X00 | '\x70' X00 | '\x71' X00 | '\x72' X00 | '\x73' X00 | 
  '\x74' X00 | '\x75' X00 | '\x76' X00 | '\x77' X00 | '\x78' X00 | '\x79' X00 | '\x7a' X00 | '\x7b' X00 | 
  '\x7c' X00 | '\x7d' X00 | '\x7e' X00 | '\x7f' X00 | '\x80' X00 | '\x81' X00 | '\x82' X00 | '\x83' X00 | 
  '\x84' X00 | '\x85' X00 | '\x86' X00 | '\x87' X00 | '\x88' X00 | '\x89' X00 | '\x8a' X00 | '\x8b' X00 | 
  '\x8c' X00 | '\x8d' X00 | '\x8e' X00 | '\x8f' X00 | '\x90' X00 | '\x91' X00 | '\x92' X00 | '\x93' X00 | 
  '\x94' X00 | '\x95' X00 | '\x96' X00 | '\x97' X00 | '\x98' X00 | '\x99' X00 | '\x9a' X00 | '\x9b' X00 | 
  '\x9c' X00 | '\x9d' X00 | '\x9e' X00 | '\x9f' X00 | '\xa0' X00 | '\xa1' X00 | '\xa2' X00 | '\xa3' X00 | 
  '\xa4' X00 | '\xa5' X00 | '\xa6' X00 | '\xa7' X00 | '\xa8' X00 | '\xa9' X00 | '\xaa' X00 | '\xab' X00 | 
  '\xac' X00 | '\xad' X00 | '\xae' X00 | '\xaf' X00 | '\xb0' X00 | '\xb1' X00 | '\xb2' X00 | '\xb3' X00 | 
  '\xb4' X00 ;
