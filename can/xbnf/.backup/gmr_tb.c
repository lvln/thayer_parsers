/* 
 * gmr_tb.c -- parse a file using the gmr
 * 
 * Author: st
 * Created: 06-22-2020
 * Version: 1.0
 * 
 * Description: 
 * 
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gmr.tab.h"


extern int yylex(void);
extern int yydebug;

void yyerror(char *s)
{
	fprintf(stderr, "error: %s\n", s);
}

FILE* yyin;

/* Start of Reformat Section */
int nbits = 0;
uint32_t buffer = 0;
uint8_t size = 0;
int bufArray[100] = {0};

/*
 * Function flips the right most 16 bits
 * @param[in] 'num' integer value to flip
 * @param[in] 'numBits' number of bits to flip
 * @return 'temp' variable that returns newly flipped integer value that is shifted right 16 - numBits
 */
int flipBits(int num, int numBits)
{
	int temp = num;
	temp = ((temp & 0x5555) << 1) | ((temp & 0xAAAA) >> 1);
	temp = ((temp & 0x3333) << 2) | ((temp & 0xCCCC) >> 2);
	temp = ((temp & 0x0F0F) << 4) | ((temp & 0xF0F0) >> 4);
	temp = ((temp & 0x00FF) << 8) | ((temp & 0xFF00) >> 8);
	return temp >> (16 - numBits);
}

void fillBuffer()
{
	uint8_t temp = 0;
	do
	{
		printf("\nFill Buffer Loop: %d < %d; nbits: %d\n", buffer, 1U << nbits, nbits);
		if(buffer < (1U << nbits))
		{
			if(fread(&temp,sizeof(uint8_t),1,yyin) != 1) { printf("\nEOF\n");return; }
			buffer = buffer | (flipBits(temp, 8) << nbits);
			printf("\nFill Buffer Input: %d\n", temp);
			nbits = nbits + 8;
		}
	} while(nbits <= 24);
}

int getBits(int numBits)
{
	int temp;
	if(numBits > nbits)
		fillBuffer();
	temp = flipBits(buffer, numBits);
	buffer = buffer >> numBits;
	printf("\nGet Bits: %d\n", numBits);
	printf("\nGet Bits Input: %d\n", temp);
	nbits = nbits - numBits;
	return temp;
}

void reformat(uint8_t *msgDict, uint8_t szMsgDict, /*uint8_t *inMsg, uint8_t szMsg,*/ uint8_t *ioBufArray, uint8_t *outSz)
{
	int index = *outSz;
	/*int loop = 0;
	bufArray[index++] = getBits(1);
	bufArray[index++] = getBits(3);
	bufArray[index++] = getBits(8);
	bufArray[index++] = getBits(1);
	bufArray[index++] = getBits(2);
	loop = bufArray[index++] = getBits(4);
	for(int ii = 1; ii <= loop; ii++)
	{
		printf("\nLoop: %d/%d\n", ii, loop);
		bufArray[index++] = getBits(8);
	}
	bufArray[index++] = getBits(7);
	bufArray[index++] = getBits(8);
	bufArray[index++] = getBits(1);
	bufArray[index++] = getBits(1);
	bufArray[index++] = getBits(1);
	bufArray[index++] = getBits(7);*/
	for(int zz = 0; zz < szMsgDict; zz++)
	{
		if(msgDict[zz] > 8)
		{
			int temp = msgDict[zz] % 8;
			if(temp > 0)
			{
				ioBufArray[index++] = getBits(temp);
				temp = msgDict[zz] - temp;
			}
			else
			{
				temp = msgDict[zz];
			}
			do
			{
				ioBufArray[index++] = getBits(temp);
				temp = temp - 8;
			} while(temp >= 8);
		}
		else
		{
			ioBufArray[index++] = getBits(msgDict[zz]);
		}
	}
	*outSz = index;
}

void reformatMain()
{
  printf("\nBegin Reformat!\n");
  uint8_t msgDict[] = { 1,11,1,2,4,8,15,1,1,1,7 };
  uint8_t tempArray[100] = {0};
  int loop = 0;
  reformat(msgDict, 5, tempArray, &size);
  loop = tempArray[size-1];
  for(int ii = 1; ii <= loop; ii++)
  {
  	printf("\nLoop: %d/%d\n", ii, loop);
  	reformat(msgDict + 5, 1, tempArray, &size);
  }
  reformat(msgDict + 6, 5, tempArray, &size);
  printf("\nFinished Reformat!\n");
  printf("\nMessage Size: %d\n", size);
  for(int ii = 0; ii < size; ii++)
  {
  	bufArray[ii] = tempArray[ii];	
  }
  return;
}
/* End of Reformat Section */

int main(int argc, char *argv[]) {

  if(argc != 2 && argc != 3) {
		printf("usage: gmr <file> [-d]\n");
		exit(EXIT_FAILURE);
	}

	/* decide if we want to trace the automaton */
	yydebug=0;
	if(argc==3 && strcmp(argv[2],"-d")==0)
		yydebug=1;
	
  if(!(yyin = fopen(argv[1],"r"))) {
		printf("Uable to open file %s\n",argv[1]);
		return EXIT_FAILURE;
	}
	
  reformatMain();
  	
  if(yyparse()!=0) {
		printf("[FAIL]\n");
		exit(EXIT_FAILURE);
	}
	printf("[PASS]\n");
  exit(EXIT_SUCCESS);
}

