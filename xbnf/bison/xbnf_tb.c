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
#include "xbnf.tab.h"


extern int yylex();
extern int yydebug;

void yyerror(char *s)
{
	fprintf(stderr, "error: %s\n", s);
}

FILE* yyin;
FILE* xout;

int main(int argc, char *argv[]) {
	char fn[80];
	int arg;
	
	if(argc==2 && strcmp(argv[1],"-h")==0) {
		printf("usage: gmr <infile> [-d -o <outfile>]\n");
		exit(EXIT_SUCCESS);
	}

	yydebug=0;
	strcpy(fn,"output.xbnf");
	for(arg=2; arg<argc; arg++) {
		if(strcmp(argv[arg],"-d")==0)
			yydebug=1;
		else if (strcmp(argv[arg],"-o")==0) 
			strcpy(fn,argv[++arg]);
		else {
			printf("usage: gmr <infile> [-d -o <outfile>]\n");
			exit(EXIT_FAILURE);
		}
	}
	
  if((yyin = fopen(argv[1],"r"))==NULL) {
		printf("Unable to open input file %s\n",argv[1]);
		return EXIT_FAILURE;
	}
	if((xout=fopen(fn,"w"))==NULL) {
		printf("Unable to open output file %s\n",fn);
		return EXIT_FAILURE;
	}	 
	
  if(yyparse()!=0) {
		printf("[FAIL]\n");
		exit(EXIT_FAILURE);
	}

	fclose(yyin);
	fflush(xout);
	fclose(xout);
	//printf("[PASS]\n");
  exit(EXIT_SUCCESS);
}

