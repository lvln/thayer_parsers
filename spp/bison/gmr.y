%{
  #define YYDEBUG 1
  extern int yylex(void);
  void yyerror(char *s);
%}
%token X00
%token BYTE
%%
P : packetPrimaryHeader ;

packetPrimaryHeader : packetVersionNumber packetIdentification packetSequenceControl packetDataLength ;

packetVersionNumber : X00 ;

packetIdentification : packetType secondaryHeaderFlag applicationProcessId ;

packetType : X00 | '\x01' ;

secondaryHeaderFlag : X00 | '\x01' ;

applicationProcessId : WC WC ;

packetSequenceControl : sequenceFlags packetSequenceCountOrPacketName ;

sequenceFlags : X00 | '\x01' | '\x02' | '\x03' ;

packetSequenceCountOrPacketName : WC WC ;

packetDataLength : WC WC ;


/* Consume byte for wildcard */
WC : BYTE ;
