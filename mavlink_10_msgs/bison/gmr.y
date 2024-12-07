%{
  #define YYDEBUG 1
  int yylex(void);
  void yyerror(char *s);
%}
%token X00
%%
AAP : optMessage ;
optMessage : /* empty */ | optMessage message ;

/* a message may be either a MAVLink 1, a signed or an unsigned message */
message : messageM1 | messageUS | messageS ;

/* MAVLink 1 messages have a slightly different header fields and cannot contain a signature */
messageM1 : messageOptM1 crc ;
messageOptM1 : hbM1 | ssM1 | stM1 | pingM1 | lnsM1 | smM1 | prrM1 | prlM1 | pvM1 | psM1 ;

/* unsigned messsages contain a header with a payload and finally a checksum */
messageUS : messageOptUS crc ;
messageOptUS : genericMessUS | hbUS | smUS | pvUS | psUS ;

/* signed messages contain a header with a payload, followed by a checksum and finally a 13 byte signature */
messageS : messageOptS crc sig ;
messageOptS : genericMessS | hbS | smS | pvS | psS ;

/* generic unsigned messages - messages with all wildcards in payload */
genericMessUS : generic01US | generic02US | generic03US | generic04US | generic05US | generic06US | generic07US | generic08US | generic09US | generic10US | generic11US | generic12US | generic13US | generic14US | generic15US | generic16US | generic17US | generic18US | generic19US | generic20US | generic21US | generic22US | generic23US | generic24US | generic25US | generic26US | generic27US | generic28US | generic29US | generic30US | generic31US | generic32US | generic33US | generic34US | generic35US | generic36US | generic37US | generic38US | generic39US | generic40US | generic41US | generic42US | generic43US ;

/* generic signed messages - messages with all wildcards in payload */
genericMessS : generic01S | generic02S | generic03S | generic04S | generic05S | generic06S | generic07S | generic08S | generic09S | generic10S | generic11S | generic12S | generic13S | generic14S | generic15S | generic16S | generic17S | generic18S | generic19S | generic20S | generic21S | generic22S | generic23S | generic24S | generic25S | generic26S | generic27S | generic28S | generic29S | generic30S | generic31S | generic32S | generic33S | generic34S | generic35S | generic36S | generic37S | generic38S | generic39S | generic40S | generic41S | generic42S | generic43S ;

/* generic unsigned messages */
generic01US : ml01US genericCodes01to02 PL01 ;
generic02US : ml02US genericCodes01to02 PL02 ;
generic03US : ml03US genericCodes03to04 PL03 ;
generic04US : ml04US genericCodes03to04 PL04 ;
generic05US : ml05US genericCodes05 PL05 ;
generic06US : ml06US genericCodes06to12 PL06 ;
generic07US : ml07US genericCodes06to12 PL07 ;
generic08US : ml08US genericCodes06to12 PL08 ;
generic09US : ml09US genericCodes06to12 PL09 ;
generic10US : ml10US genericCodes06to12 PL10 ;
generic11US : ml11US genericCodes06to12 PL11 ;
generic12US : ml12US genericCodes06to12 PL12 ;
generic13US : ml13US genericCodes13to14 PL13 ;
generic14US : ml14US genericCodes13to14 PL14 ;
generic15US : ml15US genericCodes15to22 PL15 ;
generic16US : ml16US genericCodes15to22 PL16 ;
generic17US : ml17US genericCodes15to22 PL17 ;
generic18US : ml18US genericCodes15to22 PL18 ;
generic19US : ml19US genericCodes15to22 PL19 ;
generic20US : ml20US genericCodes15to22 PL20 ;
generic21US : ml21US genericCodes15to22 PL21 ;
generic22US : ml22US genericCodes15to22 PL22 ;
generic23US : ml23US genericCodes23to24 PL23 ;
generic24US : ml24US genericCodes23to24 PL24 ;
generic25US : ml25US genericCodes25to36 PL25 ;
generic26US : ml26US genericCodes25to36 PL26 ;
generic27US : ml27US genericCodes25to36 PL27 ;
generic28US : ml28US genericCodes25to36 PL28 ;
generic29US : ml29US genericCodes25to36 PL29 ;
generic30US : ml30US genericCodes25to36 PL30 ;
generic31US : ml31US genericCodes25to36 PL31 ;
generic32US : ml32US genericCodes25to36 PL32 ;
generic33US : ml33US genericCodes25to36 PL33 ;
generic34US : ml34US genericCodes25to36 PL34 ;
generic35US : ml35US genericCodes25to36 PL35 ;
generic36US : ml36US genericCodes25to36 PL36 ;
generic37US : ml37US genericCodes37to43 PL37 ;
generic38US : ml38US genericCodes37to43 PL38 ;
generic39US : ml39US genericCodes37to43 PL39 ;
generic40US : ml40US genericCodes37to43 PL40 ;
generic41US : ml41US genericCodes37to43 PL41 ;
generic42US : ml42US genericCodes37to43 PL42 ;
generic43US : ml43US genericCodes37to43 PL43 ;

/* generic signed messages */
generic01S : ml01S genericCodes01to02 PL01 ;
generic02S : ml02S genericCodes01to02 PL02 ;
generic03S : ml03S genericCodes03to04 PL03 ;
generic04S : ml04S genericCodes03to04 PL04 ;
generic05S : ml05S genericCodes05 PL05 ;
generic06S : ml06S genericCodes06to12 PL06 ;
generic07S : ml07S genericCodes06to12 PL07 ;
generic08S : ml08S genericCodes06to12 PL08 ;
generic09S : ml09S genericCodes06to12 PL09 ;
generic10S : ml10S genericCodes06to12 PL10 ;
generic11S : ml11S genericCodes06to12 PL11 ;
generic12S : ml12S genericCodes06to12 PL12 ;
generic13S : ml13S genericCodes13to14 PL13 ;
generic14S : ml14S genericCodes13to14 PL14 ;
generic15S : ml15S genericCodes15to22 PL15 ;
generic16S : ml16S genericCodes15to22 PL16 ;
generic17S : ml17S genericCodes15to22 PL17 ;
generic18S : ml18S genericCodes15to22 PL18 ;
generic19S : ml19S genericCodes15to22 PL19 ;
generic20S : ml20S genericCodes15to22 PL20 ;
generic21S : ml21S genericCodes15to22 PL21 ;
generic22S : ml22S genericCodes15to22 PL22 ;
generic23S : ml23S genericCodes23to24 PL23 ;
generic24S : ml24S genericCodes23to24 PL24 ;
generic25S : ml25S genericCodes25to36 PL25 ;
generic26S : ml26S genericCodes25to36 PL26 ;
generic27S : ml27S genericCodes25to36 PL27 ;
generic28S : ml28S genericCodes25to36 PL28 ;
generic29S : ml29S genericCodes25to36 PL29 ;
generic30S : ml30S genericCodes25to36 PL30 ;
generic31S : ml31S genericCodes25to36 PL31 ;
generic32S : ml32S genericCodes25to36 PL32 ;
generic33S : ml33S genericCodes25to36 PL33 ;
generic34S : ml34S genericCodes25to36 PL34 ;
generic35S : ml35S genericCodes25to36 PL35 ;
generic36S : ml36S genericCodes25to36 PL36 ;
generic37S : ml37S genericCodes37to43 PL37 ;
generic38S : ml38S genericCodes37to43 PL38 ;
generic39S : ml39S genericCodes37to43 PL39 ;
generic40S : ml40S genericCodes37to43 PL40 ;
generic41S : ml41S genericCodes37to43 PL41 ;
generic42S : ml42S genericCodes37to43 PL42 ;
generic43S : ml43S genericCodes37to43 PL43 ;

/* codes for generic message payloads of various lengths */
genericCodes01to02 : HBCODEM2 | SSCODEM2 | STCODEM2 | PINGCODEM2 | LNSCODEM2 | SMCODEM2 | PRRCODEM2 | PRLCODEM2 | PVCODEM2 | PSCODEM2 ;
genericCodes03to04 : HBCODEM2 | SSCODEM2 | STCODEM2 | PINGCODEM2 | LNSCODEM2 | SMCODEM2 | PRRCODEM2 | PVCODEM2 | PSCODEM2 ;
genericCodes05 : SSCODEM2 | STCODEM2 | PINGCODEM2 | LNSCODEM2 | SMCODEM2 | PRRCODEM2 | PVCODEM2 | PSCODEM2 ;
genericCodes06to12 : SSCODEM2 | STCODEM2 | PINGCODEM2 | LNSCODEM2 | PRRCODEM2 | PVCODEM2 | PSCODEM2 ;
genericCodes13to14 : SSCODEM2 | PINGCODEM2 | LNSCODEM2 | PRRCODEM2 | PVCODEM2 | PSCODEM2 ;
genericCodes15to22 : SSCODEM2 | LNSCODEM2 | PRRCODEM2 | PVCODEM2 | PSCODEM2 ;
genericCodes23to24 : SSCODEM2 | LNSCODEM2 | PVCODEM2 ;
genericCodes25to36 : SSCODEM2 | LNSCODEM2 ;
genericCodes37to43 : SSCODEM2 ;

/* HEARTBEAT (0) */
hbM1 : ml09M1 HBCODEM1 PL04 hbType hbAutoPilot PL01 hbSysStatus PL01 ;
hbUS : hb05US | hb06US | hb07US | hb08US | hb09US ;
hbS : hb05S | hb06S | hb07S | hb08S | hb09S ;
hb05US : ml05US HBCODEM2 PL04 hbType ;
hb06US : ml06US HBCODEM2 PL04 hbType hbAutoPilot ;
hb07US : ml07US HBCODEM2 PL04 hbType hbAutoPilot PL01 ;
hb08US : ml08US HBCODEM2 PL04 hbType hbAutoPilot PL01 hbSysStatus ;
hb09US : ml09US HBCODEM2 PL04 hbType hbAutoPilot PL01 hbSysStatus PL01 ;
hb05S : ml05S HBCODEM2 PL04 hbType ;
hb06S : ml06S HBCODEM2 PL04 hbType hbAutoPilot ;
hb07S : ml07S HBCODEM2 PL04 hbType hbAutoPilot PL01 ;
hb08S : ml08S HBCODEM2 PL04 hbType hbAutoPilot PL01 hbSysStatus ;
hb09S : ml09S HBCODEM2 PL04 hbType hbAutoPilot PL01 hbSysStatus PL01 ;
hbType :     r__1 ;
hbAutoPilot :     r__2 ;
hbSysStatus :     r__3 ;

/* SYS_STATUS (1) */
ssM1 : ml31M1 SSCODEM1 PL31 ;

/* SYSTEM_TIME (2) */
stM1 : ml12M1 STCODEM1 PL12 ;

/* PING (4) */
pingM1 : ml14M1 PINGCODEM1 PL14 ;

/* LINK_NODE_STATUS (8) */
lnsM1 : ml36M1 LNSCODEM1 PL36 ;

/* SET_MODE (11) */
smM1 : ml06M1 SMCODEM1 PL05 smBaseMode ;
smUS : sm06US ;
smS : sm06S ;
sm06US : ml06US SMCODEM2 PL05 smBaseMode ;
sm06S : ml06S SMCODEM2 PL05 smBaseMode ;
smBaseMode : r__0 ;

/* PARAM_REQUEST_READ (20) */
prrM1 : ml22M1 PRRCODEM1 PL22 ;

/* PARAM_REQUEST_LIST (21) */
prlM1 : ml02M1 PRLCODEM1 PL02 ;

/* PARAM_VALUE (22) */
pvM1 : ml25M1 PVCODEM1 PL24 pvType ;
pvUS : pv25US ;
pvS : pv25S ;
pv25US : ml25US PVCODEM2 PL24 pvType ;
pv25S : ml25S PVCODEM2 PL24 pvType ;
pvType :     r__4 ;

/* PARAM_SET (23) */
psM1 : ml23M1 PSCODEM1 PL22 psType ;
psUS : ps23US ;
psS : ps23S ;
ps23US : ml23US PSCODEM2 PL22 psType ;
ps23S : ml23S PSCODEM2 PL22 psType ;
psType :     r__5 ;

/* header started for MAVLink 1 messages */
ml02M1 : MAV1CODE LEN02  messageHeaderTailM1 ;
ml06M1 : MAV1CODE LEN06 messageHeaderTailM1 ;
ml09M1 : MAV1CODE LEN09 messageHeaderTailM1 ;
ml12M1 : MAV1CODE LEN12 messageHeaderTailM1 ;
ml14M1 : MAV1CODE LEN14 messageHeaderTailM1 ;
ml22M1 : MAV1CODE LEN22 messageHeaderTailM1 ;
ml23M1 : MAV1CODE LEN23 messageHeaderTailM1 ;
ml25M1 : MAV1CODE LEN25 messageHeaderTailM1 ;
ml31M1 : MAV1CODE LEN31 messageHeaderTailM1 ;
ml36M1 : MAV1CODE LEN36 messageHeaderTailM1 ;

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
ml17US : MAV2CODE LEN17 messageHeaderTailUS ;
ml18US : MAV2CODE LEN18 messageHeaderTailUS ;
ml19US : MAV2CODE LEN19 messageHeaderTailUS ;
ml20US : MAV2CODE LEN20 messageHeaderTailUS ;
ml21US : MAV2CODE LEN21 messageHeaderTailUS ;
ml22US : MAV2CODE LEN22 messageHeaderTailUS ;
ml23US : MAV2CODE LEN23 messageHeaderTailUS ;
ml24US : MAV2CODE LEN24 messageHeaderTailUS ;
ml25US : MAV2CODE LEN25 messageHeaderTailUS ;
ml26US : MAV2CODE LEN26 messageHeaderTailUS ;
ml27US : MAV2CODE LEN27 messageHeaderTailUS ;
ml28US : MAV2CODE LEN28 messageHeaderTailUS ;
ml29US : MAV2CODE LEN29 messageHeaderTailUS ;
ml30US : MAV2CODE LEN30 messageHeaderTailUS ;
ml31US : MAV2CODE LEN31 messageHeaderTailUS ;
ml32US : MAV2CODE LEN32 messageHeaderTailUS ;
ml33US : MAV2CODE LEN33 messageHeaderTailUS ;
ml34US : MAV2CODE LEN34 messageHeaderTailUS ;
ml35US : MAV2CODE LEN35 messageHeaderTailUS ;
ml36US : MAV2CODE LEN36 messageHeaderTailUS ;
ml37US : MAV2CODE LEN37 messageHeaderTailUS ;
ml38US : MAV2CODE LEN38 messageHeaderTailUS ;
ml39US : MAV2CODE LEN39 messageHeaderTailUS ;
ml40US : MAV2CODE LEN40 messageHeaderTailUS ;
ml41US : MAV2CODE LEN41 messageHeaderTailUS ;
ml42US : MAV2CODE LEN42 messageHeaderTailUS ;
ml43US : MAV2CODE LEN43 messageHeaderTailUS ;

/* header starter for messages of the same length, necessary to remove reduce-reduce conflict since the first token that differentiates in the message header is the message code  - signed messages */
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
ml17S : MAV2CODE LEN17 messageHeaderTailS ;
ml18S : MAV2CODE LEN18 messageHeaderTailS ;
ml19S : MAV2CODE LEN19 messageHeaderTailS ;
ml20S : MAV2CODE LEN20 messageHeaderTailS ;
ml21S : MAV2CODE LEN21 messageHeaderTailS ;
ml22S : MAV2CODE LEN22 messageHeaderTailS ;
ml23S : MAV2CODE LEN23 messageHeaderTailS ;
ml24S : MAV2CODE LEN24 messageHeaderTailS ;
ml25S : MAV2CODE LEN25 messageHeaderTailS ;
ml26S : MAV2CODE LEN26 messageHeaderTailS ;
ml27S : MAV2CODE LEN27 messageHeaderTailS ;
ml28S : MAV2CODE LEN28 messageHeaderTailS ;
ml29S : MAV2CODE LEN29 messageHeaderTailS ;
ml30S : MAV2CODE LEN30 messageHeaderTailS ;
ml31S : MAV2CODE LEN31 messageHeaderTailS ;
ml32S : MAV2CODE LEN32 messageHeaderTailS ;
ml33S : MAV2CODE LEN33 messageHeaderTailS ;
ml34S : MAV2CODE LEN34 messageHeaderTailS ;
ml35S : MAV2CODE LEN35 messageHeaderTailS ;
ml36S : MAV2CODE LEN36 messageHeaderTailS ;
ml37S : MAV2CODE LEN37 messageHeaderTailS ;
ml38S : MAV2CODE LEN38 messageHeaderTailS ;
ml39S : MAV2CODE LEN39 messageHeaderTailS ;
ml40S : MAV2CODE LEN40 messageHeaderTailS ;
ml41S : MAV2CODE LEN41 messageHeaderTailS ;
ml42S : MAV2CODE LEN42 messageHeaderTailS ;
ml43S : MAV2CODE LEN43 messageHeaderTailS ;

messageHeaderTailM1 : packetSequence sysID compID ;
messageHeaderTailUS : incompFlagUS compFlag packetSequence sysID compID ;
messageHeaderTailS : incompFlagS compFlag packetSequence sysID compID ;

/* wildcard payloads for messages of various lengths */
PL01 : r__0 ;
PL02 : PL01 r__0 ;
PL03 : PL02 r__0 ;
PL04 : PL03 r__0 ;
PL05 : PL04 r__0 ;
PL06 : PL05 r__0 ;
PL07 : PL06 r__0 ;
PL08 : PL07 r__0 ;
PL09 : PL08 r__0 ;
PL10 : PL09 r__0 ;
PL11 : PL10 r__0 ;
PL12 : PL11 r__0 ;
PL13 : PL12 r__0 ;
PL14 : PL13 r__0 ;
PL15 : PL14 r__0 ;
PL16 : PL15 r__0 ;
PL17 : PL16 r__0 ;
PL18 : PL17 r__0 ;
PL19 : PL18 r__0 ;
PL20 : PL19 r__0 ;
PL21 : PL20 r__0 ;
PL22 : PL21 r__0 ;
PL23 : PL22 r__0 ;
PL24 : PL23 r__0 ;
PL25 : PL24 r__0 ;
PL26 : PL25 r__0 ;
PL27 : PL26 r__0 ;
PL28 : PL27 r__0 ;
PL29 : PL28 r__0 ;
PL30 : PL29 r__0 ;
PL31 : PL30 r__0 ;
PL32 : PL31 r__0 ;
PL33 : PL32 r__0 ;
PL34 : PL33 r__0 ;
PL35 : PL34 r__0 ;
PL36 : PL35 r__0 ;
PL37 : PL36 r__0 ;
PL38 : PL37 r__0 ;
PL39 : PL38 r__0 ;
PL40 : PL39 r__0 ;
PL41 : PL40 r__0 ;
PL42 : PL41 r__0 ;
PL43 : PL42 r__0 ;

/* Tokens */
MAV1CODE : '\xfe' ;
MAV2CODE : '\xfd' ;

HBCODEM1 : X00 ;
HBCODEM2 : HBCODEM1 X00 X00 ;
SSCODEM1 : '\x01' ;
SSCODEM2 : SSCODEM1 X00 X00 ;
STCODEM1 : '\x02' ;
STCODEM2 : STCODEM1 X00 X00 ;
PINGCODEM1 : '\x04' ;
PINGCODEM2 : PINGCODEM1 X00 X00 ;
LNSCODEM1 : '\x08' ;
LNSCODEM2 : LNSCODEM1 X00 X00 ;
SMCODEM1 : '\x0b' ;
SMCODEM2 : SMCODEM1 X00 X00 ;
PRRCODEM1 : '\x14' ;
PRRCODEM2 : PRRCODEM1 X00 X00 ;
PRLCODEM1 : '\x15' ;
PRLCODEM2 : PRLCODEM1 X00 X00 ;
PVCODEM1 : '\x16' ;
PVCODEM2 : PVCODEM1 X00 X00 ;
PSCODEM1 : '\x17' ;
PSCODEM2 : PSCODEM1 X00 X00 ;

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
LEN29 : '\x1d' ;
LEN30 : '\x1e' ;
LEN31 : '\x1f' ;
LEN32 : '\x20' ;
LEN33 : '\x21' ;
LEN34 : '\x22' ;
LEN35 : '\x23' ;
LEN36 : '\x24' ;
LEN37 : '\x25' ;
LEN38 : '\x26' ;
LEN39 : '\x27' ;
LEN40 : '\x28' ;
LEN41 : '\x29' ;
LEN42 : '\x2a' ;
LEN43 : '\x2b' ;

incompFlagUS : X00 ;  /* incompatibility flag for unsigned message */
incompFlagS : '\x01' ;  /* incompatibility flag for signed message */
compFlag : X00 ; /* compatibility flag */
packetSequence : r__0 ;
sysID : r__0 ; /* system ID */
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
r__1 : 
  X00 | '\x01' | '\x02' | '\x03' | '\x04' | '\x05' | '\x06' | '\x07' | 
  '\x08' | '\x09' | '\x0a' | '\x0b' | '\x0c' | '\x0d' | '\x0e' | '\x0f' | 
  '\x10' | '\x11' | '\x12' | '\x13' | '\x14' | '\x15' | '\x16' | '\x17' | 
  '\x18' | '\x19' | '\x1a' | '\x1b' | '\x1c' | '\x1d' | '\x1e' | '\x1f' | 
  '\x20' | '\x21' | '\x22' | '\x23' | '\x24' | '\x25' | '\x26' | '\x27' | 
  '\x28' | '\x29' | '\x2a' | '\x2b' | '\x2c' ;
r__2 : 
  X00 | '\x01' | '\x02' | '\x03' | '\x04' | '\x05' | '\x06' | '\x07' | 
  '\x08' | '\x09' | '\x0a' | '\x0b' | '\x0c' | '\x0d' | '\x0e' | '\x0f' | 
  '\x10' | '\x11' | '\x12' | '\x13' | '\x14' ;
r__3 : 
  X00 | '\x01' | '\x02' | '\x03' | '\x04' | '\x05' | '\x06' | '\x07' | 
  '\x08' ;
r__4 : 
  '\x01' | '\x02' | '\x03' | '\x04' | '\x05' | '\x06' | '\x07' | '\x08' | 
  '\x09' | '\x0a' ;
r__5 : 
  '\x01' | '\x02' | '\x03' | '\x04' | '\x05' | '\x06' | '\x07' | '\x08' | 
  '\x09' | '\x0a' ;
