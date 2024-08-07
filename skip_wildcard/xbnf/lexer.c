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

int yylex (void) {
	uint8_t c;

	if(fread(&c,sizeof(uint8_t),1,yyin) != 1) {
		printf("End of file in lexer.c.\n");
		return 0;
	}
	else if(c==0x00) {
		printf("Terminal 0 in lexer.c.\n");
		return X00;
	}
	printf("%d read in lexer.c.\n", c);
	return c;
}
