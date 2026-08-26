/* library inclusions */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <errno.h>	

#define MAXNUM 100                             /* max number of digits in number */
#define MAXENTRY 100						               /* max number of separate ranges */
#define RMIN 0									               /* index of min range value */
#define RMAX 1									               /* index of max range value */
#define ENUMMAX 10							               /* max number of entries in an enumeration */
#define CHARMAX 50                             /* max number of characters in a string */

/* variable defined in xbnf_tb.c */
extern FILE* xout;                             /* output bison file */

/* variables used externally in xbnf.y */
int linenum = 1;                               /* line number - used for error reporting */
int hval;						                           /* variable used to form a hex value from ascii chars */
int cval;                                      /* variable used to hold a character value */
bool anybyte = false;		                       /* true if anybyte is used in the grammar */
bool ranging = false;		                       /* true if range */
bool fixedwidth = false;                       /* true if currently dealing with a fixed width integer */
int strings[MAXENTRY][CHARMAX];                /* any strings that are found within rules */
int c = 0;                                     /* variable used to hold character value for a string */
int nstr = 0;                                  /* number of strings */
char decimalstr[2][MAXNUM];                    /* string to read number into */
int nnum = 0;                                  /* how many numbers have been read in as strings */
int i = 0;                                     /* index variable for number string */
int size = 8;                                  /* size of fixed-width integer */
bool neg = false;                              /* true if negative sign is present at start of fixed-width integer */
bool unsign = true;                            /* true if fixed-width integer is unsigned */
bool le = false;                               /* true if fixed-width integer is of little-endian representation */
bool be = false;                               /* true if fixed-width integer is network byte order */

/* variables used locally within xbnftobison.c */
static int nextrange = 0;				               /* number of ranges */
static int nextir16le = 0;                     /* number of 2 byte signed little endian ranges */
static int nextir32le = 0;                     /* number of 4 byte signed little endian ranges */
static int nextir64le = 0;                     /* number of 8 byte signed little endian ranges */
static int nextir8be = 0;                      /* number of 1 byte signed big endian ranges */
static int nextir16be = 0;                     /* number of 2 byte signed big endian ranges */
static int nextir32be = 0;                     /* number of 4 byte signed big endian ranges */
static int nextir64be = 0;                     /* number of 8 byte signed big endian ranges */
static int nextuir16le = 0;                    /* number of 2 byte unsigned little endian ranges */
static int nextuir32le = 0;                    /* number of 4 byte unsigned little endian ranges */
static int nextuir64le = 0;                    /* number of 8 byte unsigned little endian ranges */
static int nextuir8be = 0;                     /* number of 1 byte unsigned big endian ranges */
static int nextuir16be = 0;                    /* number of 2 byte unsigned big endian ranges */
static int nextuir32be = 0;                    /* number of 4 byte unsigned big endian ranges */
static int nextuir64be = 0;                    /* number of 8 byte unsigned big endian ranges */
static int nextenum = 0;				               /* number of enumerations */
static int eindex = 0;                         /* index into an emumeration */
static int rlow;							                 /* low value in a range */
static int rhigh;						                   /* high value in a range */
static int range[MAXENTRY][2];			           /* the ranges */
static int64_t int16lerange[MAXENTRY][2];			 /* the ranges for signed 2 byte little endian */
static int64_t int32lerange[MAXENTRY][2];		   /* the ranges for signed 4 byte little endian */
static int64_t int64lerange[MAXENTRY][2];	 	   /* the ranges for signed 8 byte little endian */
static int64_t int8berange[MAXENTRY][2]; 		   /* the ranges for signed 1 byte big endian */
static int64_t int16berange[MAXENTRY][2];			 /* the ranges for signed 2 byte big endian */
static int64_t int32berange[MAXENTRY][2];			 /* the ranges for signed 4 byte big endian */
static int64_t int64berange[MAXENTRY][2];			 /* the ranges for signed 8 byte big endian */
static uint64_t uint16lerange[MAXENTRY][2];		 /* the ranges for unsigned 2 byte little endian */
static uint64_t uint32lerange[MAXENTRY][2];		 /* the ranges for unsigned 4 byte little endian */
static uint64_t uint64lerange[MAXENTRY][2];		 /* the ranges for unsigned 8 byte little endian */
static uint64_t uint8berange[MAXENTRY][2];		 /* the ranges for unsigned 1 byte big endian */
static uint64_t uint16berange[MAXENTRY][2];		 /* the ranges for unsigned 2 byte big endian */
static uint64_t uint32berange[MAXENTRY][2];		 /* the ranges for unsigned 4 byte big endian */
static uint64_t uint64berange[MAXENTRY][2];		 /* the ranges for unsigned 8 byte big endian */
static int enumeration[MAXENTRY][ENUMMAX];     /* the enumerations */
static int64_t decimalsig = 0;                 /* signed decimal number's value */
static uint64_t decimalunsig = 0;              /* unsigned decimal number's value */
static int lowsize = 8;                        /* size of fixed-width integer at lower bound of range */
static bool lowunsign = true;                  /* sign of fixed-width integer at lower bound of range */
static int lowle = false;                      /* endianness of fixed-width integer at lower bound of range */
static int lowbe = false;                      /* endianness of fixed-width integer at lower bound of range */

/* sets the low bound on the ranging of a fixed-width integer */
static void setfwrlow(void) {
	char *endptr; 
	if (size == 16 && le) {
		if (unsign && neg) {
			printf("error: line %d - invalid range start\n", linenum);
			exit(EXIT_FAILURE);
		}
		if (!unsign) {
			decimalsig = strtoll(decimalstr[0], &endptr, 10);
			if (*endptr != '\0' || (decimalsig < INT16_MIN || decimalsig > INT16_MAX)) {
				printf("error: line %d - invalid range start\n", linenum);
				exit(EXIT_FAILURE);
			}
			int16lerange[nextir16le][0] = decimalsig;
		}
		if (unsign) {
			decimalunsig = strtoull(decimalstr[0], &endptr, 10);
			if (*endptr != '\0'|| (decimalunsig < 0 || decimalunsig > UINT16_MAX)) {   
				printf("error: line %d - invalid range start\n", linenum);
				exit(EXIT_FAILURE);          
			}
			uint16lerange[nextuir16le][0] = decimalunsig;
		}
	}
	if (size == 32 && le) {
		if (unsign && neg) {
			printf("error: line %d - invalid range start\n", linenum);
			exit(EXIT_FAILURE);
		}
		if (!unsign) {
			decimalsig = strtoll(decimalstr[0], &endptr, 10);
			if (*endptr != '\0' || (decimalsig < INT32_MIN || decimalsig > INT32_MAX)) {
				printf("error: line %d - invalid range start\n", linenum);
				exit(EXIT_FAILURE);
			}
			int32lerange[nextir32le][0] = decimalsig;
		}
		if (unsign) {
			decimalunsig = strtoull(decimalstr[0], &endptr, 10);
			if (*endptr != '\0'|| (decimalunsig < 0 || decimalunsig > UINT32_MAX)) {
				printf("error: line %d - invalid range start\n", linenum);
				exit(EXIT_FAILURE);          
			}
			uint32lerange[nextuir32le][0] = decimalunsig;
		}
	}
	if (size == 64 && le) {
		if (unsign && neg) {
			printf("error: line %d - invalid range start\n", linenum);
			exit(EXIT_FAILURE);
		}
		if (!unsign) {
			errno = 0;
			decimalsig = strtoll(decimalstr[0], &endptr, 10);
			if (*endptr != '\0' || errno == ERANGE) {
				printf("error: line %d - invalid range start\n", linenum);
				exit(EXIT_FAILURE);
			}
			int64lerange[nextir64le][0] = decimalsig;
		}
		if (unsign) {
			errno = 0;
			decimalunsig = strtoull(decimalstr[0], &endptr, 10);
			if (*endptr != '\0'|| errno == ERANGE) {
				printf("error: line %d - invalid range start\n", linenum);
				exit(EXIT_FAILURE);          
			}
			uint64lerange[nextuir64le][0] = decimalunsig;
		}
	}
	if (size == 8 && be) {
		if (unsign && neg) {
			printf("error: line %d - invalid range start\n", linenum);
			exit(EXIT_FAILURE);
		}
		if (!unsign) {
			decimalsig = strtoll(decimalstr[0], &endptr, 10);
			if (*endptr != '\0' || (decimalsig < INT8_MIN || decimalsig > INT8_MAX)) {
				printf("error: line %d - invalid range start\n", linenum);
				exit(EXIT_FAILURE);
			}
			int8berange[nextir8be][0] = decimalsig;
		}
		if (unsign) {
			decimalunsig = strtoull(decimalstr[0], &endptr, 10);
			if (*endptr != '\0'|| (decimalunsig < 0 || decimalunsig > UINT8_MAX)) {   
				printf("error: line %d - invalid range start\n", linenum);
				exit(EXIT_FAILURE);          
			}
			uint8berange[nextuir8be][0] = decimalunsig;
		}
	}
	if (size == 16 && be) {
		if (unsign && neg) {
			printf("error: line %d - invalid range start\n", linenum);
			exit(EXIT_FAILURE);
		}
		if (!unsign) {
			decimalsig = strtoll(decimalstr[0], &endptr, 10);
			if (*endptr != '\0' || (decimalsig < INT16_MIN || decimalsig > INT16_MAX)) {
				printf("error: line %d - invalid range start\n", linenum);
				exit(EXIT_FAILURE);
			}
			int16berange[nextir16be][0] = decimalsig;
		}
		if (unsign) {
			decimalunsig = strtoull(decimalstr[0], &endptr, 10);
			if (*endptr != '\0'|| (decimalunsig < 0 || decimalunsig > UINT16_MAX)) {   
				printf("error: line %d - invalid range start\n", linenum);
				exit(EXIT_FAILURE);          
			}
			uint16berange[nextuir16be][0] = decimalunsig;
		}
	}
	if (size == 32 && be) {
		if (unsign && neg) {
			printf("error: line %d - invalid range start\n", linenum);
			exit(EXIT_FAILURE);
		}
		if (!unsign) {
			decimalsig = strtoll(decimalstr[0], &endptr, 10);
			if (*endptr != '\0' || (decimalsig < INT32_MIN || decimalsig > INT32_MAX)) {
				printf("error: line %d - invalid range start\n", linenum);
				exit(EXIT_FAILURE);
			}
			int32berange[nextir32be][0] = decimalsig;
		}
		if (unsign) {
			decimalunsig = strtoull(decimalstr[0], &endptr, 10);
			if (*endptr != '\0'|| (decimalunsig < 0 || decimalunsig > UINT32_MAX)) {
				printf("error: line %d - invalid range start\n", linenum);
				exit(EXIT_FAILURE);          
			}
			uint32berange[nextuir32be][0] = decimalunsig;
		}
	}
	if (size == 64 && be) {
		if (unsign && neg) {
			printf("error: line %d - invalid range start\n", linenum);
			exit(EXIT_FAILURE);
		}
		if (!unsign) {
			errno = 0;
			decimalsig = strtoll(decimalstr[0], &endptr, 10);
			if (*endptr != '\0' || errno == ERANGE) {
				printf("error: line %d - invalid range start\n", linenum);
				exit(EXIT_FAILURE);
			}
			int64berange[nextir64be][0] = decimalsig;
		}
		if (unsign) {
			errno = 0;
			decimalunsig = strtoull(decimalstr[0], &endptr, 10);
			if (*endptr != '\0'|| errno == ERANGE) {
				printf("error: line %d - invalid range start\n", linenum);
				exit(EXIT_FAILURE);          
			}
			uint64berange[nextuir64be][0] = decimalunsig;
		}
	}
	lowsize = size;
	if (le) lowle = true;
	if (be) lowbe = true;
	if (unsign) lowunsign = true;
	else lowunsign = false;
	
	le = false;
	be = false;
	decimalsig = 0;
	decimalunsig = 0;
}

/* set the high bound on the ranging of a fixed-width integer */
static void setfwrhigh(void) {
	char *endptr;
	
	if (lowsize != size || (lowle && !le) || (lowbe && !be) || (lowunsign && !unsign) || (!lowunsign && unsign)) {
		printf("error: line %d - invalid range end\n", linenum);
		exit(EXIT_FAILURE);
	}
	
	if (size == 16 && le) {
		if (unsign && neg) {
			printf("error: line %d - invalid range end\n", linenum);
			exit(EXIT_FAILURE);
		}
		if (!unsign) {
			decimalsig = strtoll(decimalstr[1], &endptr, 10);
			if (*endptr != '\0' || (decimalsig < INT16_MIN || decimalsig > INT16_MAX) || decimalsig < int16lerange[nextir16le][0]) {
				printf("error: line %d - invalid range end\n", linenum);
				exit(EXIT_FAILURE);
			}
			int16lerange[nextir16le][1] = decimalsig;
			fprintf(xout, "ir16le__%d", nextir16le);
			if (int16lerange[nextir16le][0] == INT16_MIN && int16lerange[nextir16le][1] == INT16_MAX) anybyte = true;
			nextir16le++;
		}
		if (unsign) {
			decimalunsig = strtoull(decimalstr[1], &endptr, 10);
			if(*endptr != '\0'|| (decimalunsig < 0 || decimalunsig > UINT16_MAX) || decimalunsig < uint16lerange[nextuir32le][0]) {   
				printf("error: line %d - invalid range end\n", linenum);
				exit(EXIT_FAILURE);
			}
			uint16lerange[nextuir16le][1] = decimalunsig;
			fprintf(xout, "uir16le__%d", nextuir16le);
			if (uint16lerange[nextuir16le][0] == 0 && uint16lerange[nextuir16le][1] == UINT16_MAX) anybyte = true;
			nextuir16le++;
		}
	}
	if (size == 32 && le) {
		if (unsign && neg) {
			printf("error: line %d - invalid range end\n", linenum);
			exit(EXIT_FAILURE);
		}
		if (!unsign) {
			decimalsig = strtoll(decimalstr[1], &endptr, 10);
			if (*endptr != '\0' || (decimalsig < INT32_MIN || decimalsig > INT32_MAX) || decimalsig < int32lerange[nextir32le][0]) {
				printf("error: line %d - invalid range end\n", linenum);
				exit(EXIT_FAILURE);
			}
			int32lerange[nextir32le][1] = decimalsig;
			fprintf(xout, "ir32le__%d", nextir32le);
			if (int32lerange[nextir32le][0] == INT32_MIN && int32lerange[nextir32le][1] == INT32_MAX) anybyte = true;
			nextir32le++;
		}
		if (unsign) {
			decimalunsig = strtoull(decimalstr[1], &endptr, 10);
			if(*endptr != '\0'|| (decimalunsig < 0 || decimalunsig > UINT32_MAX) || decimalunsig < uint32lerange[nextuir32le][0]){   
				printf("error: line %d - invalid range end\n", linenum);
				exit(EXIT_FAILURE);
			}
			uint32lerange[nextuir32le][1] = decimalunsig;
			fprintf(xout, "uir32le__%d", nextuir32le);
			if (uint32lerange[nextuir32le][0] == 0 && uint32lerange[nextuir32le][1] == UINT32_MAX) anybyte = true;
			nextuir32le++;
		}
	}
	if (size == 64 && le) {
		if (unsign && neg) {
			printf("error: line %d - invalid range end\n", linenum);
			exit(EXIT_FAILURE);
		}
		if (!unsign) {
			errno = 0;
			decimalsig = strtoll(decimalstr[1], &endptr, 10);
			if (*endptr != '\0' || errno == ERANGE || decimalsig < int64lerange[nextir64le][0]) {
				printf("error: line %d - invalid range end\n", linenum);
				exit(EXIT_FAILURE);
			}
			int64lerange[nextir64le][1] = decimalsig;
			fprintf(xout, "ir64le__%d", nextir64le);
			if (int64lerange[nextir64le][0] == INT64_MIN && int64lerange[nextir64le][1] == INT64_MAX) anybyte = true;
			nextir64le++;
		}
		if (unsign) {
			decimalunsig = strtoull(decimalstr[1], &endptr, 10);
			if(*endptr != '\0'|| errno == ERANGE || decimalunsig < uint64lerange[nextuir64le][0]) {
				printf("error: line %d - invalid range end\n", linenum);
				exit(EXIT_FAILURE);
			}
			uint64lerange[nextuir64le][1] = decimalunsig;
			fprintf(xout, "uir64le__%d", nextuir64le);
			if (uint64lerange[nextuir64le][0] == 0 && uint64lerange[nextuir64le][1] == UINT64_MAX) anybyte = true;
			nextuir64le++;
		}
	}
	if (size == 8 && be) {
		if (unsign && neg) {
			printf("error: line %d - invalid range end\n", linenum);
			exit(EXIT_FAILURE);
		}
		if (!unsign) {
			decimalsig = strtoll(decimalstr[1], &endptr, 10);
			if (*endptr != '\0' || (decimalsig < INT8_MIN || decimalsig > INT8_MAX) || decimalsig < int8berange[nextir8be][0]) {
				printf("error: line %d - invalid range end\n", linenum);
				exit(EXIT_FAILURE);
			}
			int8berange[nextir8be][1] = decimalsig;
			fprintf(xout, "ir8be__%d", nextir8be);
			if (int8berange[nextir8be][0] == INT8_MIN && int8berange[nextir8be][1] == INT8_MAX) anybyte = true;
			nextir8be++;
		}
		if (unsign) {
			decimalunsig = strtoull(decimalstr[1], &endptr, 10);
			if(*endptr != '\0'|| (decimalunsig < 0 || decimalunsig > UINT8_MAX) || decimalunsig < uint8berange[nextuir8be][0]) {
				printf("error: line %d - invalid range end\n", linenum);
				exit(EXIT_FAILURE);
			}
			uint8berange[nextuir8be][1] = decimalunsig;
			fprintf(xout, "uir8be__%d", nextuir8be);
			if (uint8berange[nextuir8be][0] == 0 && uint8berange[nextuir8be][1] == UINT8_MAX) anybyte = true;
			nextuir8be++;
		}
	}
	if (size == 16 && be) {
		if (unsign && neg) {
			printf("error: line %d - invalid range end\n", linenum);
			exit(EXIT_FAILURE);
		}
		if (!unsign) {
			decimalsig = strtoll(decimalstr[1], &endptr, 10);
			if (*endptr != '\0' || (decimalsig < INT16_MIN || decimalsig > INT16_MAX) || decimalsig < int16berange[nextir16be][0]) {
				printf("error: line %d - invalid range end\n", linenum);
				exit(EXIT_FAILURE);
			}
			int16berange[nextir16be][1] = decimalsig;
			fprintf(xout, "ir16be__%d", nextir16be);
			if (int16berange[nextir16be][0] == INT16_MIN && int16berange[nextir16be][1] == INT16_MAX) anybyte = true;
			nextir16be++;
		}
		if (unsign) {
			decimalunsig = strtoull(decimalstr[1], &endptr, 10);
			if(*endptr != '\0'|| (decimalunsig < 0 || decimalunsig > UINT16_MAX) || decimalunsig < uint16berange[nextuir16be][0]) {   
				printf("error: line %d - invalid range end\n", linenum);
				exit(EXIT_FAILURE);
			}
			uint16berange[nextuir16be][1] = decimalunsig;
			fprintf(xout, "uir16be__%d", nextuir16be);
			if (uint16berange[nextuir16be][0] == 0 && uint16berange[nextuir16be][1] == UINT16_MAX) anybyte = true;
			nextuir16be++;
		}
	}
	if (size == 32 && be) {
		if (unsign && neg) {
			printf("error: line %d - invalid range end\n", linenum);
			exit(EXIT_FAILURE);
		}
		if (!unsign) {
			decimalsig = strtoll(decimalstr[1], &endptr, 10);
			if (*endptr != '\0' || (decimalsig < INT32_MIN || decimalsig > INT32_MAX) || decimalsig < int32berange[nextir32be][0]) {
				printf("error: line %d - invalid range end\n", linenum);
				exit(EXIT_FAILURE);
			}
			int32berange[nextir32be][1] = decimalsig;
			fprintf(xout, "ir32be__%d", nextir32be);
			if (int32berange[nextir32be][0] == INT32_MIN && int32berange[nextir32be][1] == INT32_MAX) anybyte = true;
			nextir32be++;
		}
		if (unsign) {
			decimalunsig = strtoull(decimalstr[1], &endptr, 10);
			if(*endptr != '\0'|| (decimalunsig < 0 || decimalunsig > UINT32_MAX) || decimalunsig < uint32berange[nextuir32be][0]) {
				printf("error: line %d - invalid range end\n", linenum);
				exit(EXIT_FAILURE);
			}
			uint32berange[nextuir32be][1] = decimalunsig;
			fprintf(xout, "uir32be__%d", nextuir32be);
			if (uint32berange[nextuir32be][0] == 0 && uint32berange[nextuir32be][1] == UINT32_MAX) anybyte = true;
			nextuir32be++;
		}
	}
	if (size == 64 && be) {
		if (unsign && neg) {
			printf("error: line %d - invalid range end\n", linenum);
			exit(EXIT_FAILURE);
		}
		if (!unsign) {
			errno = 0;
			decimalsig = strtoll(decimalstr[1], &endptr, 10);
			if (*endptr != '\0' || errno == ERANGE || decimalsig < int64berange[nextir64be][0]) {
				printf("error: line %d - invalid range end\n", linenum);
				exit(EXIT_FAILURE);
			}
			int64berange[nextir64be][1] = decimalsig;
			fprintf(xout, "ir64be__%d", nextir64be);
			if (int64berange[nextir64be][0] == INT64_MIN && int64berange[nextir64be][1] == INT64_MAX) anybyte = true;
			nextir64be++;
		}
		if (unsign) {
			decimalunsig = strtoull(decimalstr[1], &endptr, 10);
			if(*endptr != '\0'|| errno == ERANGE || decimalunsig < uint64berange[nextuir64be][0]) {
				printf("error: line %d - invalid range end\n", linenum);
				exit(EXIT_FAILURE);
			}
			uint64berange[nextuir64be][1] = decimalunsig;
			fprintf(xout, "uir64be__%d", nextuir64be);
			if (uint64berange[nextuir64be][0] == 0 && uint64berange[nextuir64be][1] == UINT64_MAX) anybyte = true;
			nextuir64be++;
		}
	}
	
	decimalsig = 0;
	decimalunsig = 0;
	fixedwidth = false;
	nnum = 0;
	le = false;
	be = false;
	lowle = false;
	lowbe = false;
}

/* writes integer in hex */
static void write_int(uint8_t *bytes) {
	int i;
	if (le) {
		for (i = 0; i < size/8; i++) {
				if (bytes[i] == 0) fprintf(xout, "X00 ");
				else {
					fprintf(xout, "\'\\x%02x\'", bytes[i]);
				  if (i != size/8 - 1) fprintf(xout, " ");
				}
		}
	}
	else if (be) {
		for (i = size/8 - 1; i >= 0; i--) {
			if (bytes[i] == 0) fprintf(xout, "X00 ");
			else {
				fprintf(xout, "\'\\x%02x\'", bytes[i]);
				if (i != 0) fprintf(xout, " ");
			}
		}
	}
}

/* converts signed integer to array of hex values */
static void convert_int(uint8_t bytes_arr[], int64_t dec, int size) {
	int i;
	for (i = 0; i < size; i++) {
		bytes_arr[i] = dec & 0xff;
		dec >>= 8;
	}
}

/* converts unsigned integer to array of hex values */
static void convert_uint(uint8_t bytes_arr[], uint64_t dec, int size) {
	int i;
	for (i = 0; i < size; i++) {
		bytes_arr[i] = dec & 0xff;
		dec >>= 8;
	}
}

/* initialise all variables */
void init(void) {
	int i, j;
	linenum = 1;
	range[0][RMIN] = 0;					          /* r__0 if used is anybyte */
	range[0][RMAX] = 255;
	nextrange++;								          /* first range is 1 */
	anybyte = false;							        /* anybyte not yet detected */
	ranging = false;
	for (i = 1; i < MAXENTRY; i++) {	    /* ranges */
		range[i][RMIN] = -1;
		range[i][RMAX] = -1 ;
	}
	for(i = 0; i < MAXENTRY; i++) {	      /* enumerations */
		for(j = 0; j < ENUMMAX; j++) 
			enumeration[i][j] = -1;
	}
	for(i = 0; i < MAXENTRY; i++) {       /* strings */
		for(j = 0; j < CHARMAX; j++)
			strings[i][j] = -1;
	}
	
	/* generate the preamble */
	fprintf(xout,"%%{\n");
	fprintf(xout,"  #define YYDEBUG 1\n");
	fprintf(xout,"  int yylex(void);\n");
	fprintf(xout,"  void yyerror(char *s);\n");
	fprintf(xout,"%%}\n");
	fprintf(xout,"%%token X00\n");
	fprintf(xout,"%%%%\n");
}

/* begins a new range */
void rbegin(void) {
	rlow = -1;
	rhigh = -1;
	cval = -1;
	hval = -1;
	eindex = 0;
	ranging = true;
}

/* set low bound on range */
void setrlow(void) {
	if (!fixedwidth) {
		if (hval < 0 && cval < 0) {
			printf("error: line %d - invalid range start\n", linenum);
			exit(EXIT_FAILURE);
		}
		if (hval >= 0 && !fixedwidth) rlow=hval;
		else rlow = cval;
		
		hval = -1;										/* reset for next value */
		cval = -1;
	}
	else setfwrlow();
}

/* set high bound on range */
void setrhigh(void) {
	if (!fixedwidth) {
		if (hval < 0 && cval < 0) {
			printf("error: line %d - invalid range end\n", linenum);
			exit(EXIT_FAILURE);
		}
		if(hval >= 0)
			rhigh = hval;
		else
			rhigh = cval;
		
		hval = -1;										/* reset for next value */
		cval = -1;
		
		if (rhigh <= rlow) {          /* legal range */
			printf("error: line %d - invalid range [%d-%d] -- low to high required\n", linenum, rlow, rhigh);
			exit(EXIT_FAILURE);
		}
		
		if (rlow == 0 && rhigh == 255) {				/* same as anybyte */
			fprintf(xout, "r__0");								/* r__0 is anybyte */
			anybyte = true;												/* at least one use */
		}
		else {
			range[nextrange][RMIN] = rlow;				/* store it */
			range[nextrange][RMAX] = rhigh;
			fprintf(xout, "r__%d", nextrange);    /* generate a symbol */
			nextrange++;
		}
	}
	else setfwrhigh();
	ranging = false;
}

/* set first enueration */
void setenum0(void) {
	if ((hval < 0 && cval < 0) || fixedwidth) {
		printf("error: line %d - invalid enumeration\n", linenum);
		exit(EXIT_FAILURE);
	}
	
	if (hval >= 0)
		enumeration[nextenum][0] = hval;
	else
		enumeration[nextenum][0] = cval;
	
	hval = -1;
	cval = -1;
	eindex = 1;
}

/* set second through n numbers in enumeration */
void setnextenum(void) {
	if ((hval < 0 && cval < 0) || fixedwidth) {
		printf("error: line %d - invalid enumeration\n", linenum);
		exit(EXIT_FAILURE);
	}
	
	if (hval >= 0)
		enumeration[nextenum][eindex]=hval;
	else
		enumeration[nextenum][eindex]=cval;
	
	hval = -1;
	cval = -1;
	eindex++;																 
}

/* ends enumeration */
void eend(void) {
	if (eindex == 1) {
		printf("error: line %d - single entry enumeration invalid\n", linenum);
		exit(EXIT_FAILURE);
	}
	fprintf(xout, "e__%d", nextenum);
	nextenum++; 
	eindex = 0; 
	
	ranging = false;
}

/* decodes a fixed-width integer into hex */
void fixed_width(void){
	char *endptr;
	if (!ranging) {
		if (size == 8) {
			if (unsign && neg) {
				printf("error: line %d - invalid entry for uint8\n", linenum);
				exit(EXIT_FAILURE);
			}
			uint8_t bytes[1] = {0};
			if (!unsign) {
				decimalsig = strtoll(decimalstr[0], &endptr, 10);
				if (*endptr != '\0' || (decimalsig < INT8_MIN || decimalsig > INT8_MAX) ) {
					printf("error: line %d - invalid entry for int8\n", linenum);
					exit(EXIT_FAILURE);
				}
				convert_int(bytes, decimalsig, 1);
			}
			if (unsign) {
				decimalunsig = strtoull(decimalstr[0], &endptr, 10);
				if(*endptr != '\0'|| (decimalunsig < 0 || decimalunsig > UINT8_MAX)){   
					printf("error: line %d - invalid entry for uint8\n", linenum);
					exit(EXIT_FAILURE);          
				}
				convert_uint(bytes, decimalunsig, 1);
			}
			write_int(bytes);
		}
		if (size == 16) {
			if (unsign && neg) {
				printf("error: line %d - invalid entry for uint16\n", linenum);    
				exit(EXIT_FAILURE);
			}
			uint8_t bytes[2] = {0, 0};  
			if (!unsign) {
				decimalsig = strtoll(decimalstr[0], &endptr, 10);
				if (*endptr != '\0' || (decimalsig < INT16_MIN || decimalsig > INT16_MAX) ) {
					printf("error: line %d - invalid entry for int16\n", linenum);
					exit(EXIT_FAILURE);
				}
				convert_int(bytes, decimalsig, 2);
			}
			if (unsign) {
				decimalunsig = strtoull(decimalstr[0], &endptr, 10);
				if(*endptr != '\0'|| (decimalunsig < 0 || decimalunsig > UINT16_MAX) ){   
					printf("error: line %d - invalid entry for uint16\n", linenum);
					exit(EXIT_FAILURE);          
				}
				convert_uint(bytes, decimalunsig, 2);
			}
			write_int(bytes);
		}
		if (size == 32) {
			if (unsign && neg) {
				printf("error: line %d - invalid entry for uint32\n", linenum);
				exit(EXIT_FAILURE);
			}
			uint8_t bytes[4] = {0, 0, 0, 0};
			if (!unsign) {
				decimalsig = strtoll(decimalstr[0], &endptr, 10);
				if (*endptr != '\0' || (decimalsig < INT32_MIN || decimalsig > INT32_MAX) ){  
					printf("error: line %d - invalid entry for int32\n", linenum);          
					exit(EXIT_FAILURE);
				}
				convert_int(bytes, decimalsig, 4);
			}
			if (unsign) {
				decimalunsig = strtoull(decimalstr[0], &endptr, 10);
				if (*endptr != '\0' || (decimalunsig < 0 || decimalunsig > UINT32_MAX) ){   
					printf("error: line %d - invalid entry for uint32\n", linenum);
					exit(EXIT_FAILURE);
				}
				convert_uint(bytes, decimalunsig, 4);
			}
			write_int(bytes);
		}
		if (size == 64) {
			if (unsign && neg) {
				printf("error: line %d - invalid entry for uint32\n", linenum);
				exit(EXIT_FAILURE);
			}
			uint8_t bytes[8] = {0, 0, 0, 0, 0, 0, 0, 0};
			if (!unsign) {
				errno = 0;
				decimalsig = strtoll(decimalstr[0], &endptr, 10);
				if (*endptr != '\0' || errno == ERANGE){  
					printf("error: line %d - invalid entry for int64\n", linenum);
					exit(EXIT_FAILURE);           
				}
				convert_int(bytes, decimalsig, 8);
			}
			if (unsign) {
				errno = 0;
				decimalunsig = strtoull(decimalstr[0], &endptr, 10);
				if (*endptr != '\0' || errno == ERANGE){   
					printf("error: line %d -invalid entry for uint64\n", linenum);
					exit(EXIT_FAILURE);
				}
				convert_uint(bytes, decimalunsig, 8);
			}
			write_int(bytes);
		}
		le = false;
		be = false;
		fixedwidth = false;
		nnum=0;
		decimalsig=0;
		decimalunsig=0;
	}
}

/* checks to make sure fixed-width integer input is not too long */
void check_index(void) {
	if (i > 99) {
		printf("error: line %d - invalid number entry\n", linenum);
		exit(EXIT_FAILURE);
	}
}

/* prints out hex values */
void hexout(void) {
	if (!ranging) {
		if (hval == 0)
			fprintf(xout, "X00");
		else
			fprintf(xout, "\'\\x%02x\'", hval);
	}
}

/* adds rules for ranges, strings and enumerations */
void addrules(void) {
	int i ,j, k;
	int64_t l;
	uint64_t m;
	
	if (anybyte) i = 0;					/* anybyte present, start at 0 */
	else i = 1;									/* otherwise, dont generate range 0 */
	
	if (anybyte || nextrange > 1)
		fprintf(xout, "\n/* Range Expansions */\n");
	for ( ; i < nextrange; i++) {
		fprintf(xout, "r__%d : ", i);
		for (k=0, j = range[i][RMIN]; j < range[i][RMAX]; k++, j++) {
			if (k%8 == 0)
				fprintf(xout, "\n  ");
			if (j == 0)
				fprintf(xout, "X00 | ");
			else
				fprintf(xout, "\'\\x%02x\' | " , (uint8_t)j);
		}
		if (k%8 == 0)
			fprintf(xout, "\n  ");
		fprintf(xout, "\'\\x%02x\' ;\n", (uint8_t)j);
	}
	
	if (nextir16le > 0)
		fprintf(xout, "\n/* int16 Little Endian Range Expansions */\n");
	for (i = 0 ; i < nextir16le; i++) {
		uint8_t bytes[2] = {0, 0};
		size = 16;
		fprintf(xout, "ir16le__%d : ", i);
		if (int16lerange[i][0] == INT16_MIN && int16lerange[i][1] == INT16_MAX)
			fprintf(xout, "r__0 r__0 ;\n");
		else {
			for (k=0, l = int16lerange[i][RMIN]; l < int16lerange[i][RMAX]; k++, l++) {
				le = true;
				if (k%8 == 0)
					fprintf(xout, "\n  ");
        convert_int(bytes, l, 2);
        write_int(bytes);
				fprintf(xout, " | ");
			}
			if (k%8 == 0)
				fprintf(xout, "\n  ");
			convert_int(bytes, l, 2);
			write_int(bytes);
			fprintf(xout, ";\n");
		}
		le = false;
	}

	if (nextuir16le > 0)
		fprintf(xout, "\n/* uint16 Little Endian Range Expansions */\n");
	for (i = 0 ; i < nextuir16le; i++) {
		uint8_t bytes[2] = {0, 0};
		size = 16;
		fprintf(xout, "uir16le__%d : ", i);
		if (uint16lerange[i][0] == 0 && uint16lerange[i][1] == UINT16_MAX)
			fprintf(xout, "r__0 r__0 ;\n");
		else {
			for (k=0, m = uint16lerange[i][RMIN]; m < uint16lerange[i][RMAX]; k++, m++) {
				le = true;
				if (k%8 == 0)
					fprintf(xout, "\n  ");
				convert_uint(bytes, m, 2);
				write_int(bytes);
				fprintf(xout, " | ");
			}
			if (k%8 == 0)
				fprintf(xout, "\n  ");
			convert_uint(bytes, m, 2);
			write_int(bytes);
			fprintf(xout, ";\n");
		}
		le = false;
	}
	
	if (nextir32le > 0)
		fprintf(xout, "\n/* int32 Little Endian Range Expansions */\n");
	for (i = 0 ; i < nextir32le; i++) {
		uint8_t bytes[4] = {0, 0, 0, 0};
		size = 32;
		fprintf(xout, "ir32le__%d : ", i);
		if (int32lerange[i][0] == INT32_MIN && int32lerange[i][1] == INT32_MAX)
			fprintf(xout, "r__0 r__0 r__0 r__0 ;\n");
		else {
			for (k=0, l = int32lerange[i][RMIN]; l < int32lerange[i][RMAX]; k++, l++) {
				le = true;
				if (k%8 == 0)
					fprintf(xout, "\n  ");
				convert_int(bytes, l, 4);
				write_int(bytes);
				fprintf(xout, " | ");
			}
			if (k%8 == 0)
				fprintf(xout, "\n  ");
			convert_int(bytes, l, 4);
			write_int(bytes);
			fprintf(xout, ";\n");
		}
		le = false;
	}
	
	if (nextuir32le > 0)
			fprintf(xout, "\n/* uint32 Little Endian Range Expansions */\n");
	for (i = 0 ; i < nextuir32le; i++) {
		uint8_t bytes[4] = {0, 0, 0, 0};
		size = 32;
		fprintf(xout, "uir32le__%d : ", i);
		if (uint32lerange[i][0] == 0 && uint32lerange[i][1] == UINT32_MAX)
			fprintf(xout, "r__0 r__0 r__0 r__0 ;\n");
		else {
			for (k=0, m = uint32lerange[i][RMIN]; m < uint32lerange[i][RMAX]; k++, m++) {
				le = true;
				if (k%8 == 0)
					fprintf(xout, "\n  ");
				convert_uint(bytes, m, 4);
				write_int(bytes);
				fprintf(xout, " | ");
			}
			if (k%8 == 0)
				fprintf(xout, "\n  ");
			convert_uint(bytes, m, 4);
			write_int(bytes);
			fprintf(xout, ";\n");
		}
		le = false;
	}
	
	if (nextir64le > 0)
		fprintf(xout, "\n/* int64 Little Endian Range Expansions */\n");
	for (i = 0 ; i < nextir64le; i++) {	
		uint8_t bytes[8] = {0, 0, 0, 0, 0, 0, 0, 0};
		size = 64;
		fprintf(xout, "ir64le__%d : ", i);
		if (int64lerange[i][0] == INT64_MIN && int64lerange[i][1] == INT64_MAX)
			fprintf(xout, "r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;\n");
		else {
			for (k=0, l = int64lerange[i][RMIN]; l < int64lerange[i][RMAX]; k++, l++) {
				le = true;
				if (k%8 == 0)
					fprintf(xout, "\n  ");
				convert_int(bytes, l, 8);
				write_int(bytes);
				fprintf(xout, " | ");
			}
			if (k%8 == 0)
				fprintf(xout, "\n  ");
			convert_int(bytes, l, 8);
			write_int(bytes);
			fprintf(xout, ";\n");
		}
		le = false;
	}
	
	if (nextuir64le > 0)
		fprintf(xout, "\n/* uint64 Little Endian Range Expansions */\n");
	for (i = 0 ; i < nextuir64le; i++) {
		uint8_t bytes[8] = {0, 0, 0, 0, 0, 0, 0, 0};
		size = 64;
		fprintf(xout, "uir64le__%d : ", i);
		if (uint64lerange[i][0] == 0 && uint64lerange[i][1] == UINT64_MAX)
			fprintf(xout, "r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;\n");
		else {
			for (k=0, m = uint64lerange[i][RMIN]; m < uint64lerange[i][RMAX]; k++, m++) {
				le = true;
				if (k%8 == 0)
					fprintf(xout, "\n  ");
				convert_uint(bytes, m, 8);
				write_int(bytes);
				fprintf(xout, " | ");
			}
			if (k%8 == 0)
				fprintf(xout, "\n  ");
			convert_uint(bytes, m, 8);
			write_int(bytes);
			fprintf(xout, ";\n");
		}
		le = false;
	}
	
	if (nextir8be > 0)
		fprintf(xout, "\n/* int8 Big Endian Range Expansions */\n");
	for (i = 0 ; i < nextir8be; i++) {	
		uint8_t bytes[1] = {0};
		size = 8;
		fprintf(xout, "ir8be__%d : ", i);
		if (int8berange[i][0] == INT8_MIN && int8berange[i][1] == INT8_MAX)
			fprintf(xout, "r__0 ;\n");
		else {
			for (k=0, l = int8berange[i][RMIN]; l < int8berange[i][RMAX]; k++, l++) {
				be = true;
				if (k%8 == 0)
					fprintf(xout, "\n  ");
				convert_int(bytes, l, 1);
				write_int(bytes);
				fprintf(xout, " | ");
			}
			if (k%8 == 0)
				fprintf(xout, "\n  ");
			convert_int(bytes, l, 1);
			write_int(bytes);
			fprintf(xout, ";\n");
		}
		be = false;
	}
	
	if (nextuir8be > 0)
		fprintf(xout, "\n/* uint8 Big Endian Range Expansions */\n");
	for (i = 0 ; i < nextuir8be; i++) {
		uint8_t bytes[1] = {0};
		size = 8;
		fprintf(xout, "uir8be__%d : ", i);
		if (uint8berange[i][0] == 0 && uint8berange[i][1] == UINT8_MAX)
			fprintf(xout, "r__0 ;\n");
		else {
			for (k=0, m = uint8berange[i][RMIN]; m < uint8berange[i][RMAX]; k++, m++) {
				be = true;
				if (k%8 == 0)
					fprintf(xout, "\n  ");
				convert_uint(bytes, m, 1);
				write_int(bytes);
				fprintf(xout, " | ");
			}
			if (k%8 == 0)
				fprintf(xout, "\n  ");
			convert_uint(bytes, m, 1);
			write_int(bytes);
			fprintf(xout, ";\n");
		}
		be = false;
	}
	
	if (nextir16be > 0)
		fprintf(xout, "\n/* int16 Big Endian Range Expansions */\n");
	for (i = 0 ; i < nextir16be; i++) {	
		uint8_t bytes[2] = {0, 0};
		size = 16;
		fprintf(xout, "ir16be__%d : ", i);
		if (int16berange[i][0] == INT16_MIN && int16berange[i][1] == INT16_MAX)
			fprintf(xout, "r__0 r__0 ;\n");
		else {
			for (k=0, l = int16berange[i][RMIN]; l < int16berange[i][RMAX]; k++, l++) {
				be = true;
				if (k%8 == 0)
					fprintf(xout, "\n  ");
				convert_int(bytes, l, 2);
				write_int(bytes);
				fprintf(xout, " | ");
			}
			if (k%8 == 0)
				fprintf(xout, "\n  ");
			convert_int(bytes, l, 2);
			write_int(bytes);
			fprintf(xout, ";\n");
		}
		be = false;
	}
	
	if (nextuir16be > 0)
		fprintf(xout, "\n/* uint16 Big Endian Range Expansions */\n");
	for (i = 0 ; i < nextuir16be; i++) {
		uint8_t bytes[2] = {0, 0};
		size = 16;
		fprintf(xout, "uir16be__%d : ", i);
		if (uint16berange[i][0] == 0 && uint16berange[i][1] == UINT16_MAX)
			fprintf(xout, "r__0 r__0 ;\n");
		else {
			for (k=0, m = uint16berange[i][RMIN]; m < uint16berange[i][RMAX]; k++, m++) {
				be = true;
				if (k%8 == 0)
					fprintf(xout, "\n  ");
				convert_uint(bytes, m, 2);
				write_int(bytes);
				fprintf(xout, " | ");
			}
			if (k%8 == 0)
				fprintf(xout, "\n  ");
			convert_uint(bytes, m, 2);
			write_int(bytes);
			fprintf(xout, ";\n");
		}
		be = false;
	}
	
	if (nextir32be > 0)
		fprintf(xout, "\n/* int32 Big Endian Range Expansions */\n");
	for (i = 0 ; i < nextir32be; i++) {	
		uint8_t bytes[4] = {0, 0, 0, 0};
		size = 32;
		fprintf(xout, "ir32be__%d : ", i);
		if (int32berange[i][0] == INT32_MIN && int32berange[i][1] == INT32_MAX)
			fprintf(xout, "r__0 r__0 r__0 r__0 ;\n");
		else {
			for (k=0, l = int32berange[i][RMIN]; l < int32berange[i][RMAX]; k++, l++) {
				be = true;
				if (k%8 == 0)
					fprintf(xout, "\n  ");
				convert_int(bytes, l, 4);
				write_int(bytes);
				fprintf(xout, " | ");
			}
			if (k%8 == 0)
				fprintf(xout, "\n  ");
			convert_int(bytes, l, 4);
			write_int(bytes);
			fprintf(xout, ";\n");
		}
		be = false;
	}
	
	if (nextuir32be > 0)
		fprintf(xout, "\n/* uint32 Big Endian Range Expansions */\n");
	for (i = 0 ; i < nextuir32be; i++) {
		uint8_t bytes[4] = {0, 0, 0, 0};
		size = 32;
		fprintf(xout, "uir32be__%d : ", i);
		if (uint32berange[i][0] == 0 && uint32berange[i][1] == UINT32_MAX)
			fprintf(xout, "r__0 r__0 r__0 r__0 ;\n");
		else {
			for (k=0, m = uint32berange[i][RMIN]; m < uint32berange[i][RMAX]; k++, m++) {
				be = true;
				if (k%8 == 0)
					fprintf(xout, "\n  ");
				convert_uint(bytes, m, 4);
				write_int(bytes);
				fprintf(xout, " | ");
			}
			if (k%8 == 0)
				fprintf(xout, "\n  ");
			convert_uint(bytes, m, 4);
			write_int(bytes);
			fprintf(xout, ";\n");
		}
		be = false;
	}
	
	if (nextir64be > 0)
		fprintf(xout, "\n/* int64 Big Endian Range Expansions */\n");
	for (i = 0 ; i < nextir64be; i++) {	
		uint8_t bytes[8] = {0, 0, 0, 0, 0, 0, 0, 0};
		size = 64;
		fprintf(xout, "ir64be__%d : ", i);
		if (int64berange[i][0] == INT64_MIN && int64berange[i][1] == INT64_MAX)
			fprintf(xout, "r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;\n");
		else {
			for (k=0, l = int64berange[i][RMIN]; l < int64berange[i][RMAX]; k++, l++) {
				be = true;
				if (k%8 == 0)
					fprintf(xout, "\n  ");
				convert_int(bytes, l, 8);
				write_int(bytes);
				fprintf(xout, " | ");
			}
			if (k%8 == 0)
				fprintf(xout, "\n  ");
			convert_int(bytes, l, 8);
			write_int(bytes);
			fprintf(xout, ";\n");
		}
		be = false;
	}
	
	if (nextuir64be > 0)
		fprintf(xout, "\n/* uint64 Big Endian Range Expansions */\n");
	for (i = 0 ; i < nextuir64be; i++) {
		uint8_t bytes[8] = {0, 0, 0, 0, 0, 0, 0, 0};
		size = 64;
		fprintf(xout, "uir64be__%d : ", i);
		if (uint64berange[i][0] == 0 && uint64berange[i][1] == UINT64_MAX)
			fprintf(xout, "r__0 r__0 r__0 r__0 r__0 r__0 r__0 r__0 ;\n");
		else {
			for (k=0, m = uint64berange[i][RMIN]; m < uint64berange[i][RMAX]; k++, m++) {
				be = true;
				if (k%8 == 0)
					fprintf(xout, "\n  ");
				convert_uint(bytes, m, 8);
				write_int(bytes);
				fprintf(xout, " | ");
			}
			if (k%8 == 0)
				fprintf(xout, "\n  ");
			convert_uint(bytes, m, 8);
			write_int(bytes);
			fprintf(xout, ";\n");
		}
		be = false;
	}
	
	if (nextenum > 0)
		fprintf(xout, "\n/* Enumeration Expansions */\n");
	for (i = 0; i < nextenum; i++) {		             /* for each enumeration */
		fprintf(xout, "e__%d : ", i);
		j = 0;
		k = 0;
		while (enumeration[i][j+1] >= 0) {	           /* next element present */
			if (k%8 == 0)
				fprintf(xout, "\n  ");
			if (enumeration[i][j] == 0)
				fprintf(xout, "X00 | ");
			else
				fprintf(xout, "\'\\x%02x\' | ", enumeration[i][j]);
			j++;
			k++;
		}
		if (k%8 == 0) 
			fprintf(xout, "\n  ");
		if(enumeration[i][j]==0)
			fprintf(xout, "X00 ;\n");
		else
			fprintf(xout, "\'\\x%02x\' ;\n", enumeration[i][j]);
	}
	
	if (nstr > 0)
		fprintf(xout, "\n/* String Expansions */\n");
	for (i = 0; i < nstr; i++) {
		fprintf(xout, "s__%d : ", i);
		j = 0;
		k = 0;
		while (strings[i][j+1] >= 0){
			if (k%8 == 0)
				fprintf(xout, "\n ");
			if(strings[i][j] == 0)
				fprintf(xout, "X00 ");
			if (strings[i][j] == '\\')
				fprintf(xout, "'\\\\'");
			else                         
				fprintf(xout, "\'%c\' ", strings[i][j]);   
			j++;
			k++;
		}
		if (k%8 == 0) 
			fprintf(xout, "\n  ");    
		if (strings[i][j] == 0)                                                                               
			fprintf(xout, "X00 ;\n");
		else
			fprintf(xout, "\'%c\' ;\n", strings[i][j]);
	}
}
