%{
#define YYDEBUG 1
int yylex(void);
void yyerror(char *s);

int hdgCount = 0;
int throttleCount = 0;
%}
%token X00
%%
P : optMessage ;
optMessage : /* empty */ | optMessage message ;

/* Messsages contain header for each packet followed by the MAVLink specific header then the message and finally a checkusm */
message : packetRecordHeader mavHeader messageOption r__0 r__0 ;
messageOption : vh ;

/* Header for each captured packet */
packetRecordHeader : prhTimestampSec prhTimestampUSNS prhCPL prhOPL ;
prhTimestampSec : r__0 r__0 r__0 r__0 ;
prhTimestampUSNS : r__0 r__0 r__0 r__0 ;
prhCPL : r__0 r__0 r__0 r__0 ; /* captured packet length */
prhOPL : r__0 r__0 r__0 r__0 ; /* original packet length */

/* MAVLink header for each message */
mavHeader : family ipvhl dsf totalLen id ffo ttl protocol headerChecksum sourceAddr destAddr sourcePort destPort length checksum ;
family : '\x02' X00 X00 X00 ; /* Always 2 for MAVLink */
ipvhl : '\x45' ; /* ip version and header length  - always ip version 4 and 5 as header length for MAVLink */
dsf : r__0 ; /* differentiated services field - not sure */
totalLen : r__0 r__0 ;
id : r__0 r__0 ;
ffo : r__0 r__0 ;
ttl : '\x40' ; /* time to live - seemingly always 0x40 for MAVLink - not sure */
protocol : '\x11' ; /* protocol - always 0x11 for MAVLink because it is UDP */
headerChecksum : r__0 r__0 ; /* header checksum - not sure */
sourceAddr : r__0 r__0 r__0 r__0 ;
destAddr : r__0 r__0 r__0 r__0 ;
sourcePort : '\x38' '\xd6' ;
destPort : '\x38' '\xf4' ;
length : r__0 r__0 ;
checksum : r__0 r__0 ;

/* VFR_HUD (74) */
vh : vhMH vhAS vhGS vhAlt vhClimb vhHeading vhThrottle ;
vhMH : MAVCODE VHLEN incompFlag compFlag packetSequence sysID compID VHCODE ;
vhAS : r__0 r__0 r__0 r__0 ;
vhGS : r__0 r__0 r__0 r__0 ;
vhAlt : r__0 r__0 r__0 r__0 ;
vhClimb : r__0 r__0 r__0 r__0 ;
vhHeading : /* empty */ | vhHeading r__0 { hdgCount++; if (hdgCount > 2) YYERROR; } ;
vhThrottle : /* empty */ | vhThrottle r__0 { throttleCount++; if (throttleCount > 2) YYERROR; } ;

/* Tokens */
MAVCODE : '\xfd' ;

VHCODE : '\x4a' X00 X00 ;

VHLEN : '\x14' ;

incompFlag : X00 ;  /* incompatibility flag */
compFlag : X00 ; /* compatibility flag */
packetSequence : r__0 ;
sysID : '\x01' | '\x33' | '\xff' ; /* system ID  */
compID : '\x01' | '\x44' | '\xbe' ; /* component ID */

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
