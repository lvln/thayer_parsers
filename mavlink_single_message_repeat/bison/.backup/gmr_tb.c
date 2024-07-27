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
#include "gmr.tab.h"

extern FILE* yyin;
extern int yylex();
extern int yydebug;

void yyerror(char *s)
{
	fprintf(stderr, "error: %s\n", s);
}


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
  if(yyparse()!=0) {
		printf("[FAIL]\n");
		exit(EXIT_FAILURE);
	}
	printf("[PASS]\n");
  exit(EXIT_SUCCESS);
}

