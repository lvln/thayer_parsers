%{
  #define YYDEBUG 1
  int yylex(void);
  void yyerror(char *s);
%}
%token X00
%%
P : optMessage ;
optMessage : /* empty */ | optMessage message ;

/* a message may be either a signed or an unsigned message */
message : messageM1 | messageUS | messageS ;

/* MAVLink 1 messages have a slightly different header fields and cannot contain a signature */
messageM1 : messageOptM1 crc ;
messageOptM1 : spM1 | attM1 | gpiM1 ;

/* unsigned messsages contain a header with a payload and finally a checksum */
messageUS : messageOptUS crc ;
messageOptUS : spUS | attUS | gpiUS ;

/* signed messages contain a header with a payload, followed by a checksum and finally a 13 byte signature */
messageS : messageOptS crc sig ;
messageOptS : spS | attS | gpiS ;

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

/* ATTITUDE (30) */
attM1 : ml28M1 ATTCODEM1 attPL28 ;
attUS : att01US | att02US | att03US | att04US | att05US | att06US | att07US | att08US | att09US | att10US | att11US | att12US | att13US | att14US | att15US | att16US | att17US | att18US | att19US | att20US | att21US | att22US | att23US | att24US | att25US | att26US | att27US | att28US ;
attS : att01S | att02S | att03S | att04S | att05S | att06S | att07S | att08S | att09S | att10S | att11S | att12S | att13S | att14S | att15S | att16S | att17S | att18S | att19S | att20S | att21S | att22S | att23S | att24S | att25S | att26S | att27S | att28S ;
att01US : attMH01US attPL01 ;
att02US : attMH02US attPL02 ;
att03US : attMH03US attPL03 ;
att04US : attMH04US attPL04 ;
att05US : attMH05US attPL05 ;
att06US : attMH06US attPL06 ;
att07US : attMH07US attPL07 ;
att08US : attMH08US attPL08 ;
att09US : attMH09US attPL09 ;
att10US : attMH10US attPL10 ;
att11US : attMH11US attPL11 ;
att12US : attMH12US attPL12 ;
att13US : attMH13US attPL13 ;
att14US : attMH14US attPL14 ;
att15US : attMH15US attPL15 ;
att16US : attMH16US attPL16 ;
att17US : attMH17US attPL17 ;
att18US : attMH18US attPL18 ;
att19US : attMH19US attPL19 ;
att20US : attMH20US attPL20 ;
att21US : attMH21US attPL21 ;
att22US : attMH22US attPL22 ;
att23US : attMH23US attPL23 ;
att24US : attMH24US attPL24 ;
att25US : attMH25US attPL25 ;
att26US : attMH26US attPL26 ;
att27US : attMH27US attPL27 ;
att28US : attMH28US attPL28 ;
att01S : attMH01S attPL01 ;
att02S : attMH02S attPL02 ;
att03S : attMH03S attPL03 ;
att04S : attMH04S attPL04 ;
att05S : attMH05S attPL05 ;
att06S : attMH06S attPL06 ;
att07S : attMH07S attPL07 ;
att08S : attMH08S attPL08 ;
att09S : attMH09S attPL09 ;
att10S : attMH10S attPL10 ;
att11S : attMH11S attPL11 ;
att12S : attMH12S attPL12 ;
att13S : attMH13S attPL13 ;
att14S : attMH14S attPL14 ;
att15S : attMH15S attPL15 ;
att16S : attMH16S attPL16 ;
att17S : attMH17S attPL17 ;
att18S : attMH18S attPL18 ;
att19S : attMH19S attPL19 ;
att20S : attMH20S attPL20 ;
att21S : attMH21S attPL21 ;
att22S : attMH22S attPL22 ;
att23S : attMH23S attPL23 ;
att24S : attMH24S attPL24 ;
att25S : attMH25S attPL25 ;
att26S : attMH26S attPL26 ;
att27S : attMH27S attPL27 ;
att28S : attMH28S attPL28 ;
attMH01US : ml01US ATTCODEM2 ;
attMH02US : ml02US ATTCODEM2 ;
attMH03US : ml03US ATTCODEM2 ;
attMH04US : ml04US ATTCODEM2 ;
attMH05US : ml05US ATTCODEM2 ;
attMH06US : ml06US ATTCODEM2 ;
attMH07US : ml07US ATTCODEM2 ;
attMH08US : ml08US ATTCODEM2 ;
attMH09US : ml09US ATTCODEM2 ;
attMH10US : ml10US ATTCODEM2 ;
attMH11US : ml11US ATTCODEM2 ;
attMH12US : ml12US ATTCODEM2 ;
attMH13US : ml13US ATTCODEM2 ;
attMH14US : ml14US ATTCODEM2 ;
attMH15US : ml15US ATTCODEM2 ;
attMH16US : ml16US ATTCODEM2 ;
attMH17US : ml17US ATTCODEM2 ;
attMH18US : ml18US ATTCODEM2 ;
attMH19US : ml19US ATTCODEM2 ;
attMH20US : ml20US ATTCODEM2 ;
attMH21US : ml21US ATTCODEM2 ;
attMH22US : ml22US ATTCODEM2 ;
attMH23US : ml23US ATTCODEM2 ;
attMH24US : ml24US ATTCODEM2 ;
attMH25US : ml25US ATTCODEM2 ;
attMH26US : ml26US ATTCODEM2 ;
attMH27US : ml27US ATTCODEM2 ;
attMH28US : ml28US ATTCODEM2 ;
attMH01S : ml01S ATTCODEM2 ;
attMH02S : ml02S ATTCODEM2 ;
attMH03S : ml03S ATTCODEM2 ;
attMH04S : ml04S ATTCODEM2 ;
attMH05S : ml05S ATTCODEM2 ;
attMH06S : ml06S ATTCODEM2 ;
attMH07S : ml07S ATTCODEM2 ;
attMH08S : ml08S ATTCODEM2 ;
attMH09S : ml09S ATTCODEM2 ;
attMH10S : ml10S ATTCODEM2 ;
attMH11S : ml11S ATTCODEM2 ;
attMH12S : ml12S ATTCODEM2 ;
attMH13S : ml13S ATTCODEM2 ;
attMH14S : ml14S ATTCODEM2 ;
attMH15S : ml15S ATTCODEM2 ;
attMH16S : ml16S ATTCODEM2 ;
attMH17S : ml17S ATTCODEM2 ;
attMH18S : ml18S ATTCODEM2 ;
attMH19S : ml19S ATTCODEM2 ;
attMH20S : ml20S ATTCODEM2 ;
attMH21S : ml21S ATTCODEM2 ;
attMH22S : ml22S ATTCODEM2 ;
attMH23S : ml23S ATTCODEM2 ;
attMH24S : ml24S ATTCODEM2 ;
attMH25S : ml25S ATTCODEM2 ;
attMH26S : ml26S ATTCODEM2 ;
attMH27S : ml27S ATTCODEM2 ;
attMH28S : ml28S ATTCODEM2 ;
attPL01 : r__0 ;
attPL02 : r__0 r__0 ;
attPL03 : r__0 r__0 r__0 ;
attPL04 : r__0 r__0 r__0 r__0 ;
attPL05 : r__0 r__0 r__0 r__0 r__0 ;
attPL06 : r__0 r__0 r__0 r__0 r__0 r__0 ;
attPL07 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
attPL08 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
attPL09 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
attPL10 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
attPL11 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
attPL12 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
attPL13 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
attPL14 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
attPL15 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
attPL16 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
attPL17 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
attPL18 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
attPL19 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
attPL20 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
attPL21 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
attPL22 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
attPL23 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
attPL24 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
attPL25 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
attPL26 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
attPL27 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
attPL28 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;

/* GLOBAL_POSITION_INT (33) */
gpiM1 : ml28M1 GPICODEM1 gpiPL28 ;
gpiUS : gpi01US | gpi02US | gpi03US | gpi04US | gpi05US | gpi06US | gpi07US | gpi08US | gpi09US | gpi10US | gpi11US | gpi12US | gpi13US | gpi14US | gpi15US | gpi16US | gpi17US | gpi18US | gpi19US | gpi20US | gpi21US | gpi22US | gpi23US | gpi24US | gpi25US | gpi26US | gpi27US | gpi28US ;
gpiS : gpi01S | gpi02S | gpi03S | gpi04S | gpi05S | gpi06S | gpi07S | gpi08S | gpi09S | gpi10S | gpi11S | gpi12S | gpi13S | gpi14S | gpi15S | gpi16S | gpi17S | gpi18S | gpi19S | gpi20S | gpi21S | gpi22S | gpi23S | gpi24S | gpi25S | gpi26S | gpi27S | gpi28S ;
gpi01US : gpiMH01US gpiPL01 ;
gpi02US : gpiMH02US gpiPL02 ;
gpi03US : gpiMH03US gpiPL03 ;
gpi04US : gpiMH04US gpiPL04 ;
gpi05US : gpiMH05US gpiPL05 ;
gpi06US : gpiMH06US gpiPL06 ;
gpi07US : gpiMH07US gpiPL07 ;
gpi08US : gpiMH08US gpiPL08 ;
gpi09US : gpiMH09US gpiPL09 ;
gpi10US : gpiMH10US gpiPL10 ;
gpi11US : gpiMH11US gpiPL11 ;
gpi12US : gpiMH12US gpiPL12 ;
gpi13US : gpiMH13US gpiPL13 ;
gpi14US : gpiMH14US gpiPL14 ;
gpi15US : gpiMH15US gpiPL15 ;
gpi16US : gpiMH16US gpiPL16 ;
gpi17US : gpiMH17US gpiPL17 ;
gpi18US : gpiMH18US gpiPL18 ;
gpi19US : gpiMH19US gpiPL19 ;
gpi20US : gpiMH20US gpiPL20 ;
gpi21US : gpiMH21US gpiPL21 ;
gpi22US : gpiMH22US gpiPL22 ;
gpi23US : gpiMH23US gpiPL23 ;
gpi24US : gpiMH24US gpiPL24 ;
gpi25US : gpiMH25US gpiPL25 ;
gpi26US : gpiMH26US gpiPL26 ;
gpi27US : gpiMH27US gpiPL27 ;
gpi28US : gpiMH28US gpiPL28 ;
gpi01S : gpiMH01S gpiPL01 ;
gpi02S : gpiMH02S gpiPL02 ;
gpi03S : gpiMH03S gpiPL03 ;
gpi04S : gpiMH04S gpiPL04 ;
gpi05S : gpiMH05S gpiPL05 ;
gpi06S : gpiMH06S gpiPL06 ;
gpi07S : gpiMH07S gpiPL07 ;
gpi08S : gpiMH08S gpiPL08 ;
gpi09S : gpiMH09S gpiPL09 ;
gpi10S : gpiMH10S gpiPL10 ;
gpi11S : gpiMH11S gpiPL11 ;
gpi12S : gpiMH12S gpiPL12 ;
gpi13S : gpiMH13S gpiPL13 ;
gpi14S : gpiMH14S gpiPL14 ;
gpi15S : gpiMH15S gpiPL15 ;
gpi16S : gpiMH16S gpiPL16 ;
gpi17S : gpiMH17S gpiPL17 ;
gpi18S : gpiMH18S gpiPL18 ;
gpi19S : gpiMH19S gpiPL19 ;
gpi20S : gpiMH20S gpiPL20 ;
gpi21S : gpiMH21S gpiPL21 ;
gpi22S : gpiMH22S gpiPL22 ;
gpi23S : gpiMH23S gpiPL23 ;
gpi24S : gpiMH24S gpiPL24 ;
gpi25S : gpiMH25S gpiPL25 ;
gpi26S : gpiMH26S gpiPL26 ;
gpi27S : gpiMH27S gpiPL27 ;
gpi28S : gpiMH28S gpiPL28 ;
gpiMH01US : ml01US GPICODEM2 ;
gpiMH02US : ml02US GPICODEM2 ;
gpiMH03US : ml03US GPICODEM2 ;
gpiMH04US : ml04US GPICODEM2 ;
gpiMH05US : ml05US GPICODEM2 ;
gpiMH06US : ml06US GPICODEM2 ;
gpiMH07US : ml07US GPICODEM2 ;
gpiMH08US : ml08US GPICODEM2 ;
gpiMH09US : ml09US GPICODEM2 ;
gpiMH10US : ml10US GPICODEM2 ;
gpiMH11US : ml11US GPICODEM2 ;
gpiMH12US : ml12US GPICODEM2 ;
gpiMH13US : ml13US GPICODEM2 ;
gpiMH14US : ml14US GPICODEM2 ;
gpiMH15US : ml15US GPICODEM2 ;
gpiMH16US : ml16US GPICODEM2 ;
gpiMH17US : ml17US GPICODEM2 ;
gpiMH18US : ml18US GPICODEM2 ;
gpiMH19US : ml19US GPICODEM2 ;
gpiMH20US : ml20US GPICODEM2 ;
gpiMH21US : ml21US GPICODEM2 ;
gpiMH22US : ml22US GPICODEM2 ;
gpiMH23US : ml23US GPICODEM2 ;
gpiMH24US : ml24US GPICODEM2 ;
gpiMH25US : ml25US GPICODEM2 ;
gpiMH26US : ml26US GPICODEM2 ;
gpiMH27US : ml27US GPICODEM2 ;
gpiMH28US : ml28US GPICODEM2 ;
gpiMH01S : ml01S GPICODEM2 ;
gpiMH02S : ml02S GPICODEM2 ;
gpiMH03S : ml03S GPICODEM2 ;
gpiMH04S : ml04S GPICODEM2 ;
gpiMH05S : ml05S GPICODEM2 ;
gpiMH06S : ml06S GPICODEM2 ;
gpiMH07S : ml07S GPICODEM2 ;
gpiMH08S : ml08S GPICODEM2 ;
gpiMH09S : ml09S GPICODEM2 ;
gpiMH10S : ml10S GPICODEM2 ;
gpiMH11S : ml11S GPICODEM2 ;
gpiMH12S : ml12S GPICODEM2 ;
gpiMH13S : ml13S GPICODEM2 ;
gpiMH14S : ml14S GPICODEM2 ;
gpiMH15S : ml15S GPICODEM2 ;
gpiMH16S : ml16S GPICODEM2 ;
gpiMH17S : ml17S GPICODEM2 ;
gpiMH18S : ml18S GPICODEM2 ;
gpiMH19S : ml19S GPICODEM2 ;
gpiMH20S : ml20S GPICODEM2 ;
gpiMH21S : ml21S GPICODEM2 ;
gpiMH22S : ml22S GPICODEM2 ;
gpiMH23S : ml23S GPICODEM2 ;
gpiMH24S : ml24S GPICODEM2 ;
gpiMH25S : ml25S GPICODEM2 ;
gpiMH26S : ml26S GPICODEM2 ;
gpiMH27S : ml27S GPICODEM2 ;
gpiMH28S : ml28S GPICODEM2 ;
gpiPL01 : r__0 ;
gpiPL02 : r__0 r__0 ;
gpiPL03 : r__0 r__0 r__0 ;
gpiPL04 : r__0 r__0 r__0 r__0 ;
gpiPL05 : r__0 r__0 r__0 r__0 r__0 ;
gpiPL06 : r__0 r__0 r__0 r__0 r__0 r__0 ;
gpiPL07 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
gpiPL08 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
gpiPL09 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
gpiPL10 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
gpiPL11 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
gpiPL12 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
gpiPL13 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
gpiPL14 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
gpiPL15 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
gpiPL16 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
gpiPL17 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
gpiPL18 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
gpiPL19 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
gpiPL20 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
gpiPL21 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
gpiPL22 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
gpiPL23 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
gpiPL24 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
gpiPL25 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
gpiPL26 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
gpiPL27 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;
gpiPL28 : r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;

/* header starter for MAVLink 1 messages */
ml14M1 : MAV1CODE LEN14 messageHeaderTailM1 ;
ml28M1 : MAV1CODE LEN28 messageHeaderTailM1 ;

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

/* tail of the message header for MAVLink 1, unsigned, signed messages, respectively */
messageHeaderTailM1 : packetSequence sysID compID ;
messageHeaderTailUS :  incompFlagUS compFlag packetSequence sysID compID ;
messageHeaderTailS :  incompFlagS compFlag packetSequence sysID compID ;

/* Tokens */
MAV1CODE : '\xfe' ;
MAV2CODE : '\xfd' ;

SPCODEM1 : '\x1d' ;
SPCODEM2 : '\x1d' X00 X00 ;
ATTCODEM1 : '\x1e' ;
ATTCODEM2 : '\x1e' X00 X00 ;
GPICODEM1 : '\x21' ;
GPICODEM2 : '\x21' X00 X00 ;

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