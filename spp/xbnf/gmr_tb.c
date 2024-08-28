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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bittobyte.h>
#include "gmr.tab.h"


extern int yylex(void);
extern int yydebug;

void yyerror(char *s)
{
	fprintf(stderr, "error: %s\n", s);
}

FILE* yyin;

int main(int argc, char *argv[]) {
	FILE *ifile, *ofile;
	int fieldSizes[] = {3, 1, 1, 11, 2, 14, 16};
	
  if(argc != 2 && argc != 3) {
		printf("usage: gmr <file> [-d]\n");
		exit(EXIT_FAILURE);
	}

	if(!(ifile = fopen(argv[1], "rb"))) {
		printf("Uable to open file %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	if(!(ofile = fopen("tmpfile", "wb"))) {
		printf("Uable to open file tmpfile\n");
		exit(EXIT_FAILURE);
	}

	if (bitToByte(ifile, ofile, fieldSizes, 7) != 0) {
		printf("Uable to convert bit fields to bytes\n");
		exit(EXIT_FAILURE);
	}

	fclose(ifile);
	fclose(ofile);
	
	/* decide if we want to trace the automaton */
	yydebug=0;
	if(argc==3 && strcmp(argv[2],"-d")==0)
		yydebug=1;
	
  if(!(yyin = fopen("tmpfile","r"))) {
		printf("Uable to open file %s\n",argv[1]);
		exit(EXIT_FAILURE);
	}
  if(yyparse()!=0) {
		printf("[FAIL]\n");
		remove("tmpfile");
		exit(EXIT_FAILURE);
	}
	printf("[PASS]\n");

	remove("tmpfile");
  exit(EXIT_SUCCESS);
}

