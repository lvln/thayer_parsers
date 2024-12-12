%{
  #define YYDEBUG 1
  int yylex(void);
  void yyerror(char *s);
%}
%token X00
%%
P : optMessage ;
optMessage : /* empty */ | optMessage message ;

/* a message may be either a MAVLink 1, a signed or an unsigned message */
message : messageM1 | messageUS | messageS ;

/* MAVLink 1 messages have a slightly different header fields and cannot contain a signature */
messageM1 : messageOptM1 crc ;
messageOptM1 : hbM1 | ssM1 | stM1 | pingM1 | lnsM1 | smM1 | prrM1 | prlM1 | pvM1 | psM1 | griM1 | spM1 | attM1 | aqM1 | lpnM1 | gpiM1 | mrM1 | mcM1 | mrlM1 | mctM1 | mcaM1 | mirM1 | maM1 | ggoM1 | mriM1 | rcM1 | mclM1 | miiM1 | vhM1 | clM1 | caM1 | atM1 | ptlnM1 | ptgiM1 | rsM1 | dsM1 | altM1 | bsM1 | avM1 | esM1 | vbM1 | hpM1 | essM1 | sM1 ;

/* unsigned messsages contain a header with a payload and finally a checksum */
messageUS : messageOptUS crc ;
messageOptUS : genericMessUS | hbUS | pvUS | psUS | griUS | mrUS | mcUS | mrlUS | mctUS | mcaUS | maUS | mriUS | miiUS | caUS | ptlnUS | ptgiUS | dsUS | bsUS | essUS | sUS | ugpUS | odilUS | odisUS ;

/* signed messages contain a header with a payload, followed by a checksum and finally a 13 byte signature */
messageS : messageOptS crc sig ;
messageOptS : genericMessS | hbS | pvS | psS | griS | mrS | mcS | mrlS | mctS | mcaS | maS | mriS |  miiS | caS | ptlnS | ptgiS | dsS | bsS | essS | sS | ugpS | odilS | odisS ;

/* generic unsigned messages - messages with all wildcards in payload */
genericMessUS : generic01US | generic02US | generic03US | generic04US | generic05US | generic06US | generic07US | generic08US | generic09US | generic10US | generic11US | generic12US | generic13US | generic14US | generic15US | generic16US | generic17US | generic18US | generic19US | generic20US | generic21US | generic22US | generic23US | generic24US | generic25US | generic26US | generic27US | generic28US | generic29US | generic30US | generic31US | generic32US | generic33US | generic34US | generic35US | generic36US | generic37US | generic38US | generic39US | generic40US | generic41US | generic42US | generic43US | generic44US | generic45US | generic46US | generic47US | generic48US | generic49US | generic50US | generic51US | generic52US | generic53US | generic54US | generic55US | generic56US | generic57US | generic58US | generic59US | generic60US | generic61US | generic62US | generic63US | generic64US | generic65US | generic66US | generic67US | generic68US | generic69US | generic70US | generic71US | generic72US | generic73US | generic74US | generic75US | generic76US | generic77US | generic78US | generic79US | generic80US | generic81US | generic82US | generic83US | generic84US | generic85US | generic86US | generic87US | generic88US | generic89US | generic90US | generic91US | generic92US | generic93US | generic94US | generic95US | generic96US | generic97US | generic98US | generic99US | generic100US | generic101US | generic102US | generic103US | generic104US | generic105US | generic106US | generic107US | generic108US ; 

/* generic signed messages - messages with all wildcards in payload */
genericMessS : generic01S | generic02S | generic03S | generic04S | generic05S | generic06S | generic07S | generic08S | generic09S | generic10S | generic11S | generic12S | generic13S | generic14S | generic15S | generic16S | generic17S | generic18S | generic19S | generic20S | generic21S | generic22S | generic23S | generic24S | generic25S | generic26S | generic27S | generic28S | generic29S | generic30S | generic31S | generic32S | generic33S | generic34S | generic35S | generic36S | generic37S | generic38S | generic39S | generic40S | generic41S | generic42S | generic43S | generic44S | generic45S | generic46S | generic47S | generic48S | generic49S | generic50S | generic51S | generic52S | generic53S | generic54S | generic55S | generic56S | generic57S | generic58S | generic59S | generic60S | generic61S | generic62S | generic63S | generic64S | generic65S | generic66S | generic67S | generic68S | generic69S | generic70S | generic71S | generic72S | generic73S | generic74S | generic75S | generic76S | generic77S | generic78S | generic79S | generic80S | generic81S | generic82S | generic83S | generic84S | generic85S | generic86S | generic87S | generic88S | generic89S | generic90S | generic91S | generic92S | generic93S | generic94S | generic95S | generic96S | generic97S | generic98S | generic99S | generic100S | generic101S | generic102S | generic103S | generic104S | generic105S | generic106S | generic107S | generic108S ; 

/* generic unsigned messages */
generic01US : ml01US genericCodes01to02 PL01 ;
generic02US : ml02US genericCodes01to02 PL02 ;
generic03US : ml03US genericCodes03 PL03 ;
generic04US : ml04US genericCodes04 PL04 ;
generic05US : ml05US genericCodes05to06 PL05 ;
generic06US : ml06US genericCodes05to06 PL06 ;
generic07US : ml07US genericCodes07to09 PL07 ;
generic08US : ml08US genericCodes07to09 PL08 ;
generic09US : ml09US genericCodes07to09 PL09 ;
generic10US : ml10US genericCodes10 PL10 ;
generic11US : ml11US genericCodes11to12 PL11 ;
generic12US : ml12US genericCodes11to12 PL12 ;
generic13US : ml13US genericCodes13to14 PL13 ;
generic14US : ml14US genericCodes13to14 PL14 ;
generic15US : ml15US genericCodes15to16 PL15 ;
generic16US : ml16US genericCodes15to16 PL16 ;
generic17US : ml17US genericCodes17to20 PL17 ;
generic18US : ml18US genericCodes17to20 PL18 ;
generic19US : ml19US genericCodes17to20 PL19 ;
generic20US : ml20US genericCodes17to20 PL20 ;
generic21US : ml21US genericCodes21to22 PL21 ;
generic22US : ml22US genericCodes21to22 PL22 ;
generic23US : ml23US genericCodes23to24 PL23 ;
generic24US : ml24US genericCodes23to24 PL24 ;
generic25US : ml25US genericCodes25to28 PL25 ;
generic26US : ml26US genericCodes25to28 PL26 ;
generic27US : ml27US genericCodes25to28 PL27 ;
generic28US : ml28US genericCodes25to28 PL28 ;
generic29US : ml29US genericCodes29to30 PL29 ;
generic30US : ml30US genericCodes29to30 PL30 ;
generic31US : ml31US genericCodes31to32 PL31 ;
generic32US : ml32US genericCodes31to32 PL32 ;
generic33US : ml33US genericCodes33 PL33 ;
generic34US : ml34US genericCodes34 PL34 ;
generic35US : ml35US genericCodes35to36 PL35 ;
generic36US : ml36US genericCodes35to36 PL36 ;
generic37US : ml37US genericCodes37 PL37 ;
generic38US : ml38US genericCodes38to42 PL38 ;
generic39US : ml39US genericCodes38to42 PL39 ;
generic40US : ml40US genericCodes38to42 PL40 ;
generic41US : ml41US genericCodes38to42 PL41 ;
generic42US : ml42US genericCodes38to42 PL42 ;
generic43US : ml43US genericCodes43 PL43 ;
generic44US : ml44US genericCodes44to48 PL44 ;
generic45US : ml45US genericCodes44to48 PL45 ;
generic46US : ml46US genericCodes44to48 PL46 ;
generic47US : ml47US genericCodes44to48 PL47 ;
generic48US : ml48US genericCodes44to48 PL48 ;
generic49US : ml49US genericCodes49to50 PL49 ;
generic50US : ml50US genericCodes49to50 PL50 ;
generic51US : ml51US genericCodes51to52 PL51 ;
generic52US : ml52US genericCodes51to52 PL52 ;
generic53US : ml53US genericCodes53 PL53 ;
generic54US : ml54US genericCodes54to60 PL54 ;
generic55US : ml55US genericCodes54to60 PL55 ;
generic56US : ml56US genericCodes54to60 PL56 ;
generic57US : ml57US genericCodes54to60 PL57 ;
generic58US : ml58US genericCodes54to60 PL58 ;
generic59US : ml59US genericCodes54to60 PL59 ;
generic60US : ml60US genericCodes54to60 PL60 ;
generic61US : ml61US genericCodes61to68 PL61 ;
generic62US : ml62US genericCodes61to68 PL62 ;
generic63US : ml63US genericCodes61to68 PL63 ;
generic64US : ml64US genericCodes61to68 PL64 ;
generic65US : ml65US genericCodes61to68 PL65 ;
generic66US : ml66US genericCodes61to68 PL66 ;
generic67US : ml67US genericCodes61to68 PL67 ;
generic68US : ml68US genericCodes61to68 PL68 ;
generic69US : ml69US genericCodes69to78 PL69 ;
generic70US : ml70US genericCodes69to78 PL70 ;
generic71US : ml71US genericCodes69to78 PL71 ;
generic72US : ml72US genericCodes69to78 PL72 ;
generic73US : ml73US genericCodes69to78 PL73 ;
generic74US : ml74US genericCodes69to78 PL74 ;
generic75US : ml75US genericCodes69to78 PL75 ;
generic76US : ml76US genericCodes69to78 PL76 ;
generic77US : ml77US genericCodes69to78 PL77 ;
generic78US : ml78US genericCodes69to78 PL78 ;
generic79US : ml79US genericCodes79to108 PL79 ;
generic80US : ml80US genericCodes79to108 PL80 ;
generic81US : ml81US genericCodes79to108 PL81 ;
generic82US : ml82US genericCodes79to108 PL82 ;
generic83US : ml83US genericCodes79to108 PL83 ;
generic84US : ml84US genericCodes79to108 PL84 ;
generic85US : ml85US genericCodes79to108 PL85 ;
generic86US : ml86US genericCodes79to108 PL86 ;
generic87US : ml87US genericCodes79to108 PL87 ;
generic88US : ml88US genericCodes79to108 PL88 ;
generic89US : ml89US genericCodes79to108 PL89 ;
generic90US : ml90US genericCodes79to108 PL90 ;
generic91US : ml91US genericCodes79to108 PL91 ;
generic92US : ml92US genericCodes79to108 PL92 ;
generic93US : ml93US genericCodes79to108 PL93 ;
generic94US : ml94US genericCodes79to108 PL94 ;
generic95US : ml95US genericCodes79to108 PL95 ;
generic96US : ml96US genericCodes79to108 PL96 ;
generic97US : ml97US genericCodes79to108 PL97 ;
generic98US : ml98US genericCodes79to108 PL98 ;
generic99US : ml99US genericCodes79to108 PL99 ;
generic100US : ml100US genericCodes79to108 PL100 ;
generic101US : ml101US genericCodes79to108 PL101 ;
generic102US : ml102US genericCodes79to108 PL102 ;
generic103US : ml103US genericCodes79to108 PL103 ;
generic104US : ml104US genericCodes79to108 PL104 ;
generic105US : ml105US genericCodes79to108 PL105 ;
generic106US : ml106US genericCodes79to108 PL106 ;
generic107US : ml107US genericCodes79to108 PL107 ;
generic108US : ml108US genericCodes79to108 PL108 ;

/* generic signed messages */
generic01S : ml01S genericCodes01to02 PL01 ;
generic02S : ml02S genericCodes01to02 PL02 ;
generic03S : ml03S genericCodes03 PL03 ;
generic04S : ml04S genericCodes04 PL04 ;
generic05S : ml05S genericCodes05to06 PL05 ;
generic06S : ml06S genericCodes05to06 PL06 ;
generic07S : ml07S genericCodes07to09 PL07 ;
generic08S : ml08S genericCodes07to09 PL08 ;
generic09S : ml09S genericCodes07to09 PL09 ;
generic10S : ml10S genericCodes10 PL10 ;
generic11S : ml11S genericCodes11to12 PL11 ;
generic12S : ml12S genericCodes11to12 PL12 ;
generic13S : ml13S genericCodes13to14 PL13 ;
generic14S : ml14S genericCodes13to14 PL14 ;
generic15S : ml15S genericCodes15to16 PL15 ;
generic16S : ml16S genericCodes15to16 PL16 ;
generic17S : ml17S genericCodes17to20 PL17 ;
generic18S : ml18S genericCodes17to20 PL18 ;
generic19S : ml19S genericCodes17to20 PL19 ;
generic20S : ml20S genericCodes17to20 PL20 ;
generic21S : ml21S genericCodes21to22 PL21 ;
generic22S : ml22S genericCodes21to22 PL22 ;
generic23S : ml23S genericCodes23to24 PL23 ;
generic24S : ml24S genericCodes23to24 PL24 ;
generic25S : ml25S genericCodes25to28 PL25 ;
generic26S : ml26S genericCodes25to28 PL26 ;
generic27S : ml27S genericCodes25to28 PL27 ;
generic28S : ml28S genericCodes25to28 PL28 ;
generic29S : ml29S genericCodes29to30 PL29 ;
generic30S : ml30S genericCodes29to30 PL30 ;
generic31S : ml31S genericCodes31to32 PL31 ;
generic32S : ml32S genericCodes31to32 PL32 ;
generic33S : ml33S genericCodes33 PL33 ;
generic34S : ml34S genericCodes34 PL34 ;
generic35S : ml35S genericCodes35to36 PL35 ;
generic36S : ml36S genericCodes35to36 PL36 ;
generic37S : ml37S genericCodes37 PL37 ;
generic38S : ml38S genericCodes38to42 PL38 ;
generic39S : ml39S genericCodes38to42 PL39 ;
generic40S : ml40S genericCodes38to42 PL40 ;
generic41S : ml41S genericCodes38to42 PL41 ;
generic42S : ml42S genericCodes38to42 PL42 ;
generic43S : ml43S genericCodes43 PL43 ;
generic44S : ml44S genericCodes44to48 PL44 ;
generic45S : ml45S genericCodes44to48 PL45 ;
generic46S : ml46S genericCodes44to48 PL46 ;
generic47S : ml47S genericCodes44to48 PL47 ;
generic48S : ml48S genericCodes44to48 PL48 ;
generic49S : ml49S genericCodes49to50 PL49 ;
generic50S : ml50S genericCodes49to50 PL50 ;
generic51S : ml51S genericCodes51to52 PL51 ;
generic52S : ml52S genericCodes51to52 PL52 ;
generic53S : ml53S genericCodes53 PL53 ;
generic54S : ml54S genericCodes54to60 PL54 ;
generic55S : ml55S genericCodes54to60 PL55 ;
generic56S : ml56S genericCodes54to60 PL56 ;
generic57S : ml57S genericCodes54to60 PL57 ;
generic58S : ml58S genericCodes54to60 PL58 ;
generic59S : ml59S genericCodes54to60 PL59 ;
generic60S : ml60S genericCodes54to60 PL60 ;
generic61S : ml61S genericCodes61to68 PL61 ;
generic62S : ml62S genericCodes61to68 PL62 ;
generic63S : ml63S genericCodes61to68 PL63 ;
generic64S : ml64S genericCodes61to68 PL64 ;
generic65S : ml65S genericCodes61to68 PL65 ;
generic66S : ml66S genericCodes61to68 PL66 ;
generic67S : ml67S genericCodes61to68 PL67 ;
generic68S : ml68S genericCodes61to68 PL68 ;
generic69S : ml69S genericCodes69to78 PL69 ;
generic70S : ml70S genericCodes69to78 PL70 ;
generic71S : ml71S genericCodes69to78 PL71 ;
generic72S : ml72S genericCodes69to78 PL72 ;
generic73S : ml73S genericCodes69to78 PL73 ;
generic74S : ml74S genericCodes69to78 PL74 ;
generic75S : ml75S genericCodes69to78 PL75 ;
generic76S : ml76S genericCodes69to78 PL76 ;
generic77S : ml77S genericCodes69to78 PL77 ;
generic78S : ml78S genericCodes69to78 PL78 ;
generic79S : ml79S genericCodes79to108 PL79 ;
generic80S : ml80S genericCodes79to108 PL80 ;
generic81S : ml81S genericCodes79to108 PL81 ;
generic82S : ml82S genericCodes79to108 PL82 ;
generic83S : ml83S genericCodes79to108 PL83 ;
generic84S : ml84S genericCodes79to108 PL84 ;
generic85S : ml85S genericCodes79to108 PL85 ;
generic86S : ml86S genericCodes79to108 PL86 ;
generic87S : ml87S genericCodes79to108 PL87 ;
generic88S : ml88S genericCodes79to108 PL88 ;
generic89S : ml89S genericCodes79to108 PL89 ;
generic90S : ml90S genericCodes79to108 PL90 ;
generic91S : ml91S genericCodes79to108 PL91 ;
generic92S : ml92S genericCodes79to108 PL92 ;
generic93S : ml93S genericCodes79to108 PL93 ;
generic94S : ml94S genericCodes79to108 PL94 ;
generic95S : ml95S genericCodes79to108 PL95 ;
generic96S : ml96S genericCodes79to108 PL96 ;
generic97S : ml97S genericCodes79to108 PL97 ;
generic98S : ml98S genericCodes79to108 PL98 ;
generic99S : ml99S genericCodes79to108 PL99 ;
generic100S : ml100S genericCodes79to108 PL100 ;
generic101S : ml101S genericCodes79to108 PL101 ;
generic102S : ml102S genericCodes79to108 PL102 ;
generic103S : ml103S genericCodes79to108 PL103 ;
generic104S : ml104S genericCodes79to108 PL104 ;
generic105S : ml105S genericCodes79to108 PL105 ;
generic106S : ml106S genericCodes79to108 PL106 ;
generic107S : ml107S genericCodes79to108 PL107 ;
generic108S : ml108S genericCodes79to108 PL108 ;

/* codes for generic message payloads of various lengths */
genericCodes01to02 : PRLCODEM2 | MRLCODEM2 | MCACODEM2 | MIRCODEM2 | MACODEM2 | CACODEM2 | genericCodes03 ;
genericCodes03 : CESCODE | genericCodes04 ;
genericCodes04 : HBCODEM2 | MRCODEM2 | MCCODEM2 | MCTCODEM2 | MRICODEM2 | genericCodes05to06 ;
genericCodes05to06 : SMCODEM2 | genericCodes07to09 ;
genericCodes07to09 : RSCODEM2 | genericCodes10 ;
genericCodes10 : DSCODEM2 | genericCodes11to12 ;
genericCodes11to12 : STCODEM2 | genericCodes13to14 ;
genericCodes13to14 : PINGCODEM2 | genericCodes15to16 ;
genericCodes15to16 : SPCODEM2 | genericCodes17to20 ;
genericCodes17to20 : GGOCODEM2 | VHCODEM2 | TETTCODE | genericCodes21to22 ;
genericCodes21to22 : PRRCODEM2 | PSCODEM2 | PVNCODE | genericCodes23to24 ;
genericCodes23to24 : PVCODEM2 | genericCodes25to28 ;
genericCodes25to28 : GRICODEM2 | ATTCODEM2 | LPNCODEM2 | GPICODEM2 | genericCodes29to30 ;
genericCodes29to30 : MCLCODEM2 | genericCodes31to32 ;
genericCodes31to32 : ALTCODEM2 | VBCODEM2 | genericCodes33 ;
genericCodes33 : CLCODEM2 | BSCODEM2 | genericCodes34 ;
genericCodes34 : MIICODEM2 | genericCodes35to36 ;
genericCodes35to36 : LNSCODEM2 | genericCodes37 ;
genericCodes37 : ATCODEM2 | genericCodes38to42 ;
genericCodes38to42 : RCCODEM2 | ESCODEM2 | genericCodes43 ;
genericCodes43 : SSCODEM2 | genericCodes44to48 ;
genericCodes44to48 : AQCODEM2 | genericCodes49to50 ;
genericCodes49to50 : PTLNCODEM2 | PTGICODEM2 | ODISCODE | genericCodes51to52 ;
genericCodes51to52 : ODILCODE | genericCodes53 ;
genericCodes53 : EVCODE | genericCodes54to60 ;
genericCodes54to60 : HPCODEM2 | genericCodes61to68 ;
genericCodes61to68 : UGPCODE | genericCodes69to78 ;
genericCodes69to78 : AVCODEM2 | genericCodes79to108 ;
genericCodes79to108 : CMCODE ;

/* HEARTBEAT (0) */
hbM1 : ml09M1 HBCODEM1 hbPL09 ;
hbUS : hb05US | hb06US | hb07US | hb08US | hb09US ;
hbS : hb05S | hb06S | hb07S | hb08S | hb09S ;
hb05US : ml05US HBCODEM2 hbPL05 ;
hb06US : ml06US HBCODEM2 hbPL06 ;
hb07US : ml07US HBCODEM2 hbPL07 ;
hb08US : ml08US HBCODEM2 hbPL08 ;
hb09US : ml09US HBCODEM2 hbPL09 ;
hb05S : ml05S HBCODEM2 hbPL05 ;
hb06S : ml06S HBCODEM2 hbPL06 ;
hb07S : ml07S HBCODEM2 hbPL07 ;
hb08S : ml08S HBCODEM2 hbPL08 ;
hb09S : ml09S HBCODEM2 hbPL09 ;
hbPL05 : PL04 e17 ;
hbPL06 : hbPL05 e15 ;
hbPL07 : hbPL06 PL01 ;
hbPL08 : hbPL07 e4 ;
hbPL09 : hbPL08 PL01 ;

/* SYS_STATUS (1) */
ssM1 : ml31M1 SSCODEM1 PL31 ;

/* SYSTEM_TIME (2) */
stM1 : ml12M1 STCODEM1 PL12 ;

/* PING (4) */
pingM1 : ml14M1 PINGCODEM1 PL14 ;

/* LINK_NODE_STATUS (8) */
lnsM1 : ml36M1 LNSCODEM1 PL36 ;

/* SET_MODE (11) */
smM1 : ml06M1 SMCODEM1 PL06 ;

/* PARAM_REQUEST_READ (20) */
prrM1 : ml22M1 PRRCODEM1 PL22 ;

/* PARAM_REQUEST_LIST (21) */
prlM1 : ml02M1 PRLCODEM1 PL02 ;

/* PARAM_VALUE (22) */
pvM1 : ml25M1 PVCODEM1 pvPL25 ;
pvUS : ml25US PVCODEM2 pvPL25 ;
pvS : ml25S PVCODEM2 pvPL25 ;
pvPL25 : PL24 e2 ;

/* PARAM_SET (23) */
psM1 : ml23M1 PSCODEM1 psPL23 ;
psUS : ml23US PSCODEM2 psPL23 ;
psS : ml23S PSCODEM2 psPL23 ;
psPL23 : PL22 e2 ;

/* GPS_RAW_INT (24) */
griM1 : ml30M1 GRICODEM1 griPL30 ;
griUS : gri29US | gri30US | gri31US | gri32US | gri33US | gri34US | gri35US | gri36US | gri37US | gri38US | gri39US | gri40US | gri41US | gri42US | gri43US | gri44US | gri45US | gri46US | gri47US | gri48US | gri49US | gri50US | gri51US | gri52US ;
griS : gri29S | gri30S | gri31S | gri32S | gri33S | gri34S | gri35S | gri36S | gri37S | gri38S | gri39S | gri40S | gri41S | gri42S | gri43S | gri44S | gri45S | gri46S | gri47S | gri48S | gri49S | gri50S | gri51S | gri52S ;
gri29US : ml29US GRICODEM2 griPL29 ;
gri30US : ml30US GRICODEM2 griPL30 ;
gri31US : ml31US GRICODEM2 griPL31 ;
gri32US : ml32US GRICODEM2 griPL32 ;
gri33US : ml33US GRICODEM2 griPL33 ;
gri34US : ml34US GRICODEM2 griPL34 ;
gri35US : ml35US GRICODEM2 griPL35 ;
gri36US : ml36US GRICODEM2 griPL36 ;
gri37US : ml37US GRICODEM2 griPL37 ;
gri38US : ml38US GRICODEM2 griPL38 ;
gri39US : ml39US GRICODEM2 griPL39 ;
gri40US : ml40US GRICODEM2 griPL40 ;
gri41US : ml41US GRICODEM2 griPL41 ;
gri42US : ml42US GRICODEM2 griPL42 ;
gri43US : ml43US GRICODEM2 griPL43 ;
gri44US : ml44US GRICODEM2 griPL44 ;
gri45US : ml45US GRICODEM2 griPL45 ;
gri46US : ml46US GRICODEM2 griPL46 ;
gri47US : ml47US GRICODEM2 griPL47 ;
gri48US : ml48US GRICODEM2 griPL48 ;
gri49US : ml49US GRICODEM2 griPL49 ;
gri50US : ml50US GRICODEM2 griPL50 ;
gri51US : ml51US GRICODEM2 griPL51 ;
gri52US : ml52US GRICODEM2 griPL52 ;
gri29S : ml29S GRICODEM2 griPL29 ;
gri30S : ml30S GRICODEM2 griPL30 ;
gri31S : ml31S GRICODEM2 griPL31 ;
gri32S : ml32S GRICODEM2 griPL32 ;
gri33S : ml33S GRICODEM2 griPL33 ;
gri34S : ml34S GRICODEM2 griPL34 ;
gri35S : ml35S GRICODEM2 griPL35 ;
gri36S : ml36S GRICODEM2 griPL36 ;
gri37S : ml37S GRICODEM2 griPL37 ;
gri38S : ml38S GRICODEM2 griPL38 ;
gri39S : ml39S GRICODEM2 griPL39 ;
gri40S : ml40S GRICODEM2 griPL40 ;
gri41S : ml41S GRICODEM2 griPL41 ;
gri42S : ml42S GRICODEM2 griPL42 ;
gri43S : ml43S GRICODEM2 griPL43 ;
gri44S : ml44S GRICODEM2 griPL44 ;
gri45S : ml45S GRICODEM2 griPL45 ;
gri46S : ml46S GRICODEM2 griPL46 ;
gri47S : ml47S GRICODEM2 griPL47 ;
gri48S : ml48S GRICODEM2 griPL48 ;
gri49S : ml49S GRICODEM2 griPL49 ;
gri50S : ml50S GRICODEM2 griPL50 ;
gri51S : ml51S GRICODEM2 griPL51 ;
gri52S : ml52S GRICODEM2 griPL52 ;
griPL29 : PL28 e4 ;
griPL30 : griPL29 PL01 ;
griPL31 : griPL30 PL01 ;
griPL32 : griPL31 PL01 ;
griPL33 : griPL32 PL01 ;
griPL34 : griPL33 PL01 ;
griPL35 : griPL34 PL01 ;
griPL36 : griPL35 PL01 ;
griPL37 : griPL36 PL01 ;
griPL38 : griPL37 PL01 ;
griPL39 : griPL38 PL01 ;
griPL40 : griPL39 PL01 ;
griPL41 : griPL40 PL01 ;
griPL42 : griPL41 PL01 ;
griPL43 : griPL42 PL01 ;
griPL44 : griPL43 PL01 ;
griPL45 : griPL44 PL01 ;
griPL46 : griPL45 PL01 ;
griPL47 : griPL46 PL01 ;
griPL48 : griPL47 PL01 ;
griPL49 : griPL48 PL01 ;
griPL50 : griPL49 PL01 ;
griPL51 : griPL50 PL01 ;
griPL52 : griPL51 PL01 ;

/* SCALED_PRESSURE (29) */
spM1 : ml14M1 SPCODEM1 PL14 ;

/* ATTITUDE (30) */
attM1 : ml28M1 ATTCODEM1 PL28 ;

/* ATTITUDE_QUATERNION (31) */
aqM1 : ml32M1 AQCODEM1 PL32 ;

/* LOCAL_POSITION_NED (32) */
lpnM1 : ml28M1 LPNCODEM1 PL28 ;

/* GLOBAL_POSITION_INT (33) */
gpiM1 : ml28M1 GPICODEM1 PL28 ;

/* MISSION_REQUEST (40) */
mrM1 : ml04M1 MRCODEM1 PL04 ;
mrUS : ml05US MRCODEM2 mrPL05 ;
mrS : ml05S MRCODEM2 mrPL05 ;
mrPL05 : PL04 e1 ;

/* MISSION_CURRENT (42) */
mcM1 : ml02M1 MCCODEM1 PL02 ;
mcUS : mc05US | mc06US | mc07US | mc08US | mc09US | mc10US | mc11US | mc12US | mc13US | mc14US | mc15US | mc16US | mc17US | mc18US ;
mcS : mc05S | mc06S | mc07S | mc08S | mc09S | mc10S | mc11S | mc12S | mc13S | mc14S | mc15S | mc16S | mc17S | mc18S ;
mc05US : ml05US MCCODEM2 mcPL05 ;
mc06US : ml06US MCCODEM2 mcPL06 ;
mc07US : ml07US MCCODEM2 mcPL07 ;
mc08US : ml08US MCCODEM2 mcPL08 ; 
mc09US : ml09US MCCODEM2 mcPL09 ;
mc10US : ml10US MCCODEM2 mcPL10 ;
mc11US : ml11US MCCODEM2 mcPL11 ;
mc12US : ml12US MCCODEM2 mcPL12 ;
mc13US : ml13US MCCODEM2 mcPL13 ;
mc14US : ml14US MCCODEM2 mcPL14 ;
mc15US : ml15US MCCODEM2 mcPL15 ;
mc16US : ml16US MCCODEM2 mcPL16 ;
mc17US : ml17US MCCODEM2 mcPL17 ;
mc18US : ml18US MCCODEM2 mcPL18 ;
mc05S : ml05S MCCODEM2 mcPL05 ;
mc06S : ml06S MCCODEM2 mcPL06 ;
mc07S : ml07S MCCODEM2 mcPL07 ;
mc08S : ml08S MCCODEM2 mcPL08 ; 
mc09S : ml09S MCCODEM2 mcPL09 ;
mc10S : ml10S MCCODEM2 mcPL10 ;
mc11S : ml11S MCCODEM2 mcPL11 ;
mc12S : ml12S MCCODEM2 mcPL12 ;
mc13S : ml13S MCCODEM2 mcPL13 ;
mc14S : ml14S MCCODEM2 mcPL14 ;
mc15S : ml15S MCCODEM2 mcPL15 ;
mc16S : ml16S MCCODEM2 mcPL16 ;
mc17S : ml17S MCCODEM2 mcPL17 ;
mc18S : ml18S MCCODEM2 mcPL18 ;
mcPL05 : PL04 e8 ;
mcPL06 : mcPL05 e5 ;
mcPL07 : mcPL06 PL01 ;
mcPL08 : mcPL07 PL01 ;
mcPL09 : mcPL08 PL01 ;
mcPL10 : mcPL09 PL01 ;
mcPL11 : mcPL10 PL01 ;
mcPL12 : mcPL11 PL01 ;
mcPL13 : mcPL12 PL01 ;
mcPL14 : mcPL13 PL01 ;
mcPL15 : mcPL14 PL01 ;
mcPL16 : mcPL15 PL01 ;
mcPL17 : mcPL16 PL01 ;
mcPL18 : mcPL17 PL01 ;

/* MISSION_REQUEST_LIST (43) */
mrlM1 : ml02M1 MRLCODEM1 PL02 ;
mrlUS : ml03US MRLCODEM2 mrlPL03 ;
mrlS : ml03S MRLCODEM2 mrlPL03 ;
mrlPL03 : PL02 e1 ;

/* MISSION_COUNT (44) */
mctM1 : ml04M1 MCTCODEM1 PL04 ;
mctUS : mct05US | mct06US | mct07US | mct08US | mct09US ;
mctS : mct05S | mct06S | mct07S | mct08S | mct09S ;
mct05US : ml05US MCTCODEM2 mctPL05 ;
mct06US : ml06US MCTCODEM2 mctPL06 ;
mct07US : ml07US MCTCODEM2 mctPL07 ;
mct08US : ml08US MCTCODEM2 mctPL08 ;
mct09US : ml09US MCTCODEM2 mctPL09 ;
mct05S : ml05S MCTCODEM2 mctPL05 ;
mct06S : ml06S MCTCODEM2 mctPL06 ;
mct07S : ml07S MCTCODEM2 mctPL07 ;
mct08S : ml08S MCTCODEM2 mctPL08 ;
mct09S : ml09S MCTCODEM2 mctPL09 ;
mctPL05 : PL04 e1 ;
mctPL06 : mctPL05 PL01 ;
mctPL07 : mctPL06 PL01 ;
mctPL08 : mctPL07 PL01 ;
mctPL09 : mctPL08 PL01 ; 

/* MISSION_CLEAR_ALL (45) */
mcaM1 : ml02M1 MCACODEM1 PL02 ;
mcaUS : ml03US MCACODEM2 mcaPL03 ;
mcaS : ml03S MCACODEM2 mcaPL03 ;
mcaPL03 : PL02 e1 ;

/* MISSION_ITEM_REACHED (46) */
mirM1 : ml02M1 MIRCODEM1 PL02 ;

/* MISSION_ACK (47) */
maM1 : ml03M1 MACODEM1 maPL03 ;
maUS : ma03US | ma04US | ma05US | ma06US | ma07US | ma08US ;
maS : ma03S | ma04S | ma05S | ma06S | ma07S | ma08S ;
ma03US : ml03US MACODEM2 maPL03 ;
ma04US : ml04US MACODEM2 maPL04 ;
ma05US : ml05US MACODEM2 maPL05 ;
ma06US : ml06US MACODEM2 maPL06 ;
ma07US : ml07US MACODEM2 maPL07 ;
ma08US : ml08US MACODEM2 maPL08 ;
ma03S : ml03S MACODEM2 maPL03 ;
ma04S : ml04S MACODEM2 maPL04 ;
ma05S : ml05S MACODEM2 maPL05 ;
ma06S : ml06S MACODEM2 maPL06 ;
ma07S : ml07S MACODEM2 maPL07 ;
ma08S : ml08S MACODEM2 maPL08 ;
maPL03 : PL02 e14 ;
maPL04 : maPL03 e1 ;
maPL05 : maPL04 PL01 ;
maPL06 : maPL05 PL01 ;
maPL07 : maPL06 PL01 ;
maPL08 : maPL07 PL01 ;

/* GPS_GLOBAL_ORIGIN (49) */
ggoM1 : ml20M1 GGOCODEM1 PL20 ;

/* MISSION_REQUEST_INT (51) */
mriM1 : ml04M1 MRICODEM1 PL04 ;
mriUS : ml05US MRICODEM2 mriPL05 ;
mriS : ml05S MRICODEM2 mriPL05 ;
mriPL05 : PL04 e1 ;

/* RC_CHANNELS (65) */
rcM1 : ml42M1 RCCODEM1 PL42 ;

/* MANUAL_CONTROL (69) */
mclM1 : ml11M1 MCLCODEM1 PL11 ;

/* MISSION_ITEM_INT (73) */
miiM1 : ml37M1 MIICODEM1 miiPL37 ;
miiUS : mii35US | mii36US | mii37US | mii38US ;
miiS : mii35S | mii36S | mii37S | mii38S ;
mii35US : ml35US MIICODEM2 miiPL35 ;
mii36US : ml36US MIICODEM2 miiPL36 ;
mii37US : ml37US MIICODEM2 miiPL37 ;
mii38US : ml38US MIICODEM2 miiPL38 ;
mii35S : ml35S MIICODEM2 miiPL35 ;
mii36S : ml36S MIICODEM2 miiPL36 ;
mii37S : ml37S MIICODEM2 miiPL37 ;
mii38S : ml38S MIICODEM2 miiPL38 ;
miiPL35 : PL34 e16 ;
miiPL36 : miiPL35 PL01 ;
miiPL37 : miiPL36 PL01 ;
miiPL38 : miiPL37 e1 ;

/* VFR_HUD (74) */
vhM1 : ml20M1 VHCODEM1 PL20 ;

/* COMMAND_LONG (76) */
clM1 : ml33M1 CLCODEM1 PL33 ;

/* COMMAND_ACK (77) */
caM1 : ml03M1 CACODEM1 caPL03 ;
caUS : ca03US | ca04US | ca05US | ca06US | ca07US | ca08US | ca09US | ca10US ;
caS : ca03S | ca04S | ca05S | ca06S | ca07S | ca08S | ca09S | ca10S ;
ca03US : ml03US CACODEM2 caPL03 ;
ca04US : ml04US CACODEM2 caPL04 ;
ca05US : ml05US CACODEM2 caPL05 ;
ca06US : ml06US CACODEM2 caPL06 ;
ca07US : ml07US CACODEM2 caPL07 ;
ca08US : ml08US CACODEM2 caPL08 ;
ca09US : ml09US CACODEM2 caPL09 ;
ca10US : ml10US CACODEM2 caPL10 ;
ca03S : ml03S CACODEM2 caPL03 ;
ca04S : ml04S CACODEM2 caPL04 ;
ca05S : ml05S CACODEM2 caPL05 ;
ca06S : ml06S CACODEM2 caPL06 ;
ca07S : ml07S CACODEM2 caPL07 ;
ca08S : ml08S CACODEM2 caPL08 ;
ca09S : ml09S CACODEM2 caPL09 ;
ca10S : ml10S CACODEM2 caPL10 ;
caPL03 : PL02 e11 ;
caPL04 : caPL03 PL01 ;
caPL05 : caPL04 PL01 ;
caPL06 : caPL05 PL01 ;
caPL07 : caPL06 PL01 ;
caPL08 : caPL07 PL01 ;
caPL09 : caPL08 PL01 ;
caPL10 : caPL09 PL01 ;

/* ATTITUDE_TARGET (83) */
atM1 : ml37M1 ATCODEM1 PL37 ;

/* POSITION_TARGET_LOCAL_NED (85) */
ptlnM1 : ml51M1 PTLNCODEM1 ptlnPL51 ;
ptlnUS : ml51US PTLNCODEM2 ptlnPL51 ;
ptlnS : ml51S PTLNCODEM2 ptlnPL51 ;
ptlnPL51 : PL50 ptlnCoordFrame ;
ptlnCoordFrame : '\x01' |     r__1 ;

/* POSITION_TARGET_GLOBAL_INT (87) */
ptgiM1 : ml51M1 PTGICODEM1 ptgiPL51 ;
ptgiUS : ml51US PTGICODEM2 ptgiPL51 ;
ptgiS : ml51S PTGICODEM2 ptgiPL51 ;
ptgiPL51 : PL50 ptgiCoordFrame ;
ptgiCoordFrame : X00 | '\x03' | '\x0a' | '\x05' | '\x06' | '\x0b' ;

/* RADIO_STATUS (109) */
rsM1 : ml09M1 RSCODEM1 PL09 ;

/* DISTANCE SENSOR (132) */
dsM1 : ml14M1 DSCODEM1 dsPL14 ;
dsUS : ds11US | ds12US | ds13US | ds14US | ds15US | ds16US | ds17US | ds18US | ds19US | ds20US | ds21US | ds22US | ds23US | ds24US | ds25US | ds26US | ds27US | ds28US | ds29US | ds30US | ds31US | ds32US | ds33US | ds34US | ds35US | ds36US | ds37US | ds38US | ds39US ;
dsS : ds11S | ds12S | ds13S | ds14S | ds15S | ds16S | ds17S | ds18S | ds19S | ds20S | ds21S | ds22S | ds23S | ds24S | ds25S | ds26S | ds27S | ds28S | ds29S | ds30S | ds31S | ds32S | ds33S | ds34S | ds35S | ds36S | ds37S | ds38S | ds39S ;
ds11US : ml11US DSCODEM2 dsPL11 ;
ds12US : ml12US DSCODEM2 dsPL12 ;
ds13US : ml13US DSCODEM2 dsPL13 ;
ds14US : ml14US DSCODEM2 dsPL14 ;
ds15US : ml15US DSCODEM2 dsPL15 ;
ds16US : ml16US DSCODEM2 dsPL16 ;
ds17US : ml17US DSCODEM2 dsPL17 ;
ds18US : ml18US DSCODEM2 dsPL18 ;
ds19US : ml19US DSCODEM2 dsPL19 ;
ds20US : ml20US DSCODEM2 dsPL20 ;
ds21US : ml21US DSCODEM2 dsPL21 ;
ds22US : ml22US DSCODEM2 dsPL22 ;
ds23US : ml23US DSCODEM2 dsPL23 ;
ds24US : ml24US DSCODEM2 dsPL24 ;
ds25US : ml25US DSCODEM2 dsPL25 ;
ds26US : ml26US DSCODEM2 dsPL26 ;
ds27US : ml27US DSCODEM2 dsPL27 ;
ds28US : ml28US DSCODEM2 dsPL28 ;
ds29US : ml29US DSCODEM2 dsPL29 ;
ds30US : ml30US DSCODEM2 dsPL30 ;
ds31US : ml31US DSCODEM2 dsPL31 ;
ds32US : ml32US DSCODEM2 dsPL32 ;
ds33US : ml33US DSCODEM2 dsPL33 ;
ds34US : ml34US DSCODEM2 dsPL34 ;
ds35US : ml35US DSCODEM2 dsPL35 ;
ds36US : ml36US DSCODEM2 dsPL36 ;
ds37US : ml37US DSCODEM2 dsPL37 ;
ds38US : ml38US DSCODEM2 dsPL38 ;
ds39US : ml39US DSCODEM2 dsPL39 ;
ds11S : ml11S DSCODEM2 dsPL11 ;
ds12S : ml12S DSCODEM2 dsPL12 ;
ds13S : ml13S DSCODEM2 dsPL13 ;
ds14S : ml14S DSCODEM2 dsPL14 ;
ds15S : ml15S DSCODEM2 dsPL15 ;
ds16S : ml16S DSCODEM2 dsPL16 ;
ds17S : ml17S DSCODEM2 dsPL17 ;
ds18S : ml18S DSCODEM2 dsPL18 ;
ds19S : ml19S DSCODEM2 dsPL19 ;
ds20S : ml20S DSCODEM2 dsPL20 ;
ds21S : ml21S DSCODEM2 dsPL21 ;
ds22S : ml22S DSCODEM2 dsPL22 ;
ds23S : ml23S DSCODEM2 dsPL23 ;
ds24S : ml24S DSCODEM2 dsPL24 ;
ds25S : ml25S DSCODEM2 dsPL25 ;
ds26S : ml26S DSCODEM2 dsPL26 ;
ds27S : ml27S DSCODEM2 dsPL27 ;
ds28S : ml28S DSCODEM2 dsPL28 ;
ds29S : ml29S DSCODEM2 dsPL29 ;
ds30S : ml30S DSCODEM2 dsPL30 ;
ds31S : ml31S DSCODEM2 dsPL31 ;
ds32S : ml32S DSCODEM2 dsPL32 ;
ds33S : ml33S DSCODEM2 dsPL33 ;
ds34S : ml34S DSCODEM2 dsPL34 ;
ds35S : ml35S DSCODEM2 dsPL35 ;
ds36S : ml36S DSCODEM2 dsPL36 ;
ds37S : ml37S DSCODEM2 dsPL37 ;
ds38S : ml38S DSCODEM2 dsPL38 ;
ds39S : ml39S DSCODEM2 dsPL39 ;
dsPL11 : PL10 e3 ;
dsPL12 : dsPL11 PL01 ;
dsPL13 : dsPL12 dsOrientation ;
dsPL14 : dsPL13 PL01 ;
dsPL15 : dsPL14 PL01 ;
dsPL16 : dsPL15 PL01 ;
dsPL17 : dsPL16 PL01 ;
dsPL18 : dsPL17 PL01 ;
dsPL19 : dsPL18 PL01 ;
dsPL20 : dsPL19 PL01 ;
dsPL21 : dsPL20 PL01 ;
dsPL22 : dsPL21 PL01 ;
dsPL23 : dsPL22 PL01 ;
dsPL24 : dsPL23 PL01 ;
dsPL25 : dsPL24 PL01 ;
dsPL26 : dsPL25 PL01 ;
dsPL27 : dsPL26 PL01 ;
dsPL28 : dsPL27 PL01 ;
dsPL29 : dsPL28 PL01 ;
dsPL30 : dsPL29 PL01 ;
dsPL31 : dsPL30 PL01 ;
dsPL32 : dsPL31 PL01 ;
dsPL33 : dsPL32 PL01 ;
dsPL34 : dsPL33 PL01 ;
dsPL35 : dsPL34 PL01 ;
dsPL36 : dsPL35 PL01 ;
dsPL37 : dsPL36 PL01 ;
dsPL38 : dsPL37 PL01 ;
dsPL39 : dsPL38 PL01 ;
dsOrientation: e18 | '\x64' ; 

/* ALTITUDE (141) */
altM1 : ml32M1 ALTCODEM1 PL32 ;

/* BATTERY_STATUS (147) */
bsM1 : ml36M1 BSCODEM1 bsPL36 ;
bsUS : bs34US | bs35US | bs36US | bs37US | bs38US | bs39US | bs40US | bs41US | bs42US | bs43US | bs44US | bs45US | bs46US | bs47US | bs48US | bs49US | bs50US | bs51US | bs52US | bs53US | bs54US ;
bsS : bs34S | bs35S | bs36S | bs37S | bs38S | bs39S | bs40S | bs41S | bs42S | bs43S | bs44S | bs45S | bs46S | bs47S | bs48S | bs49S | bs50S | bs51S | bs52S | bs53S | bs54S ;
bs34US : ml34US BSCODEM2 bsPL34 ;
bs35US : ml35US BSCODEM2 bsPL35 ;
bs36US : ml36US BSCODEM2 bsPL36 ;
bs37US : ml37US BSCODEM2 bsPL37 ;
bs38US : ml38US BSCODEM2 bsPL38 ;
bs39US : ml39US BSCODEM2 bsPL39 ;
bs40US : ml40US BSCODEM2 bsPL40 ;
bs41US : ml41US BSCODEM2 bsPL41 ;
bs42US : ml42US BSCODEM2 bsPL42 ;
bs43US : ml43US BSCODEM2 bsPL43 ;
bs44US : ml44US BSCODEM2 bsPL44 ;
bs45US : ml45US BSCODEM2 bsPL45 ;
bs46US : ml46US BSCODEM2 bsPL46 ;
bs47US : ml47US BSCODEM2 bsPL47 ;
bs48US : ml48US BSCODEM2 bsPL48 ;
bs49US : ml49US BSCODEM2 bsPL49 ;
bs50US : ml50US BSCODEM2 bsPL50 ;
bs51US : ml51US BSCODEM2 bsPL51 ;
bs52US : ml52US BSCODEM2 bsPL52 ;
bs53US : ml53US BSCODEM2 bsPL53 ;
bs54US : ml54US BSCODEM2 bsPL54 ;
bs34S : ml34S BSCODEM2 bsPL34 ;
bs35S : ml35S BSCODEM2 bsPL35 ;
bs36S : ml36S BSCODEM2 bsPL36 ;
bs37S : ml37S BSCODEM2 bsPL37 ;
bs38S : ml38S BSCODEM2 bsPL38 ;
bs39S : ml39S BSCODEM2 bsPL39 ;
bs40S : ml40S BSCODEM2 bsPL40 ;
bs41S : ml41S BSCODEM2 bsPL41 ;
bs42S : ml42S BSCODEM2 bsPL42 ;
bs43S : ml43S BSCODEM2 bsPL43 ;
bs44S : ml44S BSCODEM2 bsPL44 ;
bs45S : ml45S BSCODEM2 bsPL45 ;
bs46S : ml46S BSCODEM2 bsPL46 ;
bs47S : ml47S BSCODEM2 bsPL47 ;
bs48S : ml48S BSCODEM2 bsPL48 ;
bs49S : ml49S BSCODEM2 bsPL49 ;
bs50S : ml50S BSCODEM2 bsPL50 ;
bs51S : ml51S BSCODEM2 bsPL51 ;
bs52S : ml52S BSCODEM2 bsPL52 ;
bs53S : ml53S BSCODEM2 bsPL53 ;
bs54S : ml54S BSCODEM2 bsPL54 ;
bsPL34 : PL33 e3 ;
bsPL35 : bsPL34 e3 ;
bsPL36 : bsPL35 PL01 ;
bsPL37 : bsPL36 PL01 ;
bsPL38 : bsPL37 PL01 ;
bsPL39 : bsPL38 PL01 ;
bsPL40 : bsPL39 PL01 ;
bsPL41 : bsPL40 e6 ;
bsPL42 : bsPL41 PL01 ;
bsPL43 : bsPL42 PL01 ;
bsPL44 : bsPL43 PL01 ;
bsPL45 : bsPL44 PL01 ;
bsPL46 : bsPL45 PL01 ;
bsPL47 : bsPL46 PL01 ;
bsPL48 : bsPL47 PL01 ;
bsPL49 : bsPL48 PL01 ;
bsPL50 : bsPL49 e5 ;
bsPL51 : bsPL50 PL01 ;
bsPL52 : bsPL51 PL01 ;
bsPL53 : bsPL52 PL01 ;
bsPL54 : bsPL53 PL01 ;

/* AUTOPILOT_VERSION (148) */
avM1 : ml60M1 AVCODEM1 PL60 ;

/* ESTIMATOR_STATUS (230) */
esM1 : ml42M1 ESCODEM1 PL42 ;

/* VIBRATION (241) */
vbM1 : ml32M1 VBCODEM1 PL32 ;

/* HOME_POSITION (242) */
hpM1 : ml52M1 HPCODEM1 PL52 ;

/* EXTENDED_SYS_STATE (245) */
essM1 : ml02M1 ESSCODEM1 essPL02 ;
essUS : ess01US | ess02US ;
essS : ess01S | ess02S ;
ess01US : ml01US ESSCODEM2 essPL01 ;
ess02US : ml02US ESSCODEM2 essPL02 ;
ess01S : ml01S ESSCODEM2 essPL01 ;
ess02S : ml02S ESSCODEM2 essPL02 ;
essPL01 : e3 ;
essPL02 : essPL01 e3 ;

/* STATUSTEXT (253) */
sM1 : ml51M1 SCODEM1 sPL51 ;
sUS : s01US | s02US | s03US | s04US | s05US | s06US | s07US | s08US | s09US | s10US | s11US | s12US | s13US | s14US | s15US | s16US | s17US | s18US | s19US | s20US | s21US | s22US | s23US | s24US | s25US | s26US | s27US | s28US | s29US | s30US | s31US | s32US | s33US | s34US | s35US | s36US | s37US | s38US | s39US | s40US | s41US | s42US | s43US | s44US | s45US | s46US | s47US | s48US | s49US | s50US | s51US | s52US | s53US | s54US ;
sS : s01S | s02S | s03S | s04S | s05S | s06S | s07S | s08S | s09S | s10S | s11S | s12S | s13S | s14S | s15S | s16S | s17S | s18S | s19S | s20S | s21S | s22S | s23S | s24S | s25S | s26S | s27S | s28S | s29S | s30S | s31S | s32S | s33S | s34S | s35S | s36S | s37S | s38S | s39S | s40S | s41S | s42S | s43S | s44S | s45S | s46S | s47S | s48S | s49S | s50S | s51S | s52S | s53S | s54S ;
s01US : ml01US SCODEM2 sPL01 ;
s02US : ml02US SCODEM2 sPL02 ;
s03US : ml03US SCODEM2 sPL03 ;
s04US : ml04US SCODEM2 sPL04 ;
s05US : ml05US SCODEM2 sPL05 ;
s06US : ml06US SCODEM2 sPL06 ;
s07US : ml07US SCODEM2 sPL07 ;
s08US : ml08US SCODEM2 sPL08 ;
s09US : ml09US SCODEM2 sPL09 ;
s10US : ml10US SCODEM2 sPL10 ;
s11US : ml11US SCODEM2 sPL11 ;
s12US : ml12US SCODEM2 sPL12 ;
s13US : ml13US SCODEM2 sPL13 ;
s14US : ml14US SCODEM2 sPL14 ;
s15US : ml15US SCODEM2 sPL15 ;
s16US : ml16US SCODEM2 sPL16 ;
s17US : ml17US SCODEM2 sPL17 ;
s18US : ml18US SCODEM2 sPL18 ;
s19US : ml19US SCODEM2 sPL19 ;
s20US : ml20US SCODEM2 sPL20 ;
s21US : ml21US SCODEM2 sPL21 ;
s22US : ml22US SCODEM2 sPL22 ;
s23US : ml23US SCODEM2 sPL23 ;
s24US : ml24US SCODEM2 sPL24 ;
s25US : ml25US SCODEM2 sPL25 ;
s26US : ml26US SCODEM2 sPL26 ;
s27US : ml27US SCODEM2 sPL27 ;
s28US : ml28US SCODEM2 sPL28 ;
s29US : ml29US SCODEM2 sPL29 ;
s30US : ml30US SCODEM2 sPL30 ;
s31US : ml31US SCODEM2 sPL31 ;
s32US : ml32US SCODEM2 sPL32 ;
s33US : ml33US SCODEM2 sPL33 ;
s34US : ml34US SCODEM2 sPL34 ;
s35US : ml35US SCODEM2 sPL35 ;
s36US : ml36US SCODEM2 sPL36 ;
s37US : ml37US SCODEM2 sPL37 ;
s38US : ml38US SCODEM2 sPL38 ;
s39US : ml39US SCODEM2 sPL39 ;
s40US : ml40US SCODEM2 sPL40 ;
s41US : ml41US SCODEM2 sPL41 ;
s42US : ml42US SCODEM2 sPL42 ;
s43US : ml43US SCODEM2 sPL43 ;
s44US : ml44US SCODEM2 sPL44 ;
s45US : ml45US SCODEM2 sPL45 ;
s46US : ml46US SCODEM2 sPL46 ;
s47US : ml47US SCODEM2 sPL47 ;
s48US : ml48US SCODEM2 sPL48 ;
s49US : ml49US SCODEM2 sPL49 ;
s50US : ml50US SCODEM2 sPL50 ;
s51US : ml51US SCODEM2 sPL51 ;
s52US : ml52US SCODEM2 sPL52 ;
s53US : ml53US SCODEM2 sPL53 ;
s54US : ml54US SCODEM2 sPL54 ;
s01S : ml01S SCODEM2 sPL01 ;
s02S : ml02S SCODEM2 sPL02 ;
s03S : ml03S SCODEM2 sPL03 ;
s04S : ml04S SCODEM2 sPL04 ;
s05S : ml05S SCODEM2 sPL05 ;
s06S : ml06S SCODEM2 sPL06 ;
s07S : ml07S SCODEM2 sPL07 ;
s08S : ml08S SCODEM2 sPL08 ;
s09S : ml09S SCODEM2 sPL09 ;
s10S : ml10S SCODEM2 sPL10 ;
s11S : ml11S SCODEM2 sPL11 ;
s12S : ml12S SCODEM2 sPL12 ;
s13S : ml13S SCODEM2 sPL13 ;
s14S : ml14S SCODEM2 sPL14 ;
s15S : ml15S SCODEM2 sPL15 ;
s16S : ml16S SCODEM2 sPL16 ;
s17S : ml17S SCODEM2 sPL17 ;
s18S : ml18S SCODEM2 sPL18 ;
s19S : ml19S SCODEM2 sPL19 ;
s20S : ml20S SCODEM2 sPL20 ;
s21S : ml21S SCODEM2 sPL21 ;
s22S : ml22S SCODEM2 sPL22 ;
s23S : ml23S SCODEM2 sPL23 ;
s24S : ml24S SCODEM2 sPL24 ;
s25S : ml25S SCODEM2 sPL25 ;
s26S : ml26S SCODEM2 sPL26 ;
s27S : ml27S SCODEM2 sPL27 ;
s28S : ml28S SCODEM2 sPL28 ;
s29S : ml29S SCODEM2 sPL29 ;
s30S : ml30S SCODEM2 sPL30 ;
s31S : ml31S SCODEM2 sPL31 ;
s32S : ml32S SCODEM2 sPL32 ;
s33S : ml33S SCODEM2 sPL33 ;
s34S : ml34S SCODEM2 sPL34 ;
s35S : ml35S SCODEM2 sPL35 ;
s36S : ml36S SCODEM2 sPL36 ;
s37S : ml37S SCODEM2 sPL37 ;
s38S : ml38S SCODEM2 sPL38 ;
s39S : ml39S SCODEM2 sPL39 ;
s40S : ml40S SCODEM2 sPL40 ;
s41S : ml41S SCODEM2 sPL41 ;
s42S : ml42S SCODEM2 sPL42 ;
s43S : ml43S SCODEM2 sPL43 ;
s44S : ml44S SCODEM2 sPL44 ;
s45S : ml45S SCODEM2 sPL45 ;
s46S : ml46S SCODEM2 sPL46 ;
s47S : ml47S SCODEM2 sPL47 ;
s48S : ml48S SCODEM2 sPL48 ;
s49S : ml49S SCODEM2 sPL49 ;
s50S : ml50S SCODEM2 sPL50 ;
s51S : ml51S SCODEM2 sPL51 ;
s52S : ml52S SCODEM2 sPL52 ;
s53S : ml53S SCODEM2 sPL53 ;
s54S : ml54S SCODEM2 sPL54 ;
sPL01 : e6 ;
sPL02 : sPL01 PL01 ;
sPL03 : sPL02 PL01 ;
sPL04 : sPL03 PL01 ;
sPL05 : sPL04 PL01 ;
sPL06 : sPL05 PL01 ;
sPL07 : sPL06 PL01 ;
sPL08 : sPL07 PL01 ;
sPL09 : sPL08 PL01 ;
sPL10 : sPL09 PL01 ;
sPL11 : sPL10 PL01 ;
sPL12 : sPL11 PL01 ;
sPL13 : sPL12 PL01 ;
sPL14 : sPL13 PL01 ;
sPL15 : sPL14 PL01 ;
sPL16 : sPL15 PL01 ;
sPL17 : sPL16 PL01 ;
sPL18 : sPL17 PL01 ;
sPL19 : sPL18 PL01 ;
sPL20 : sPL19 PL01 ;
sPL21 : sPL20 PL01 ;
sPL22 : sPL21 PL01 ;
sPL23 : sPL22 PL01 ;
sPL24 : sPL23 PL01 ;
sPL25 : sPL24 PL01 ;
sPL26 : sPL25 PL01 ;
sPL27 : sPL26 PL01 ;
sPL28 : sPL27 PL01 ;
sPL29 : sPL28 PL01 ;
sPL30 : sPL29 PL01 ;
sPL31 : sPL30 PL01 ;
sPL32 : sPL31 PL01 ;
sPL33 : sPL32 PL01 ;
sPL34 : sPL33 PL01 ;
sPL35 : sPL34 PL01 ;
sPL36 : sPL35 PL01 ;
sPL37 : sPL36 PL01 ;
sPL38 : sPL37 PL01 ;
sPL39 : sPL38 PL01 ;
sPL40 : sPL39 PL01 ;
sPL41 : sPL40 PL01 ;
sPL42 : sPL41 PL01 ;
sPL43 : sPL42 PL01 ;
sPL44 : sPL43 PL01 ;
sPL45 : sPL44 PL01 ;
sPL46 : sPL45 PL01 ;
sPL47 : sPL46 PL01 ;
sPL48 : sPL47 PL01 ;
sPL49 : sPL48 PL01 ;
sPL50 : sPL49 PL01 ;
sPL51 : sPL50 PL01 ;
sPL52 : sPL51 PL01 ;
sPL53 : sPL52 PL01 ;
sPL54 : sPL53 PL01 ;

/* PROTOCOL VERSION (300) */

/* UTM_GLOBAL_POSITION (340) */
ugpUS : ugp69US | ugp70US ;
ugpS : ugp69S | ugp70S ;
ugp69US : ml69US UGPCODE ugpPL69 ;
ugp70US : ml70US UGPCODE ugpPL70 ;
ugp69S : ml69S UGPCODE ugpPL69 ;
ugp70S : ml70S UGPCODE ugpPL70 ;
ugpPL69 : PL68 ugpFlightState ;
ugpPL70 : ugpPL69 PL01 ;
ugpFlightState : '\x01' | '\x02' | '\x03' | '\x10' | '\x20' ;

/* TIME_ESTIMATE_TO_TARGET (380) */

/* COMPONENT_METADATA (397) */

/* EVENT (410) */

/* CURRENT_EVENT_SEQUENCE (411) */

/* OPEN_DRONE_ID_LOCATION (12901) */
odilUS : odil53US | odil54US | odil55US | odil56US | odil57US | odil58US | odil59US ;
odilS : odil53S | odil54S | odil55S | odil56S | odil57S | odil58S | odil59S ;
odil53US : ml53US ODILCODE odilPL53 ;
odil54US : ml54US ODILCODE odilPL54 ;
odil55US : ml55US ODILCODE odilPL55 ;
odil56US : ml56US ODILCODE odilPL56 ;
odil57US : ml57US ODILCODE odilPL57 ;
odil58US : ml58US ODILCODE odilPL58 ;
odil59US : ml59US ODILCODE odilPL59 ;
odil53S : ml53S ODILCODE odilPL53 ;
odil54S : ml54S ODILCODE odilPL54 ;
odil55S : ml55S ODILCODE odilPL55 ;
odil56S : ml56S ODILCODE odilPL56 ;
odil57S : ml57S ODILCODE odilPL57 ;
odil58S : ml58S ODILCODE odilPL58 ;
odil59S : ml59S ODILCODE odilPL59 ;
odilPL53 : PL52 e3 ;
odilPL54 : odilPL53 e7 ;
odilPL55 : odilPL54 e12 ;
odilPL56 : odilPL55 e9 ;
odilPL57 : odilPL56 e9 ;
odilPL58 : odilPL57 e3 ;
odilPL59 : odilPL58 e13 ;

/* OPEN_DRONE_ID_SYSTEM (12904) */
odisUS : odis51US | odis52US | odis53US | odis54US ;
odisS : odis51S | odis52S | odis53S | odis54S ;
odis51US : ml51US ODISCODE odisPL51 ;
odis52US : ml52US ODISCODE odisPL52 ;
odis53US : ml53US ODISCODE odisPL53 ;
odis54US : ml54US ODISCODE odisPL54 ;
odis51S : ml51S ODISCODE odisPL51 ;
odis52S : ml52S ODISCODE odisPL52 ;
odis53S : ml53S ODISCODE odisPL53 ;
odis54S : ml54S ODISCODE odisPL54 ;
odisPL51 : PL50 e5 ;
odisPL52 : odisPL51 e7 ;
odisPL53 : odisPL52 PL01 ;
odisPL54 : odisPL53 PL01 ;

/* header started for MAVLink 1 messages */
ml02M1 : MAV1CODE '\x02' PL03 ;
ml03M1 : MAV1CODE '\x03' PL03 ;
ml04M1 : MAV1CODE '\x04' PL03 ;
ml06M1 : MAV1CODE '\x06' PL03 ;
ml09M1 : MAV1CODE '\x09' PL03 ;
ml11M1 : MAV1CODE '\x0b' PL03 ;
ml12M1 : MAV1CODE '\x0c' PL03 ;
ml14M1 : MAV1CODE '\x0e' PL03 ;
ml20M1 : MAV1CODE '\x14' PL03 ;
ml22M1 : MAV1CODE '\x16' PL03 ;
ml23M1 : MAV1CODE '\x17' PL03 ;
ml25M1 : MAV1CODE '\x19' PL03 ;
ml28M1 : MAV1CODE '\x1c' PL03 ;
ml30M1 : MAV1CODE '\x1e' PL03 ;
ml31M1 : MAV1CODE '\x1f' PL03 ;
ml32M1 : MAV1CODE '\x20' PL03 ;
ml33M1 : MAV1CODE '\x21' PL03 ;
ml36M1 : MAV1CODE '\x24' PL03 ;
ml37M1 : MAV1CODE '\x25' PL03 ;
ml42M1 : MAV1CODE '\x2a' PL03 ;
ml51M1 : MAV1CODE '\x33' PL03 ;
ml52M1 : MAV1CODE '\x34' PL03 ;
ml60M1 : MAV1CODE '\x3c' PL03 ;

/* header starter for messages of the same length, necessary to remove reduce-reduce conflict since the first token that differentiates in the message header is the message code - unsigned messages */
ml01US : MAV2CODE '\x01' messageHeaderTailUS ;
ml02US : MAV2CODE '\x02' messageHeaderTailUS ;
ml03US : MAV2CODE '\x03' messageHeaderTailUS ;
ml04US : MAV2CODE '\x04' messageHeaderTailUS ;
ml05US : MAV2CODE '\x05' messageHeaderTailUS ;
ml06US : MAV2CODE '\x06' messageHeaderTailUS ;
ml07US : MAV2CODE '\x07' messageHeaderTailUS ;
ml08US : MAV2CODE '\x08' messageHeaderTailUS ;
ml09US : MAV2CODE '\x09' messageHeaderTailUS ;
ml10US : MAV2CODE '\x0a' messageHeaderTailUS ;
ml11US : MAV2CODE '\x0b' messageHeaderTailUS ;
ml12US : MAV2CODE '\x0c' messageHeaderTailUS ;
ml13US : MAV2CODE '\x0d' messageHeaderTailUS ;
ml14US : MAV2CODE '\x0e' messageHeaderTailUS ;
ml15US : MAV2CODE '\x0f' messageHeaderTailUS ;
ml16US : MAV2CODE '\x10' messageHeaderTailUS ;
ml17US : MAV2CODE '\x11' messageHeaderTailUS ;
ml18US : MAV2CODE '\x12' messageHeaderTailUS ;
ml19US : MAV2CODE '\x13' messageHeaderTailUS ;
ml20US : MAV2CODE '\x14' messageHeaderTailUS ;
ml21US : MAV2CODE '\x15' messageHeaderTailUS ;
ml22US : MAV2CODE '\x16' messageHeaderTailUS ;
ml23US : MAV2CODE '\x17' messageHeaderTailUS ;
ml24US : MAV2CODE '\x18' messageHeaderTailUS ;
ml25US : MAV2CODE '\x19' messageHeaderTailUS ;
ml26US : MAV2CODE '\x1a' messageHeaderTailUS ;
ml27US : MAV2CODE '\x1b' messageHeaderTailUS ;
ml28US : MAV2CODE '\x1c' messageHeaderTailUS ;
ml29US : MAV2CODE '\x1d' messageHeaderTailUS ;
ml30US : MAV2CODE '\x1e' messageHeaderTailUS ;
ml31US : MAV2CODE '\x1f' messageHeaderTailUS ;
ml32US : MAV2CODE '\x20' messageHeaderTailUS ;
ml33US : MAV2CODE '\x21' messageHeaderTailUS ;
ml34US : MAV2CODE '\x22' messageHeaderTailUS ;
ml35US : MAV2CODE '\x23' messageHeaderTailUS ;
ml36US : MAV2CODE '\x24' messageHeaderTailUS ;
ml37US : MAV2CODE '\x25' messageHeaderTailUS ;
ml38US : MAV2CODE '\x26' messageHeaderTailUS ;
ml39US : MAV2CODE '\x27' messageHeaderTailUS ;
ml40US : MAV2CODE '\x28' messageHeaderTailUS ;
ml41US : MAV2CODE '\x29' messageHeaderTailUS ;
ml42US : MAV2CODE '\x2a' messageHeaderTailUS ;
ml43US : MAV2CODE '\x2b' messageHeaderTailUS ;
ml44US : MAV2CODE '\x2c' messageHeaderTailUS ;
ml45US : MAV2CODE '\x2d' messageHeaderTailUS ;
ml46US : MAV2CODE '\x2e' messageHeaderTailUS ;
ml47US : MAV2CODE '\x2f' messageHeaderTailUS ;
ml48US : MAV2CODE '\x30' messageHeaderTailUS ;
ml49US : MAV2CODE '\x31' messageHeaderTailUS ;
ml50US : MAV2CODE '\x32' messageHeaderTailUS ;
ml51US : MAV2CODE '\x33' messageHeaderTailUS ;
ml52US : MAV2CODE '\x34' messageHeaderTailUS ;
ml53US : MAV2CODE '\x35' messageHeaderTailUS ;
ml54US : MAV2CODE '\x36' messageHeaderTailUS ;
ml55US : MAV2CODE '\x37' messageHeaderTailUS ;
ml56US : MAV2CODE '\x38' messageHeaderTailUS ;
ml57US : MAV2CODE '\x39' messageHeaderTailUS ;
ml58US : MAV2CODE '\x3a' messageHeaderTailUS ;
ml59US : MAV2CODE '\x3b' messageHeaderTailUS ;
ml60US : MAV2CODE '\x3c' messageHeaderTailUS ;
ml61US : MAV2CODE '\x3d' messageHeaderTailUS ;
ml62US : MAV2CODE '\x3e' messageHeaderTailUS ;
ml63US : MAV2CODE '\x3f' messageHeaderTailUS ;
ml64US : MAV2CODE '\x40' messageHeaderTailUS ;
ml65US : MAV2CODE '\x41' messageHeaderTailUS ;
ml66US : MAV2CODE '\x42' messageHeaderTailUS ;
ml67US : MAV2CODE '\x43' messageHeaderTailUS ;
ml68US : MAV2CODE '\x44' messageHeaderTailUS ;
ml69US : MAV2CODE '\x45' messageHeaderTailUS ;
ml70US : MAV2CODE '\x46' messageHeaderTailUS ;
ml71US : MAV2CODE '\x47' messageHeaderTailUS ;
ml72US : MAV2CODE '\x48' messageHeaderTailUS ;
ml73US : MAV2CODE '\x49' messageHeaderTailUS ;
ml74US : MAV2CODE '\x4a' messageHeaderTailUS ;
ml75US : MAV2CODE '\x4b' messageHeaderTailUS ;
ml76US : MAV2CODE '\x4c' messageHeaderTailUS ;
ml77US : MAV2CODE '\x4d' messageHeaderTailUS ;
ml78US : MAV2CODE '\x4e' messageHeaderTailUS ;
ml79US : MAV2CODE '\x4f' messageHeaderTailUS ;
ml80US : MAV2CODE '\x50' messageHeaderTailUS ;
ml81US : MAV2CODE '\x51' messageHeaderTailUS ;
ml82US : MAV2CODE '\x52' messageHeaderTailUS ;
ml83US : MAV2CODE '\x53' messageHeaderTailUS ;
ml84US : MAV2CODE '\x54' messageHeaderTailUS ;
ml85US : MAV2CODE '\x55' messageHeaderTailUS ;
ml86US : MAV2CODE '\x56' messageHeaderTailUS ;
ml87US : MAV2CODE '\x57' messageHeaderTailUS ;
ml88US : MAV2CODE '\x58' messageHeaderTailUS ;
ml89US : MAV2CODE '\x59' messageHeaderTailUS ;
ml90US : MAV2CODE '\x5a' messageHeaderTailUS ;
ml91US : MAV2CODE '\x5b' messageHeaderTailUS ;
ml92US : MAV2CODE '\x5c' messageHeaderTailUS ;
ml93US : MAV2CODE '\x5d' messageHeaderTailUS ;
ml94US : MAV2CODE '\x5e' messageHeaderTailUS ;
ml95US : MAV2CODE '\x5f' messageHeaderTailUS ;
ml96US : MAV2CODE '\x60' messageHeaderTailUS ;
ml97US : MAV2CODE '\x61' messageHeaderTailUS ;
ml98US : MAV2CODE '\x62' messageHeaderTailUS ;
ml99US : MAV2CODE '\x63' messageHeaderTailUS ;
ml100US : MAV2CODE '\x64' messageHeaderTailUS ;
ml101US : MAV2CODE '\x65' messageHeaderTailUS ;
ml102US : MAV2CODE '\x66' messageHeaderTailUS ;
ml103US : MAV2CODE '\x67' messageHeaderTailUS ;
ml104US : MAV2CODE '\x68' messageHeaderTailUS ;
ml105US : MAV2CODE '\x69' messageHeaderTailUS ;
ml106US : MAV2CODE '\x6a' messageHeaderTailUS ;
ml107US : MAV2CODE '\x6b' messageHeaderTailUS ;
ml108US : MAV2CODE '\x6c' messageHeaderTailUS ;

/* header starter for messages of the same length, necessary to remove reduce-reduce conflict since the first token that differentiates in the message header is the message code  - signed messageUs*/
ml01S : MAV2CODE '\x01' messageHeaderTailS ;
ml02S : MAV2CODE '\x02' messageHeaderTailS ;
ml03S : MAV2CODE '\x03' messageHeaderTailS ;
ml04S : MAV2CODE '\x04' messageHeaderTailS ;
ml05S : MAV2CODE '\x05' messageHeaderTailS ;
ml06S : MAV2CODE '\x06' messageHeaderTailS ;
ml07S : MAV2CODE '\x07' messageHeaderTailS ;
ml08S : MAV2CODE '\x08' messageHeaderTailS ;
ml09S : MAV2CODE '\x09' messageHeaderTailS ;
ml10S : MAV2CODE '\x0a' messageHeaderTailS ;
ml11S : MAV2CODE '\x0b' messageHeaderTailS ;
ml12S : MAV2CODE '\x0c' messageHeaderTailS ;
ml13S : MAV2CODE '\x0d' messageHeaderTailS ;
ml14S : MAV2CODE '\x0e' messageHeaderTailS ;
ml15S : MAV2CODE '\x0f' messageHeaderTailS ;
ml16S : MAV2CODE '\x10' messageHeaderTailS ;
ml17S : MAV2CODE '\x11' messageHeaderTailS ;
ml18S : MAV2CODE '\x12' messageHeaderTailS ;
ml19S : MAV2CODE '\x13' messageHeaderTailS ;
ml20S : MAV2CODE '\x14' messageHeaderTailS ;
ml21S : MAV2CODE '\x15' messageHeaderTailS ;
ml22S : MAV2CODE '\x16' messageHeaderTailS ;
ml23S : MAV2CODE '\x17' messageHeaderTailS ;
ml24S : MAV2CODE '\x18' messageHeaderTailS ;
ml25S : MAV2CODE '\x19' messageHeaderTailS ;
ml26S : MAV2CODE '\x1a' messageHeaderTailS ;
ml27S : MAV2CODE '\x1b' messageHeaderTailS ;
ml28S : MAV2CODE '\x1c' messageHeaderTailS ;
ml29S : MAV2CODE '\x1d' messageHeaderTailS ;
ml30S : MAV2CODE '\x1e' messageHeaderTailS ;
ml31S : MAV2CODE '\x1f' messageHeaderTailS ;
ml32S : MAV2CODE '\x20' messageHeaderTailS ;
ml33S : MAV2CODE '\x21' messageHeaderTailS ;
ml34S : MAV2CODE '\x22' messageHeaderTailS ;
ml35S : MAV2CODE '\x23' messageHeaderTailS ;
ml36S : MAV2CODE '\x24' messageHeaderTailS ;
ml37S : MAV2CODE '\x25' messageHeaderTailS ;
ml38S : MAV2CODE '\x26' messageHeaderTailS ;
ml39S : MAV2CODE '\x27' messageHeaderTailS ;
ml40S : MAV2CODE '\x28' messageHeaderTailS ;
ml41S : MAV2CODE '\x29' messageHeaderTailS ;
ml42S : MAV2CODE '\x2a' messageHeaderTailS ;
ml43S : MAV2CODE '\x2b' messageHeaderTailS ;
ml44S : MAV2CODE '\x2c' messageHeaderTailS ;
ml45S : MAV2CODE '\x2d' messageHeaderTailS ;
ml46S : MAV2CODE '\x2e' messageHeaderTailS ;
ml47S : MAV2CODE '\x2f' messageHeaderTailS ;
ml48S : MAV2CODE '\x30' messageHeaderTailS ;
ml49S : MAV2CODE '\x31' messageHeaderTailS ;
ml50S : MAV2CODE '\x32' messageHeaderTailS ;
ml51S : MAV2CODE '\x33' messageHeaderTailS ;
ml52S : MAV2CODE '\x34' messageHeaderTailS ;
ml53S : MAV2CODE '\x35' messageHeaderTailS ;
ml54S : MAV2CODE '\x36' messageHeaderTailS ;
ml55S : MAV2CODE '\x37' messageHeaderTailS ;
ml56S : MAV2CODE '\x38' messageHeaderTailS ;
ml57S : MAV2CODE '\x39' messageHeaderTailS ;
ml58S : MAV2CODE '\x3a' messageHeaderTailS ;
ml59S : MAV2CODE '\x3b' messageHeaderTailS ;
ml60S : MAV2CODE '\x3c' messageHeaderTailS ;
ml61S : MAV2CODE '\x3d' messageHeaderTailS ;
ml62S : MAV2CODE '\x3e' messageHeaderTailS ;
ml63S : MAV2CODE '\x3f' messageHeaderTailS ;
ml64S : MAV2CODE '\x40' messageHeaderTailS ;
ml65S : MAV2CODE '\x41' messageHeaderTailS ;
ml66S : MAV2CODE '\x42' messageHeaderTailS ;
ml67S : MAV2CODE '\x43' messageHeaderTailS ;
ml68S : MAV2CODE '\x44' messageHeaderTailS ;
ml69S : MAV2CODE '\x45' messageHeaderTailS ;
ml70S : MAV2CODE '\x46' messageHeaderTailS ;
ml71S : MAV2CODE '\x47' messageHeaderTailS ;
ml72S : MAV2CODE '\x48' messageHeaderTailS ;
ml73S : MAV2CODE '\x49' messageHeaderTailS ;
ml74S : MAV2CODE '\x4a' messageHeaderTailS ;
ml75S : MAV2CODE '\x4b' messageHeaderTailS ;
ml76S : MAV2CODE '\x4c' messageHeaderTailS ;
ml77S : MAV2CODE '\x4d' messageHeaderTailS ;
ml78S : MAV2CODE '\x4e' messageHeaderTailS ;
ml79S : MAV2CODE '\x4f' messageHeaderTailS ;
ml80S : MAV2CODE '\x50' messageHeaderTailS ;
ml81S : MAV2CODE '\x51' messageHeaderTailS ;
ml82S : MAV2CODE '\x52' messageHeaderTailS ;
ml83S : MAV2CODE '\x53' messageHeaderTailS ;
ml84S : MAV2CODE '\x54' messageHeaderTailS ;
ml85S : MAV2CODE '\x55' messageHeaderTailS ;
ml86S : MAV2CODE '\x56' messageHeaderTailS ;
ml87S : MAV2CODE '\x57' messageHeaderTailS ;
ml88S : MAV2CODE '\x58' messageHeaderTailS ;
ml89S : MAV2CODE '\x59' messageHeaderTailS ;
ml90S : MAV2CODE '\x5a' messageHeaderTailS ;
ml91S : MAV2CODE '\x5b' messageHeaderTailS ;
ml92S : MAV2CODE '\x5c' messageHeaderTailS ;
ml93S : MAV2CODE '\x5d' messageHeaderTailS ;
ml94S : MAV2CODE '\x5e' messageHeaderTailS ;
ml95S : MAV2CODE '\x5f' messageHeaderTailS ;
ml96S : MAV2CODE '\x60' messageHeaderTailS ;
ml97S : MAV2CODE '\x61' messageHeaderTailS ;
ml98S : MAV2CODE '\x62' messageHeaderTailS ;
ml99S : MAV2CODE '\x63' messageHeaderTailS ;
ml100S : MAV2CODE '\x64' messageHeaderTailS ;
ml101S : MAV2CODE '\x65' messageHeaderTailS ;
ml102S : MAV2CODE '\x66' messageHeaderTailS ;
ml103S : MAV2CODE '\x67' messageHeaderTailS ;
ml104S : MAV2CODE '\x68' messageHeaderTailS ;
ml105S : MAV2CODE '\x69' messageHeaderTailS ;
ml106S : MAV2CODE '\x6a' messageHeaderTailS ;
ml107S : MAV2CODE '\x6b' messageHeaderTailS ;
ml108S : MAV2CODE '\x6c' messageHeaderTailS ;

/* header tail for unsigned and signed MAVLink messages, respectively */
messageHeaderTailUS : X00 X00 PL03 ;
messageHeaderTailS : '\x01' X00 PL03 ;

/* Tokens */
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
PL44 : PL43 r__0 ;
PL45 : PL44 r__0 ;
PL46 : PL45 r__0 ;
PL47 : PL46 r__0 ;
PL48 : PL47 r__0 ;
PL49 : PL48 r__0 ;
PL50 : PL49 r__0 ;
PL51 : PL50 r__0 ;
PL52 : PL51 r__0 ;
PL53 : PL52 r__0 ;
PL54 : PL53 r__0 ;
PL55 : PL54 r__0 ;
PL56 : PL55 r__0 ;
PL57 : PL56 r__0 ;
PL58 : PL57 r__0 ;
PL59 : PL58 r__0 ;
PL60 : PL59 r__0 ;
PL61 : PL60 r__0 ;
PL62 : PL61 r__0 ;
PL63 : PL62 r__0 ;
PL64 : PL63 r__0 ;
PL65 : PL64 r__0 ;
PL66 : PL65 r__0 ;
PL67 : PL66 r__0 ;
PL68 : PL67 r__0 ;
PL69 : PL68 r__0 ;
PL70 : PL69 r__0 ;
PL71 : PL70 r__0 ;
PL72 : PL71 r__0 ;
PL73 : PL72 r__0 ;
PL74 : PL73 r__0 ;
PL75 : PL74 r__0 ;
PL76 : PL75 r__0 ;
PL77 : PL76 r__0 ;
PL78 : PL77 r__0 ;
PL79 : PL78 r__0 ;
PL80 : PL79 r__0 ;
PL81 : PL80 r__0 ;
PL82 : PL81 r__0 ;
PL83 : PL82 r__0 ;
PL84 : PL83 r__0 ;
PL85 : PL84 r__0 ;
PL86 : PL85 r__0 ;
PL87 : PL86 r__0 ;
PL88 : PL87 r__0 ;
PL89 : PL88 r__0 ;
PL90 : PL89 r__0 ;
PL91 : PL90 r__0 ;
PL92 : PL91 r__0 ;
PL93 : PL92 r__0 ;
PL94 : PL93 r__0 ;
PL95 : PL94 r__0 ;
PL96 : PL95 r__0 ;
PL97 : PL96 r__0 ;
PL98 : PL97 r__0 ;
PL99 : PL98 r__0 ;
PL100 : PL99 r__0 ;
PL101 : PL100 r__0 ;
PL102 : PL101 r__0 ;
PL103 : PL102 r__0 ;
PL104 : PL103 r__0 ;
PL105 : PL104 r__0 ;
PL106 : PL105 r__0 ;
PL107 : PL106 r__0 ;
PL108 : PL107 r__0 ;

MAV1CODE : '\xfe' ;
MAV2CODE : '\xfd' ;

TWOZEROBYTES : X00 X00 ;

HBCODEM1 : X00 ;
HBCODEM2 : X00 TWOZEROBYTES ;
SSCODEM1 : '\x01' ;
SSCODEM2 : '\x01' TWOZEROBYTES ;
STCODEM1 : '\x02' ;
STCODEM2 : '\x02' TWOZEROBYTES ;
PINGCODEM1 : '\x04' ;
PINGCODEM2 : '\x04' TWOZEROBYTES ;
LNSCODEM1 : '\x08' ;
LNSCODEM2 : '\x08' TWOZEROBYTES ;
SMCODEM1 : '\x0b' ;
SMCODEM2 : '\x0b' TWOZEROBYTES ;
PRRCODEM1 : '\x14' ;
PRRCODEM2 : '\x14' TWOZEROBYTES ;
PRLCODEM1 : '\x15' ;
PRLCODEM2 : '\x15' TWOZEROBYTES ;
PVCODEM1 : '\x16' ;
PVCODEM2 : '\x16' TWOZEROBYTES ;
PSCODEM1 : '\x17' ;
PSCODEM2 : '\x17' TWOZEROBYTES ;
GRICODEM1 : '\x18' ;
GRICODEM2 : '\x18' TWOZEROBYTES ;
SPCODEM1 : '\x1d' ;
SPCODEM2 : '\x1d' TWOZEROBYTES ;
ATTCODEM1 : '\x1e' ;
ATTCODEM2 : '\x1e' TWOZEROBYTES ;
AQCODEM1 : '\x1f' ;
AQCODEM2 : '\x1f' TWOZEROBYTES ;
LPNCODEM1 : '\x20' ;
LPNCODEM2 : '\x20' TWOZEROBYTES ;
GPICODEM1 : '\x21' ;
GPICODEM2 : '\x21' TWOZEROBYTES ;
MRCODEM1 : '\x28' ;
MRCODEM2 : '\x28' TWOZEROBYTES ;
MCCODEM1 : '\x2a' ;
MCCODEM2 : '\x2a' TWOZEROBYTES ;
MRLCODEM1 : '\x2b' ;
MRLCODEM2 : '\x2b' TWOZEROBYTES ;
MCTCODEM1 : '\x2c' ;
MCTCODEM2 : '\x2c' TWOZEROBYTES ;
MCACODEM1 : '\x2d' ;
MCACODEM2 : '\x2d' TWOZEROBYTES ;
MIRCODEM1 : '\x2e' ;
MIRCODEM2 : '\x2e' TWOZEROBYTES ;
MACODEM1 : '\x2f' ;
MACODEM2 : '\x2f' TWOZEROBYTES ;
GGOCODEM1 : '\x31' ;
GGOCODEM2 : '\x31' TWOZEROBYTES ;
MRICODEM1 : '\x33' ;
MRICODEM2 : '\x33' TWOZEROBYTES ;
RCCODEM1 : '\x41' ;
RCCODEM2 : '\x41' TWOZEROBYTES ;
MCLCODEM1 : '\x45' ;
MCLCODEM2 : '\x45' TWOZEROBYTES ;
MIICODEM1 : '\x49' ;
MIICODEM2 : '\x49' TWOZEROBYTES ;
VHCODEM1 : '\x4a' ;
VHCODEM2 : '\x4a' TWOZEROBYTES ;
CLCODEM1 : '\x4c' ;
CLCODEM2 : '\x4c' TWOZEROBYTES ;
CACODEM1 : '\x4d' ;
CACODEM2 : '\x4d' TWOZEROBYTES ;
ATCODEM1 : '\x53' ;
ATCODEM2 : '\x53' TWOZEROBYTES ;
PTLNCODEM1 : '\x55' ;
PTLNCODEM2 : '\x55' TWOZEROBYTES ;
PTGICODEM1 : '\x57' ;
PTGICODEM2 : '\x57' TWOZEROBYTES ;
RSCODEM1 : '\x6d' ;
RSCODEM2 : '\x6d' TWOZEROBYTES ;
DSCODEM1 : '\x84' ;
DSCODEM2 : '\x84' TWOZEROBYTES ;
ALTCODEM1 : '\x8d' ;
ALTCODEM2 : '\x8d' TWOZEROBYTES ;
BSCODEM1 : '\x93' ;
BSCODEM2 : '\x93' TWOZEROBYTES ;
AVCODEM1 : '\x94' ;
AVCODEM2 : '\x94' TWOZEROBYTES ;
ESCODEM1 : '\xe6' ;
ESCODEM2 : '\xe6' TWOZEROBYTES ;
VBCODEM1 : '\xf1' ;
VBCODEM2 : '\xf1' TWOZEROBYTES ;
HPCODEM1 : '\xf2' ;
HPCODEM2 : '\xf2' TWOZEROBYTES ;
ESSCODEM1 : '\xf5' ;
ESSCODEM2 : '\xf5' TWOZEROBYTES ;
SCODEM1 : '\xfd' ;
SCODEM2 : '\xfd' TWOZEROBYTES ;
PVNCODE : '\x2c' '\x01' X00 ;
UGPCODE : '\x54' '\x01' X00 ;
TETTCODE : '\x7c' '\x01' X00 ;
CMCODE : '\x8d' '\x01' X00 ;
EVCODE : '\x9a' '\x01' X00 ;
CESCODE : '\x9b' '\x01' X00 ;
ODILCODE : '\x65' '\x32' X00 ;
ODISCODE : '\x68' '\x32' X00 ;

/* common enumerated values */
e1 : e5 | '\xff' ;																		/* 0x00 to 0x02 or 0xFF */
e2 :     r__2 ;														/* 0x01 to 0x0A */
e3 : e10 | '\x04' ;		 	 															/* 0x00 to 0x04 */
e4 : e6 | '\x08' ;																		/* 0x00 to 0x08 */
e5 : e7 | '\x02' ;																		/* 0x00 to 0x02 */
e6 : e9 | '\x07' ;																		/* 0x00 to 0x07 */
e7 : X00 | '\x01' ;																/* 0x00 to 0x01 */
e8 : e3 | '\x05' ;	 																	/* 0x00 to 0x05 */
e9 : e8 | '\x06' ;																		/* 0x00 to 0x06 */
e10 : e5 | '\x03' ;																		/* 0x00 to 0x03 */
e11 : e4 | '\x09' ;																		/* 0x00 to 0x09 */
e12 : e11 | '\x0a' | '\x0b' | '\x0c' ;								/* 0x00 to 0x0C */
e13 : e12 | '\x0d' | '\x0e' ;				 									/* 0x00 to 0x0E */
e14 : e13 | '\x0f' ; 																	/* 0x00 to 0x0F */
e15 : e14 |     r__3 ;											/* 0x00 to 0x14 */
e16 : e15 | '\x15' ; 	 																/* 0x00 to 0x15 */
e17 : e18 | '\x29' | '\x2a' | '\x2b' | '\x2c' ;				/* 0x00 to 0x2C */
e18 : e16 |     r__4 ;		 	 								/* 0x00 to 0x28 */

crc : PL02 ;				 	 																/* double wildcard - message crc */
sig : PL13 ;																					/* thirteen wildcards - message signature */
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
  '\x07' | '\x08' | '\x09' ;
r__2 : 
  '\x01' | '\x02' | '\x03' | '\x04' | '\x05' | '\x06' | '\x07' | '\x08' | 
  '\x09' | '\x0a' ;
r__3 : 
  '\x10' | '\x11' | '\x12' | '\x13' | '\x14' ;
r__4 : 
  '\x16' | '\x17' | '\x18' | '\x19' | '\x1a' | '\x1b' | '\x1c' | '\x1d' | 
  '\x1e' | '\x1f' | '\x20' | '\x21' | '\x22' | '\x23' | '\x24' | '\x25' | 
  '\x26' | '\x27' | '\x28' ;
