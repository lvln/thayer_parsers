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
%start P

%%

/* Here is the grammar */

P : 'a'
  | '(' M ')'
;
M : /* nothing */
  | P
;
%%
