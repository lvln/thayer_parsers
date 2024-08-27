%{
  #define YYDEBUG 1
  int yylex(void);
  void yyerror(char *s);
%}
%token X00
%%
P : packetPrimaryHeader ;

packetPrimaryHeader : packetVersionNumber packetIdentification packetSequenceControl packetDataLength ;

packetVersionNumber : X00 X00 X00 ;

packetIdentification : packetType secondaryHeaderFlag applicationProcessId ;

packetType : X00 | '\x01' ;

secondaryHeaderFlag : X00 | '\x01' ;

applicationProcessId : wildCardBit wildCardBit wildCardBit wildCardBit wildCardBit wildCardBit wildCardBit wildCardBit wildCardBit wildCardBit wildCardBit ;

packetSequenceControl : sequenceFlags packetSequenceCountOrPacketName ;

sequenceFlags : X00 X00 | X00 '\x01' | '\x01' X00 | '\x01' '\x01' ;

packetSequenceCountOrPacketName : wildCardBit wildCardBit wildCardBit wildCardBit wildCardBit wildCardBit wildCardBit wildCardBit wildCardBit wildCardBit wildCardBit wildCardBit wildCardBit wildCardBit ;

packetDataLength : wildCardBit wildCardBit wildCardBit wildCardBit wildCardBit wildCardBit wildCardBit wildCardBit wildCardBit wildCardBit wildCardBit wildCardBit wildCardBit wildCardBit wildCardBit wildCardBit ;

wildCardBit : X00 | '\x01' ;