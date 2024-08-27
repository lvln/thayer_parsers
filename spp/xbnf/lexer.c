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
#include "gmr.tab.h"

extern FILE *yyin;
static int i = 0;
static uint8_t c;

int yylex (void) {
	uint8_t b;
	
	if (i == 0)
		if(fread(&c,sizeof(uint8_t),1,yyin) != 1)
			return 0;
 
	b = (c >> (7 - i)) & 1;
	
	if (i < 7) i++;
	else i = 0;

	if (b == 0) return X00;
	else return 0x01;

}

