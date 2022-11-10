/*
 * hmr.c -- Hammer parser for JSON
 *
 * Author: Stephen Taylor
 *
 */
#include "parser.h"
#include "pp_lalr.h"

/* Set the backend */
#define BKEND PB_LALR /* or PB_LALR, PB_MIN */
/* set location of tests */
#define TESTPATH "../tests/"
/* largest input */
#define MAXINPUT 100000
/* a string to hold the test vectors */
static uint8_t str[MAXINPUT];

static int tvload(char *path,char *test,uint8_t *strp) {
	FILE *fp;
	int c;
	char fname[128];
	uint8_t *p,*endp;
	
	sprintf(fname,"%s%s",path,test);
	if((fp=fopen(fname,"r"))==NULL) {
		printf("unable to open: %s\n",fname);
		return -1;
	}
	p=strp;
	endp=p+MAXINPUT-1;
	do {
		c=fgetc(fp);
		if(c!=EOF) 
			*p++ = (uint8_t)c;
		if(p==endp) {
			printf("\nnot enough space to load %s\n",fname);
			return -1;
		}
	} while(c!=EOF);
	*p='\0';
	if(p==strp) {
		printf("\nempty file %s\n",fname);
		return -1;
	}
	//	printf("%s: %s\n",fname,str);
	return p-strp;
}

int main(int argc, char *argv[]) {
	HParser *parser;
	int len;
	
	if(argc!=2) {
		printf("usage: hmr [ <test_file> | -g ] \n");
		exit(EXIT_FAILURE);
	}

	parser = init_parser();

	if(h_compile(parser,BKEND,NULL)!=0) {
		printf("[Compilation failed]\n");
		exit(EXIT_FAILURE);
	}

	if(strcmp(argv[1],"-g")==0) {
		if(pp_lalr(argv[0],parser)!=0)
			exit(EXIT_FAILURE);
		exit(EXIT_SUCCESS);
	}

	/* running a test */
	if((len = tvload(TESTPATH,argv[1],str))<0) {
		printf("[Unable to load: %s]\n",argv[1]);
		exit(EXIT_FAILURE);
	}
	printf("%s: len=%d\n",argv[1],len);
	if(h_parse(parser, (const uint8_t*)str, len)==0)
		exit(EXIT_FAILURE);
	
	exit(EXIT_SUCCESS);
}
