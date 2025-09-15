/* 
 * lexer.c --- a binary scanner
 * 
 * Author: Stephen Taylor
 * Created: 04-08-2021
 * Version: 1.0
 * 
 * Description: 
 * 
 */
#include <stdio.h>
#include <stdint.h>
#include "xbnf.tab.h"

extern FILE *yyin;
extern int yylval;

int yylex (void) {
	uint8_t c;

	if(fread(&c,sizeof(uint8_t),1,yyin) != 1)
		return 0;
	else if(c==0x00)
		return X00;
	yylval=c;
	return c;
}

