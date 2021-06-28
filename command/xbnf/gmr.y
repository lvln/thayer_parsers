%{
  #define YYDEBUG 1
  int yylex(void);
  void yyerror(char *s);
%}
%token X00
%%
/*
 * command parser
 */
P : AOR | EZ | T ;			/* three tyes of command */
	
AOR : '\x10' C C r__0 ;    /* aor, coord, coord, msgid*/

EZ : '\x20' C C r__0 ;     /* ez, coord, coord , msgid */

T : '\x30' C W r__0 ;      /* target coord, wpnid msgid*/

W : '\x01' | '\x02' ;   /* wpnid */

C : LAT D D LONG D D ;  /* lat min sec long min sec */

D : 