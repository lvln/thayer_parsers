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
message : packetRecordHeader mavHeader sp WC WC ;

/* Header for each captured packet */
packetRecordHeader : prhTimestampSec prhTimestampUSNS prhCPL prhOPL ;
prhTimestampSec : WC WC WC WC ;
prhTimestampUSNS : WC WC WC WC ;
prhCPL : WC WC WC WC ; /* captured packet length */
prhOPL : WC WC WC WC ; /* original packet length */

/* MAVLink header for each message */
mavHeader : family ipvhl dsf totalLen id ffo ttl protocol headerChecksum sourceAddr destAddr sourcePort destPort length checksum ;
family : '\x02' X00 X00 X00 ; /* Always 2 for MAVLink */
ipvhl : '\x45' ; /* ip version and header length  - always ip version 4 and 5 as header length for MAVLink */
dsf : WC ; /* differentiated services field - not sure */
totalLen : X00 '\x38' ;
id : WC WC ;
ffo : WC WC ;
ttl : '\x40' ; /* time to live - seemingly always 0x40 for MAVLink - not sure */
protocol : '\x11' ; /* protocol - always 0x11 for MAVLink because it is UDP */
headerChecksum : WC WC ; /* header checksum - not sure */
sourceAddr : WC WC WC WC ;
destAddr : WC WC WC WC ;
sourcePort : '\x38' '\xd6' ;
destPort : '\x38' '\xf4' ;
length : WC WC ;
checksum : WC WC ;

/* SCALED_PRESSURE (29) */
sp : spMH timeBoot spPressAb spPressDiff spTemp spTempParseDiff ;
spMH : MAVCODE SPLEN incompFlag compFlag packetSequence sysID compID SPCODE ;
spPressAb : WC WC WC WC ;
spPressDiff : WC WC WC WC ;
spTemp : WC WC ;
spTempParseDiff : WC WC ;

/* Tokens */
MAVCODE : '\xfd' ;

SPCODE : '\x1d' X00 X00 ;

SPLEN : '\x10' ;

incompFlag : X00 ;  /* incompatibility flag */
compFlag : X00 ; /* compatibility flag */
packetSequence : WC ;
sysID : '\x01' | '\x33' | '\xff' ; /* system ID  */
compID : '\x01' | '\x44' | '\xbe' ; /* component ID */
timeBoot : WC WC WC WC ;

/* Range Expansions */
WC : { int c = yylex(); if (c == 0) YYABORT; } ;
