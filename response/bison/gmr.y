%{
  #define YYDEBUG 1
  extern int yylex(void);
  void yyerror(char *s);
%}
%token X00
%token BYTE
%%
/*
 * Response Parser
 */
P : IT | WA | FF | CA | ACK | NCK ;

IT : '\x40' WC ; /* invalid target */

WA : '\x50' WC ; /* weapon away */

FF : '\x60' WC ; /* firing fault */

CA : '\x70' WC ; /* cyber alert */

ACK : '\x80' WC ; /* ack */

NCK : '\x81' WC ; /* nack */


/* Consume byte for wildcard */
WC : BYTE ;
