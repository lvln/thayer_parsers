/*
 * main program for loading and running test vectors
 */
#include "parser.h"
#include "pp_xml.h"

 /* Set the backend */
#ifndef BKEND
#define BKEND PB_LALR /* or PB_LALR, PB_MIN */
#endif
/* set location of tests */
#define TESTPATH "../tests/"
/* largest input */
#define MAXINPUT 100000000
#define MAXFN 128
/* a string to hold the test vectors */
static uint8_t buff[MAXINPUT];

static int tvload(char *path,char *test,uint8_t *p) {
	FILE *fp;
	uint8_t b;
	char fname[MAXFN];
	int len;
	
	sprintf(fname,"%s%s",path,test);
	if((fp=fopen(fname,"rb"))==NULL) {
		printf("unable to open: %s\n",fname);
		return -1;
	}
	len=0;
	while(fread(&b,1,1,fp)==1) {
		*p++ = b;
		len++;
		if(len>MAXINPUT)
			return -1;
	}
	return len;
}


int main(int argc, char *argv[]) {
	HParser *parser;
	int len;
	
	if(argc!=2) {
		printf("usage: hmr [ <test_file> | -g ] \n");
		exit(EXIT_FAILURE);
	}
	parser = init_parser();
	/* produce the LALR parsing table */
	if(h_compile(parser,BKEND,NULL)!=0) {
		printf("[Compilation failed]\n");
		exit(EXIT_FAILURE);
	}
	if(strcmp(argv[1],"-g")==0) {
		if(pp_xml(argv[0],parser)!=0)
			exit(EXIT_FAILURE);
		exit(EXIT_SUCCESS);
	}
	/* running a test */
	if((len = tvload(TESTPATH,argv[1],buff))<0) {
		printf("[Unable to load: %s]\n",argv[1]);
		exit(EXIT_FAILURE);
	}
	printf("%s: len=%d\n",argv[1],len);
	if(h_parse(parser, (const uint8_t*)buff, len)==0)
		exit(EXIT_FAILURE);
	
	exit(EXIT_SUCCESS);
}
