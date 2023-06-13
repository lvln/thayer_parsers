/* 
 * gmr_tb.c -- parse a file using the gmr
 * 
 * Author: jr
 * Created: 04-06-2023
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

extern int yylex();
extern int yydebug;

void yyerror(char *s)
{
	fprintf(stderr, "error: %s\n", s);
}

FILE* yyin;

/* Start of Reformat Section */
int nbits = 0; // number of bits stored in buffer
uint32_t buffer = 0; // input storage for bit extraction
uint8_t size = 0; // size of bufArray
uint8_t bufArray[128] = {0}; // array for the parser

/*
 * Function reads from file 
 * @param[out] 'ioMsg' pointer to array for input from file
 * @param[out] 'szMsg' size of array
 */
void readFile(uint8_t *ioMsg, uint8_t *szMsg)
{
	uint8_t temp = 0;
	uint8_t index = 0;
	while(fread(&temp,sizeof(uint8_t),1,yyin) == 1) 
	{ 
		ioMsg[index++] = temp;
	}
	*szMsg = index;
	return;
}

/*
 * Function flips all 32 bits and extract result by numBits 
 * @param[in] 'num' integer value to flip
 * @param[in] 'numBits' number of bits to flip
 * @return 'temp' variable that returns newly flipped integer value that is shifted right by 32 - numBits
 */
uint32_t flipBits(const uint32_t num, const uint32_t numBits)
{
	uint32_t temp = num;
	temp = ((temp & 0x55555555) << 1) | ((temp & 0xAAAAAAAA) >> 1);
	temp = ((temp & 0x33333333) << 2) | ((temp & 0xCCCCCCCC) >> 2);
	temp = ((temp & 0x0F0F0F0F) << 4) | ((temp & 0xF0F0F0F0) >> 4);
	temp = ((temp & 0x00FF00FF) << 8) | ((temp & 0xFF00FF00) >> 8);
	temp = ((temp & 0x0000FFFF) << 16) | ((temp & 0xFFFF0000) >> 16);
	return temp >> (32 - numBits);
}

/*
 * Function fills buffer for input
 * @param[in] 'inMsg' pointer to array with data input
 * @param[in] 'szMsg' size of array for 'inMsg'
 * @param[in out] 'msgFront' current index position for read from 'inMsg'
 * @warning 'buffer' and 'nbits' must be synchronized to avoid any known errors. If 
 * 	the value in 'buffer' is ever greater than or equal to "(1 << nbits)" the 
 * 	function will be trapped in an infinite loop.
 * @note When 'msgFront' is greater than or equal to 'szMsg' the function will exit signifying EOF has been reached
 */
void fillBuffer(const uint8_t *inMsg, const uint8_t szMsg, uint8_t *msgFront)
{
	uint8_t temp = 0;
	uint8_t front = *msgFront;
	do
	{
		#ifdef DEBUG
			printf("\n----Fill Buffer Loop: 0x%.8X < 0x%.8X; nbits: %d\n", buffer, 1U << nbits, nbits);
		#endif
		if(buffer < (1U << nbits))
		{
			#ifdef DEBUG
				printf("\n----Front: %d\n----szMsg: %d\n", front, szMsg);
			#endif
			if(front >= szMsg) {
				#ifdef DEBUG
					printf("\n----EOF----\n");
				#endif
				*msgFront = front; return; 
			}
			temp = flipBits(inMsg[front++], 8);
			buffer = buffer | (temp << nbits);
			nbits = nbits + 8;
			#ifdef DEBUG
				printf("\n----Fill Buffer Input: 0x%.2X\n----Current Buffer: 0x%.8X\n", temp, buffer);
			#endif
		}
	} while(nbits <= 24);
	*msgFront = front;
}

/*
 * Function extracts requested number of bits from buffer
 * @param[in] 'numBits' the number of bits to extract from buffer. Must be within range [0, 32]
 * @param[in] 'inMsg' pointer to array with data input
 * @param[in] 'szMsg' size of array for 'inMsg'
 * @param[in out] 'msgFront' current index position for read from 'inMsg'
 * @warning Much like the function 'fillBuffer' the user must take care that 'buffer' and
 * 	'nbits' are synchronized to avoid any known errors. If 'nbits' is ever less than zero,
 * 	the function will still extract 'numBits' number of bits from buffer and should be used
 * 	to signify an error such as an EOF state from 'fillBuffer'. The function will not fail
 * 	and will return whatever value remains from buffer.
 * @note The parameters 'inMsg', 'szMsg', and 'msgFront' is passed into fill buffer when the requested 
 * 	number of bits is greater than the number of bits actually stored in the buffer
 * @return 'temp' result of extracted bits from buffer
 */
uint32_t getBits(const uint8_t numBits, const uint8_t *inMsg, const uint8_t szMsg, uint8_t *msgFront)
{
	uint32_t temp;
	if(numBits > nbits)
		fillBuffer(inMsg, szMsg, msgFront);
	#ifdef DEBUG
		printf("\n--szMsg: %d\n\n--Get Bits: %d\n--Current Buffer: 0x%.8X\n--Current Bits: %d\n", szMsg, numBits, buffer, nbits);
	#endif
	temp = buffer & (((uint64_t)1 << numBits) - 1);
	buffer = (uint64_t)buffer >> numBits;
	nbits = nbits - numBits;
	temp = flipBits(temp, numBits);
	#ifdef DEBUG
		printf("\n--Get Bits Output: 0x%.8X\n--Current Buffer After: 0x%.8X\n--Current Bits After: %d\n", temp, buffer, nbits);
	#endif
	return temp;
}

/*
 * Endian Byte Conversion function extracts requested number of bits from buffer
 * @param[in] 'numBits' the number of bits to extract from buffer. Must be within range [0, 32]
 * @param[in out] 'offset' the number of bits that are flipped within the buffer
 * @param[in] 'inMsg' pointer to array with data input
 * @param[in] 'szMsg' size of array for 'inMsg'
 * @param[in out] 'msgFront' current index position for read from 'inMsg'
 * @warning Much like the function 'fillBuffer' the user must take care that 'buffer' and
 * 	'nbits' are synchronized to avoid any known errors. If 'nbits' is ever less than zero,
 * 	the function will still extract 'numBits' number of bits from buffer and should be used
 * 	to signify an error such as an EOF state from 'fillBuffer'. The function will not fail
 * 	and will return whatever value remains from buffer.
 * @note The parameters 'inMsg', 'szMsg', and 'msgFront' is passed into fill buffer when the requested 
 * 	number of bits is greater than the number of bits actually stored in the buffer
 * @return 'temp' result of extracted bits from buffer
 */
uint32_t getBitsEBC(const uint8_t numBits, int8_t *offset, const uint8_t *inMsg, const uint8_t szMsg, uint8_t *msgFront)
{
	uint32_t temp, mask;
	if(numBits > nbits)
		fillBuffer(inMsg, szMsg, msgFront);
	if(numBits > *offset) { //if true, flip next byte in sequence
		mask = 0xFF << *offset;
		temp = flipBits((buffer&mask), *offset+8) << *offset;
		buffer = (buffer & ~mask) | temp;
		*offset = *offset + 8;
	}
	temp = buffer & (((uint64_t)1 << numBits) - 1);
	buffer = (uint64_t)buffer >> numBits;
	nbits = nbits - numBits;
	*offset = *offset - numBits;
	return temp;
}

/*
 * Reformat input data from Big Endian to Little Endian fromat
 * @param[in] 'msgDict' pointer to array that holds relevent data to 
 * 	reformat 'inMsg' data. The data must use the following structure to be valid:
 * 		{ Reformat1, Reformat2, Reformat3, . . . , ReformatM } 
 * 	where each 'Reformat' subtype is:
 * 		{ Type, Length, BitSize1, BitSize2, BitSize3, . . . , BitSizeN }
 * 		{ Type = 2, Length, BitSize }
 * 	where:
 * 		Type: { 0 = normal parse, 1 = BE <-> LE, 2 = variable length parsing }
 * 		Length: Read the next N number of BitSize values where N is Length.
 * 			If type = 2, read N number of bits to get Length of message.
 * 		BitSize: Number of bits to read and convert into 8-bit oriented values.
 * @param[in] 'szMsgDict' size of array for 'msgDict'
 * @param[in] 'inMsg' pointer to array with data input
 * @param[in] 'szMsg' size of array for 'inMsg'
 * @param[in out] 'msgFront' current index position for read from 'inMsg'
 * @param[in out] 'outMsg' output for reformatted message
 * @param[in out] 'outSz' size of message during reformat
 * @note Function is unable to handle data of unknown length. Current workaround is to split the 
 * 	reformat implementation into multiple calls.
 */
void reformat(const uint8_t *msgDict, const uint8_t szMsgDict, const uint8_t *inMsg, const uint8_t szMsg, uint8_t *msgFront, uint8_t *outMsg, uint8_t *outSz)
{
	int8_t index,length;
	uint8_t zz,type,temp,tmpArr[32];
	for(zz = 0; zz < szMsgDict; zz++)
	{
		temp = 0;
		type = msgDict[zz++];
		if(type==0) {
			length = msgDict[zz];
			for(;length>0;length--) {
				temp = msgDict[++zz];
				type = temp % 8;
				if(type>0) {
					temp = temp - type;
					outMsg[(*outSz)++] = getBits(type, inMsg, szMsg, msgFront);
				}
				while(temp>=8) {
					temp = temp - 8;
					outMsg[(*outSz)++] = getBits(8, inMsg, szMsg, msgFront);
				}
			}
		}
		else if(type==1) {
			index = length = 0;
			for(type = msgDict[zz];type>0;type--) {
				temp = msgDict[zz+type];
				while(temp>=8) {
					tmpArr[index++] = getBitsEBC(8, &length, inMsg, szMsg, msgFront);
					temp = temp - 8;
				}
				if(temp > 0) {
					tmpArr[index++] = getBitsEBC(temp, &length, inMsg, szMsg, msgFront);
				}
			}
			if(length>0) {
				buffer = getBits(length, inMsg, szMsg, msgFront) | (buffer << length); nbits = nbits + length;
			}
			zz = msgDict[zz] + zz;
			for(;index>0;index--) {
				outMsg[(*outSz)++] = tmpArr[index-1];
			}
		}
		else if(type==2) {
			temp = msgDict[zz++];
			length = getBits(temp, inMsg, szMsg, msgFront);
			outMsg[(*outSz)++] = length;
			temp = msgDict[zz];
			for(;length>0;length--) {
				outMsg[(*outSz)++] = getBits(temp, inMsg, szMsg, msgFront);
			}
		}
	}
}

/*
 * Main function where reformatting takes place
 */
void reformatMain()
{
  uint8_t msgDict[] =	{ 0,5,1,11,1,1,1,2,4,8,0,5,15,1,1,1,7 };//17
  uint8_t dataMsg[64] = {0};
  uint8_t front, back;
  front = back = 0;
  
  readFile(dataMsg, &back);
  printf("==========================================");
  printf("\nFinished Read!\n");
  printf("==========================================\n");
  
  for(int ii = 0; ii < back; ii++)
  {
  	printf("\nInput %d: 0x%.2X", ii, dataMsg[ii]);
  }
  
  printf("\n==========================================");
  printf("\nMessage Size: %d\n", back);
  printf("==========================================\n");
  printf("\n==========================================");
  printf("\nBegin Reformat!\n");
  printf("==========================================\n");
  
  buffer = 0; nbits = 0; // Flush buffer data

  reformat(msgDict, 17, dataMsg, back, &front, bufArray, &size);
  
  printf("\n==========================================");
  printf("\nFinished Reformat!\n");
  printf("==========================================\n");
  
  for(int ii = 0; ii < size; ii++)
  {
  	printf("\nInput %d: 0x%.2X", ii, bufArray[ii]);
  }
  
  printf("\n==========================================");
  printf("\nMessage Size: %d\n", size);
  printf("==========================================\n");
  
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

