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
extern uint8_t bufArray[100];
extern int size;
int index = 0;

int yylex (void) {
	//uint8_t c;
	
	//if(fread(&c,sizeof(uint8_t),1,yyin) != 1)
	int temp = bufArray[index++];
	
	if(index-1 == size)
		return 0;
	else if(temp==0x00)
		return X00;
	else
		return temp;
}
