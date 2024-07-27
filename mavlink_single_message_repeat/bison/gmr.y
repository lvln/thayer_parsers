%{
  #define YYDEBUG 1
  int yylex(void);
  void yyerror(char *s);
%}
%token X00
%%
P : optMessage ;
optMessage : /* empty */ | optMessage message ;

/* Messsages contain header for each packet followed by the MAVLink specific header then the message and finally a checkusm */
message : sp r__0 r__0 ;

/* SCALED_PRESSURE (29) */
sp : sp01 | sp02 | sp03 | sp04 | sp05 | sp06 | sp07 | sp08 | sp09 | sp10 | sp11 | sp12 | sp13 | sp14 | sp15 | sp16 ;
sp01 : spMH01 r__0 ;
sp02 : spMH02 r__0 r__0 ;
sp03 : spMH03 r__0 r__0 r__0 ;
sp04 : spMH04 r__0 r__0 r__0 r__0 ;
sp05 : spMH05 r__0 r__0 r__0 r__0 r__0 ;
sp06 : spMH06 r__0 r__0 r__0 r__0 r__0 r__0 ;
sp07 : spMH07 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
sp08 : spMH08 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
sp09 : spMH09 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
sp10 : spMH10 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
sp11 : spMH11 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
sp12 : spMH12 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
sp13 : spMH13 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
sp14 : spMH14 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
sp15 : spMH15 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
sp16 : spMH16 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
spMH01 : ml01 SPCODE ;
spMH02 : ml02 SPCODE ;
spMH03 : ml03 SPCODE ;
spMH04 : ml04 SPCODE ;
spMH05 : ml05 SPCODE ;
spMH06 : ml06 SPCODE ;
spMH07 : ml07 SPCODE ;
spMH08 : ml08 SPCODE ;
spMH09 : ml09 SPCODE ;
spMH10 : ml10 SPCODE ;
spMH11 : ml11 SPCODE ;
spMH12 : ml12 SPCODE ;
spMH13 : ml13 SPCODE ;
spMH14 : ml14 SPCODE ;
spMH15 : ml15 SPCODE ;
spMH16 : ml16 SPCODE ;

/* Header starter for messages of the same length, necessary to remove reduce-reduce conflict since the first token that differentiates in the message header is the message code */
ml01 : MAVCODE LEN01 messageHeaderTail ;
ml02 : MAVCODE LEN02 messageHeaderTail ;
ml03 : MAVCODE LEN03 messageHeaderTail ;
ml04 : MAVCODE LEN04 messageHeaderTail ;
ml05 : MAVCODE LEN05 messageHeaderTail ;
ml06 : MAVCODE LEN06 messageHeaderTail ;
ml07 : MAVCODE LEN07 messageHeaderTail ;
ml08 : MAVCODE LEN08 messageHeaderTail ;
ml09 : MAVCODE LEN09 messageHeaderTail ;
ml10 : MAVCODE LEN10 messageHeaderTail ;
ml11 : MAVCODE LEN11 messageHeaderTail ;
ml12 : MAVCODE LEN12 messageHeaderTail ;
ml13 : MAVCODE LEN13 messageHeaderTail ;
ml14 : MAVCODE LEN14 messageHeaderTail ;
ml15 : MAVCODE LEN15 messageHeaderTail ;
ml16 : MAVCODE LEN16 messageHeaderTail ;

messageHeaderTail :  incompFlag compFlag packetSequence sysID compID ;

/* Tokens */
MAVCODE : '\xfd' ;

SPCODE : '\x1d' X00 X00 ;

LEN01 : '\x01' ;
LEN02 : '\x02' ;
LEN03 : '\x03' ;
LEN04 : '\x04' ;
LEN05 : '\x05' ;
LEN06 : '\x06' ;
LEN07 : '\x07' ;
LEN08 : '\x08' ;
LEN09 : '\x09' ;
LEN10 : '\x0a' ;
LEN11 : '\x0b' ;
LEN12 : '\x0c' ;
LEN13 : '\x0d' ;
LEN14 : '\x0e' ;
LEN15 : '\x0f' ;
LEN16 : '\x10' ;

incompFlag : X00 ;  /* incompatibility flag */
compFlag : X00 ; /* compatibility flag */
packetSequence : r__0 ;
sysID : r__0 ; /* system ID  */
compID : r__0 ; /* component ID */

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
