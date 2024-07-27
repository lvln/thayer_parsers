%{
  #define YYDEBUG 1
  int yylex(void);
  void yyerror(char *s);
%}
%token X00
%%
P : optMessage ;
optMessage : /* empty */ | optMessage message ;

/* Messsages contain the message followed by a checkusm */
message : messageOption r__0 r__0 ;
messageOption : sp | att | gpi ;

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

/* ATTITUDE (30) */
att : att01 | att02 | att03 | att04 | att05 | att06 | att07 | att08 | att09 | att10 | att11 | att12 | att13 | att14 | att15 | att16 | att17 | att18 | att19 | att20 | att21 | att22 | att23 | att24 | att25 | att26 | att27 | att28 ;
att01 : attMH01 r__0 ;
att02 : attMH02 r__0 r__0 ;
att03 : attMH03 r__0 r__0 r__0 ;
att04 : attMH04 r__0 r__0 r__0 r__0 ;
att05 : attMH05 r__0 r__0 r__0 r__0 r__0 ;
att06 : attMH06 r__0 r__0 r__0 r__0 r__0 r__0 ;
att07 : attMH07 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
att08 : attMH08 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
att09 : attMH09 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
att10 : attMH10 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
att11 : attMH11 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
att12 : attMH12 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
att13 : attMH13 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
att14 : attMH14 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
att15 : attMH15 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
att16 : attMH16 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
att17 : attMH17 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
att18 : attMH18 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
att19 : attMH19 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
att20 : attMH20 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
att21 : attMH21 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
att22 : attMH22 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
att23 : attMH23 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
att24 : attMH24 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
att25 : attMH25 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
att26 : attMH26 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
att27 : attMH27 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
att28 : attMH28 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
attMH01 : ml01 ATTCODE ;
attMH02 : ml02 ATTCODE ;
attMH03 : ml03 ATTCODE ;
attMH04 : ml04 ATTCODE ;
attMH05 : ml05 ATTCODE ;
attMH06 : ml06 ATTCODE ;
attMH07 : ml07 ATTCODE ;
attMH08 : ml08 ATTCODE ;
attMH09 : ml09 ATTCODE ;
attMH10 : ml10 ATTCODE ;
attMH11 : ml11 ATTCODE ;
attMH12 : ml12 ATTCODE ;
attMH13 : ml13 ATTCODE ;
attMH14 : ml14 ATTCODE ;
attMH15 : ml15 ATTCODE ;
attMH16 : ml16 ATTCODE ;
attMH17 : ml17 ATTCODE ;
attMH18 : ml18 ATTCODE ;
attMH19 : ml19 ATTCODE ;
attMH20 : ml20 ATTCODE ;
attMH21 : ml21 ATTCODE ;
attMH22 : ml22 ATTCODE ;
attMH23 : ml23 ATTCODE ;
attMH24 : ml24 ATTCODE ;
attMH25 : ml25 ATTCODE ;
attMH26 : ml26 ATTCODE ;
attMH27 : ml27 ATTCODE ;
attMH28 : ml28 ATTCODE ;

/* GLOBAL_POSITION_INT (33) */
gpi : gpi01 | gpi02 | gpi03 | gpi04 | gpi05 | gpi06 | gpi07 | gpi08 | gpi09 | gpi10 | gpi11 | gpi12 | gpi13 | gpi14 | gpi15 | gpi16 | gpi17 | gpi18 | gpi19 | gpi20 | gpi21 | gpi22 | gpi23 | gpi24 | gpi25 | gpi26 | gpi27 | gpi28 ;
gpi01 : gpiMH01 r__0 ;
gpi02 : gpiMH02 r__0 r__0 ;
gpi03 : gpiMH03 r__0 r__0 r__0 ;
gpi04 : gpiMH04 r__0 r__0 r__0 r__0 ;
gpi05 : gpiMH05 r__0 r__0 r__0 r__0 r__0 ;
gpi06 : gpiMH06 r__0 r__0 r__0 r__0 r__0 r__0 ;
gpi07 : gpiMH07 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
gpi08 : gpiMH08 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
gpi09 : gpiMH09 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
gpi10 : gpiMH10 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
gpi11 : gpiMH11 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
gpi12 : gpiMH12 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
gpi13 : gpiMH13 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
gpi14 : gpiMH14 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
gpi15 : gpiMH15 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
gpi16 : gpiMH16 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
gpi17 : gpiMH17 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
gpi18 : gpiMH18 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
gpi19 : gpiMH19 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
gpi20 : gpiMH20 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
gpi21 : gpiMH21 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
gpi22 : gpiMH22 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
gpi23 : gpiMH23 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
gpi24 : gpiMH24 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
gpi25 : gpiMH25 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
gpi26 : gpiMH26 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
gpi27 : gpiMH27 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
gpi28 : gpiMH28 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
gpiMH01 : ml01 GPICODE ;
gpiMH02 : ml02 GPICODE ;
gpiMH03 : ml03 GPICODE ;
gpiMH04 : ml04 GPICODE ;
gpiMH05 : ml05 GPICODE ;
gpiMH06 : ml06 GPICODE ;
gpiMH07 : ml07 GPICODE ;
gpiMH08 : ml08 GPICODE ;
gpiMH09 : ml09 GPICODE ;
gpiMH10 : ml10 GPICODE ;
gpiMH11 : ml11 GPICODE ;
gpiMH12 : ml12 GPICODE ;
gpiMH13 : ml13 GPICODE ;
gpiMH14 : ml14 GPICODE ;
gpiMH15 : ml15 GPICODE ;
gpiMH16 : ml16 GPICODE ;
gpiMH17 : ml17 GPICODE ;
gpiMH18 : ml18 GPICODE ;
gpiMH19 : ml19 GPICODE ;
gpiMH20 : ml20 GPICODE ;
gpiMH21 : ml21 GPICODE ;
gpiMH22 : ml22 GPICODE ;
gpiMH23 : ml23 GPICODE ;
gpiMH24 : ml24 GPICODE ;
gpiMH25 : ml25 GPICODE ;
gpiMH26 : ml26 GPICODE ;
gpiMH27 : ml27 GPICODE ;
gpiMH28 : ml28 GPICODE ;

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
ml17 : MAVCODE LEN17 messageHeaderTail ;
ml18 : MAVCODE LEN18 messageHeaderTail ;
ml19 : MAVCODE LEN19 messageHeaderTail ;
ml20 : MAVCODE LEN20 messageHeaderTail ;
ml21 : MAVCODE LEN21 messageHeaderTail ;
ml22 : MAVCODE LEN22 messageHeaderTail ;
ml23 : MAVCODE LEN23 messageHeaderTail ;
ml24 : MAVCODE LEN24 messageHeaderTail ;
ml25 : MAVCODE LEN25 messageHeaderTail ;
ml26 : MAVCODE LEN26 messageHeaderTail ;
ml27 : MAVCODE LEN27 messageHeaderTail ;
ml28 : MAVCODE LEN28 messageHeaderTail ;

messageHeaderTail : incompFlag compFlag packetSequence sysID compID ;

/* Tokens */
MAVCODE : '\xfd' ;

SPCODE : '\x1d' X00 X00 ;
ATTCODE : '\x1e' X00 X00 ;
GPICODE : '\x21' X00 X00 ;

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
LEN17 : '\x11' ;
LEN18 : '\x12' ;
LEN19 : '\x13' ;
LEN20 : '\x14' ;
LEN21 : '\x15' ;
LEN22 : '\x16' ;
LEN23 : '\x17' ;
LEN24 : '\x18' ;
LEN25 : '\x19' ;
LEN26 : '\x1a' ;
LEN27 : '\x1b' ;
LEN28 : '\x1c' ;

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
