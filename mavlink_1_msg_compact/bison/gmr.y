%{
  #define YYDEBUG 1
  int yylex(void);
  void yyerror(char *s);
%}
%token X00
%%
P : optMessage ;
optMessage : /* empty */ | optMessage message ;

/* a message may be either a MAVLink 1, signed or an unsigned message */
message : messageM1 | messageUS | messageS ;

/* MAVLink 1 messages have a slightly different header fields and cannot contain a signature */
messageM1 : spM1 crc ;

/* unsigned messsages contain a header with a payload and finally a checksum */
messageUS : spUS crc ;

/* signed messages contain a header with a payload, followed by a checksum and finally a 13 byte signature */
messageS : spS crc sig ;

/* SCALED_PRESSURE (29) */
spM1 : ml14M1 SPCODEM1 spPL14 ;
spUS : sp01US | sp02US | sp03US | sp04US | sp05US | sp06US | sp07US | sp08US | sp09US | sp10US | sp11US | sp12US | sp13US | sp14US | sp15US | sp16US ;
spS : sp01S | sp02S | sp03S | sp04S | sp05S | sp06S | sp07S | sp08S | sp09S | sp10S | sp11S | sp12S | sp13S | sp14S | sp15S | sp16S ;
sp01US : spMH01US spPL01 ;
sp02US : spMH02US spPL02 ;
sp03US : spMH03US spPL03 ;
sp04US : spMH04US spPL04 ;
sp05US : spMH05US spPL05 ;
sp06US : spMH06US spPL06 ;
sp07US : spMH07US spPL07 ;
sp08US : spMH08US spPL08 ;
sp09US : spMH09US spPL09 ;
sp10US : spMH10US spPL10 ;
sp11US : spMH11US spPL11 ;
sp12US : spMH12US spPL12 ;
sp13US : spMH13US spPL13 ;
sp14US : spMH14US spPL14 ;
sp15US : spMH15US spPL15 ;
sp16US : spMH16US spPL16 ;
sp01S : spMH01S spPL01 ;
sp02S : spMH02S spPL02 ;
sp03S : spMH03S spPL03 ;
sp04S : spMH04S spPL04 ;
sp05S : spMH05S spPL05 ;
sp06S : spMH06S spPL06 ;
sp07S : spMH07S spPL07 ;
sp08S : spMH08S spPL08 ;
sp09S : spMH09S spPL09 ;
sp10S : spMH10S spPL10 ;
sp11S : spMH11S spPL11 ;
sp12S : spMH12S spPL12 ;
sp13S : spMH13S spPL13 ;
sp14S : spMH14S spPL14 ;
sp15S : spMH15S spPL15 ;
sp16S : spMH16S spPL16 ;
spMH01US : ml01US SPCODEM2 ;
spMH02US : ml02US SPCODEM2 ;
spMH03US : ml03US SPCODEM2 ;
spMH04US : ml04US SPCODEM2 ;
spMH05US : ml05US SPCODEM2 ;
spMH06US : ml06US SPCODEM2 ;
spMH07US : ml07US SPCODEM2 ;
spMH08US : ml08US SPCODEM2 ;
spMH09US : ml09US SPCODEM2 ;
spMH10US : ml10US SPCODEM2 ;
spMH11US : ml11US SPCODEM2 ;
spMH12US : ml12US SPCODEM2 ;
spMH13US : ml13US SPCODEM2 ;
spMH14US : ml14US SPCODEM2 ;
spMH15US : ml15US SPCODEM2 ;
spMH16US : ml16US SPCODEM2 ;
spMH01S : ml01S SPCODEM2 ;
spMH02S : ml02S SPCODEM2 ;
spMH03S : ml03S SPCODEM2 ;
spMH04S : ml04S SPCODEM2 ;
spMH05S : ml05S SPCODEM2 ;
spMH06S : ml06S SPCODEM2 ;
spMH07S : ml07S SPCODEM2 ;
spMH08S : ml08S SPCODEM2 ;
spMH09S : ml09S SPCODEM2 ;
spMH10S : ml10S SPCODEM2 ;
spMH11S : ml11S SPCODEM2 ;
spMH12S : ml12S SPCODEM2 ;
spMH13S : ml13S SPCODEM2 ;
spMH14S : ml14S SPCODEM2 ;
spMH15S : ml15S SPCODEM2 ;
spMH16S : ml16S SPCODEM2 ;
spPL01 : r__0 ;
spPL02 : r__0 r__0 ;
spPL03 : r__0 r__0 r__0 ;
spPL04 : r__0 r__0 r__0 r__0 ;
spPL05 : r__0 r__0 r__0 r__0 r__0 ;
spPL06 : r__0 r__0 r__0 r__0 r__0 r__0 ;
spPL07 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
spPL08 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
spPL09 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
spPL10 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
spPL11 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
spPL12 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
spPL13 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
spPL14 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
spPL15 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
spPL16 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;

/* header starter for MAVLink 1 messages */
ml14M1 : MAV1CODE LEN14 messageHeaderTailM1 ;

/* header starter for messages of the same length, necessary to remove reduce-reduce conflict since the first token that differentiates in the message header is the message code - unsigned messages */
ml01US : MAV2CODE LEN01 messageHeaderTailUS ;
ml02US : MAV2CODE LEN02 messageHeaderTailUS ;
ml03US : MAV2CODE LEN03 messageHeaderTailUS ;
ml04US : MAV2CODE LEN04 messageHeaderTailUS ;
ml05US : MAV2CODE LEN05 messageHeaderTailUS ;
ml06US : MAV2CODE LEN06 messageHeaderTailUS ;
ml07US : MAV2CODE LEN07 messageHeaderTailUS ;
ml08US : MAV2CODE LEN08 messageHeaderTailUS ;
ml09US : MAV2CODE LEN09 messageHeaderTailUS ;
ml10US : MAV2CODE LEN10 messageHeaderTailUS ;
ml11US : MAV2CODE LEN11 messageHeaderTailUS ;
ml12US : MAV2CODE LEN12 messageHeaderTailUS ;
ml13US : MAV2CODE LEN13 messageHeaderTailUS ;
ml14US : MAV2CODE LEN14 messageHeaderTailUS ;
ml15US : MAV2CODE LEN15 messageHeaderTailUS ;
ml16US : MAV2CODE LEN16 messageHeaderTailUS ;

/* header starter for messages of the same length, necessary to remove reduce-reduce conflict since the first token that differentiates in the message header is the message code  - signed messages*/
ml01S : MAV2CODE LEN01 messageHeaderTailS ;
ml02S : MAV2CODE LEN02 messageHeaderTailS ;
ml03S : MAV2CODE LEN03 messageHeaderTailS ;
ml04S : MAV2CODE LEN04 messageHeaderTailS ;
ml05S : MAV2CODE LEN05 messageHeaderTailS ;
ml06S : MAV2CODE LEN06 messageHeaderTailS ;
ml07S : MAV2CODE LEN07 messageHeaderTailS ;
ml08S : MAV2CODE LEN08 messageHeaderTailS ;
ml09S : MAV2CODE LEN09 messageHeaderTailS ;
ml10S : MAV2CODE LEN10 messageHeaderTailS ;
ml11S : MAV2CODE LEN11 messageHeaderTailS ;
ml12S : MAV2CODE LEN12 messageHeaderTailS ;
ml13S : MAV2CODE LEN13 messageHeaderTailS ;
ml14S : MAV2CODE LEN14 messageHeaderTailS ;
ml15S : MAV2CODE LEN15 messageHeaderTailS ;
ml16S : MAV2CODE LEN16 messageHeaderTailS ;

/* tail of the message header for MAVLink 1, unsigned and signed messages, respectively */
messageHeaderTailM1 : packetSequence sysID compID ;
messageHeaderTailUS :  incompFlagUS compFlag packetSequence sysID compID ;
messageHeaderTailS :  incompFlagS compFlag packetSequence sysID compID ;

/* Tokens */
MAV1CODE : '\xfe' ;
MAV2CODE : '\xfd' ;

SPCODEM1 : '\x1d' ;
SPCODEM2 : '\x1d' X00 X00 ;

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

incompFlagUS : X00 ;  /* incompatibility flag for unsigned message */
incompFlagS : '\x01' ; /* incompatinility flag for signed message */
compFlag : X00 ; /* compatibility flag */
packetSequence : r__0 ;
sysID : r__0 ; /* system ID  */
compID : r__0 ; /* component ID */

crc : r__0 r__0 ;
sig : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
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
