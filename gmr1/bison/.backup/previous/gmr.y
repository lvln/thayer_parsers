%{
	#define YYDEBUG 1
	int yylex(void);
	void yyerror(char *s);
%}

/* Declaration section */

/* 
 * generatae an LALR parser
 */
%define lr.type lalr

/* 
 * Non-terminal L is the start of the grammar
 */
%start L

%%

/* Here is the grammar */

L : L ';' E
  | E
;
E : E ',' P
  | P
;
P : 'a'
  | '(' M ')'
;
M : /* nothing */
  | L
;
%%
