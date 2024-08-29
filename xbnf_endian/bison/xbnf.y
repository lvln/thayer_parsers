%{

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <errno.h>
	
#define YYDEBUG 1
#define MAXNUM 100              /* max number of digits in number */
#define MAXENTRY 100						/* max number of separate ranges */
#define RMIN 0									/* index of min range value */
#define RMAX 1									/* index of max range value */
#define ENUMMAX 10							/* max number of entries in an enumeration */
#define CHARMAX 50              /* max number of characters in a string */

	int yylex(void);
	void yyerror(char *s);

	extern FILE* xout;
	extern int yylval;

	static int linenum = 1;
	static int hval;						                    /* variable used to form a hex value from ascii chars */
	static int cval;                                /* variable used to hold a character value */
	static int nextrange = 0;				                /* number of ranges */
  static int nextir16le = 0;                      /* number of 2 byte signed little endian ranges */
  static int nextir32le = 0;                      /* number of 4 byte signed little endian ranges */
  static int nextir64le = 0;                      /* number of 8 byte signed little endian ranges */
  static int nextir16be = 0;                      /* number of 2 byte signed big endian ranges */
  static int nextir32be = 0;                      /* number of 4 byte signed big endian ranges */
  static int nextir64be = 0;                      /* number of 8 byte signed big endian ranges */
  static int nextuir16le = 0;                     /* number of 2 byte unsigned little endian ranges */
  static int nextuir32le = 0;                     /* number of 4 byte unsigned little endian ranges */
  static int nextuir64le = 0;                     /* number of 8 byte unsigned little endian ranges */
  static int nextuir16be = 0;                     /* number of 2 byte unsigned big endian ranges */
  static int nextuir32be = 0;                     /* number of 4 byte unsigned big endian ranges */
  static int nextuir64be = 0;                     /* number of 8 byte unsigned big endian ranges */
	static int nextenum = 0;				                /* number of enumerations */
	static int eindex = 0;                          /* index into an emumeration */
	static int rlow;							                  /* low value in a range */
	static int rhigh;							                  /* high value in a range */
	static int range[MAXENTRY][2];			            /* the ranges */
  static int64_t int16lerange[MAXENTRY][2];			  /* the ranges for signed 2 byte little endian */
  static int64_t int32lerange[MAXENTRY][2];			  /* the ranges for signed 4 byte little endian */
  static int64_t int64lerange[MAXENTRY][2];			  /* the ranges for signed 8 byte little endian */
  static int64_t int16berange[MAXENTRY][2];			  /* the ranges for signed 2 byte big endian */
  static int64_t int32berange[MAXENTRY][2];			  /* the ranges for signed 4 byte big endian */
  static int64_t int64berange[MAXENTRY][2];			  /* the ranges for signed 8 byte big endian */
  static uint64_t uint16lerange[MAXENTRY][2];			/* the ranges for unsigned 2 byte little endian */
  static uint64_t uint32lerange[MAXENTRY][2];			/* the ranges for unsigned 4 byte little endian */
  static uint64_t uint64lerange[MAXENTRY][2];			/* the ranges for unsigned 8 byte little endian */
  static uint64_t uint16berange[MAXENTRY][2];			/* the ranges for unsigned 2 byte big endian */
  static uint64_t uint32berange[MAXENTRY][2];			/* the ranges for unsigned 4 byte big endian */
  static uint64_t uint64berange[MAXENTRY][2];			/* the ranges for unsigned 8 byte big endian */
	static int enumeration[MAXENTRY][ENUMMAX];      /* the enumerations */
	static bool anybyte = false;		                /* true if anybytes is used in the grammar */
	static bool ranging = false;		                /* true if insize a range */
	static int strings[MAXENTRY][CHARMAX];          /* any strings that are found within rules */
	static int c = 0;                               /* variable used to hold character value for a string */
	static int nstr = 0;                            /* number of strings */

  static char decimalstr[2][MAXNUM];              /* string to read number into */
  static int nnum = 0;                            /* how many numbers have been read in as strings */
  static int i = 0;                               /* index variable for number string */
  static int64_t decimalsig = 0;
	static uint64_t decimalunsig = 0;     
	static int size = 16;
	static bool neg = false;
	static bool unsign = true; /*default variable for fwi, intially unsigned*/
	static bool fixedW = false;
  static bool le = false;
  static bool be = false;

	static FILE* counts;
	static int Xrules = 0;
	static int Xterminals = 0;
	static int Xnonterminals = 0;

	static int Brules = 0;
	static int Bterminals = 0;
  static int Bnonterminals = 0;

	/* clear the tables */
	static void init(void) {
		int i,j;
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
    for (i = 1; i < MAXENTRY; i++) {	    /* 2 byte little endian ranges */
			int16lerange[i][RMIN] = -1;
			int16lerange[i][RMAX] = -1 ;
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

  /* clear the range indexes */
	static void rbegin(void) {
		rlow = -1;
		rhigh = -1;
		cval = -1;
		hval = -1;
		eindex = 0; 								/* first enumeration index */
		ranging=true;
	}

	static void setrlow(void) {
		if (hval < 0 && cval < 0) {
			printf("error: line %d - invalid range start\n", linenum);
			exit(EXIT_FAILURE);
		}
		if (hval >= 0) rlow=hval;
  		else rlow = cval;
		
		hval = -1;										/* reset for next value */
		cval = -1;
	}
	
	static void setrhigh(void) {
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
		ranging = false;
	}


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
			 	  printf("error: invalid range start\n");
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
			 	  printf("error: invalid range start\n");
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
			 	  printf("error: invalid range start\n");
		 		  exit(EXIT_FAILURE);          
	 		  }
        uint64lerange[nextuir64le][0] = decimalunsig;
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
			 	  printf("error: invalid range start\n");
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
			 	  printf("error: invalid range start\n");
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
			 	  printf("error: invalid range start\n");
		 		  exit(EXIT_FAILURE);          
	 		  }
        uint64berange[nextuir64be][0] = decimalunsig;
      }
		}

    decimalsig = 0;
    decimalunsig = 0;
	}
	
	static void setfwrhigh(void) {
    char *endptr;
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
		    nextir16le++;
      }
      if (unsign) {
					decimalunsig = strtoull(decimalstr[1], &endptr, 10);
 			  if(*endptr != '\0'|| (decimalunsig < 0 || decimalunsig > UINT16_MAX) || decimalsig < uint16lerange[nextuir32le][0]) {   
					printf("error: line %d - invalid range end\n", linenum);
		 		  exit(EXIT_FAILURE);
	 		  }
        uint16lerange[nextuir16le][1] = decimalunsig;
		    fprintf(xout, "uir16le__%d", nextuir16le);
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
		    nextir32le++;
      }
      if (unsign) {
				decimalunsig = strtoull(decimalstr[1], &endptr, 10);
 			  if(*endptr != '\0'|| (decimalunsig < 0 || decimalunsig > UINT16_MAX) || decimalsig < uint32lerange[nextuir32le][0]){   
					printf("error: line %d - invalid range end\n", linenum);
		 		  exit(EXIT_FAILURE);
	 		  }
        uint32lerange[nextuir32le][1] = decimalunsig;
		    fprintf(xout, "uir32le__%d", nextuir32le);
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
		    nextir64le++;
      }
      if (unsign) {
				decimalunsig = strtoull(decimalstr[1], &endptr, 10);
 			  if(*endptr != '\0'|| errno == ERANGE || decimalsig < uint64lerange[nextuir64le][0]) {
					printf("error: line %d - invalid range end\n", linenum);
		 		  exit(EXIT_FAILURE);
	 		  }
        uint64lerange[nextuir64le][1] = decimalunsig;
		    fprintf(xout, "uir64le__%d", nextuir64le);
		    nextuir64le++;
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
		    nextir16be++;
      }
      if (unsign) {
				decimalunsig = strtoull(decimalstr[1], &endptr, 10);
 			  if(*endptr != '\0'|| (decimalunsig < 0 || decimalunsig > UINT16_MAX) || decimalsig < uint16berange[nextuir32be][0]) {   
					printf("error: line %d - invalid range end\n", linenum);
		 		  exit(EXIT_FAILURE);
	 		  }
        uint16berange[nextuir16be][1] = decimalunsig;
		    fprintf(xout, "uir16be__%d", nextuir16be);
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
		    nextir32be++;
      }
      if (unsign) {
				decimalunsig = strtoull(decimalstr[1], &endptr, 10);
 			  if(*endptr != '\0'|| (decimalunsig < 0 || decimalunsig > UINT16_MAX) || decimalsig < uint32berange[nextuir32be][0]){   
					printf("error: line %d - invalid range end\n", linenum);
		 		  exit(EXIT_FAILURE);
	 		  }
        uint32berange[nextuir32be][1] = decimalunsig;
		    fprintf(xout, "uir32be__%d", nextuir32be);
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
		    nextir64be++;
      }
      if (unsign) {
				decimalunsig = strtoull(decimalstr[1], &endptr, 10);
 			  if(*endptr != '\0'|| errno == ERANGE || decimalsig < uint64berange[nextuir64be][0]) {
					printf("error: line %d - invalid range end\n", linenum);
		 		  exit(EXIT_FAILURE);
	 		  }
        uint64berange[nextuir64be][1] = decimalunsig;
		    fprintf(xout, "uir64be__%d", nextuir64be);
		    nextuir64be++;
      }
		}

    decimalsig = 0;
    decimalunsig = 0;
    nnum = 0;
    le = false;
    be = false;
	}

	
	static void setenum0(void) {
		if (hval < 0 && cval < 0) {
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

	static void setnextenum(void) {
		if (hval < 0 && cval < 0) {
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

	static void eend(void) {
		if (eindex == 1) {
			printf("error: line %d - single entry enumeration invalid\n", linenum);
			exit(EXIT_FAILURE);
		}
		fprintf(xout, "e__%d", nextenum);
		nextenum++; 
		eindex = 0; 

		ranging = false;
	}

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

  static void convert_int(uint8_t bytes_arr[], int64_t dec, int size) {
		int i;
		for (i = 0; i < size; i++) {
			bytes_arr[i] = dec & 0xff;
			dec >>= 8;
		}
	}

  static void convert_uint(uint8_t bytes_arr[], uint64_t dec, int size) {
		int i;
		for (i = 0; i < size; i++) {
			bytes_arr[i] = dec & 0xff;
			dec >>= 8;
		}
	}
	
	static void fixed_width(void){
		char *endptr;
		if (size == 8) {
			if (unsign && neg) {
			  printf("error: invalid entry for uint8\n");   
	 		  exit(EXIT_FAILURE);
		  }
			uint8_t bytes[1] = {0};
			if (!unsign) {
				decimalsig = strtoll(decimalstr[0], &endptr, 10);
				if (*endptr != '\0' || (decimalsig < INT8_MIN || decimalsig > INT8_MAX) ) {
				  printf("error: invalid entry for int8\n");
	 			  exit(EXIT_FAILURE);
 			  }
        convert_int(bytes, decimalsig, 1);
      }
      if (unsign) {
				decimalunsig = strtoull(decimalstr[0], &endptr, 10);
 			  if(*endptr != '\0'|| (decimalunsig < 0 || decimalunsig > UINT8_MAX)){   
			 	  printf("error: invalid entry for uint8\n");
		 		  exit(EXIT_FAILURE);          
	 		  }
        convert_uint(bytes, decimalunsig, 1);
      }
      write_int(bytes);
		}
		if (size == 16) {
			if (unsign && neg) {
			  printf("error: invalid entry for uint16\n");                              
	 		  exit(EXIT_FAILURE);
		  }
			uint8_t bytes[2] = {0, 0};  
			if (!unsign) {
				decimalsig = strtoll(decimalstr[0], &endptr, 10);
				if (*endptr != '\0' || (decimalsig < INT16_MIN || decimalsig > INT16_MAX) ) {
				  printf("error: invalid entry for int16\n");
	 			  exit(EXIT_FAILURE);
 			  }
        convert_int(bytes, decimalsig, 2);
      }
      if (unsign) {
				decimalunsig = strtoull(decimalstr[0], &endptr, 10);
 			  if(*endptr != '\0'|| (decimalunsig < 0 || decimalunsig > UINT16_MAX) ){   
			 	  printf("error: invalid entry for uint16\n");
		 		  exit(EXIT_FAILURE);          
	 		  }
        convert_uint(bytes, decimalunsig, 2);
      }
      write_int(bytes);
		}
		if (size == 32) {
			if (unsign && neg) {
			  printf("error: invalid entry for uint32\n");         
	 		  exit(EXIT_FAILURE);
		  }
			uint8_t bytes[4] = {0, 0, 0, 0};
			if (!unsign) {
				decimalsig = strtoll(decimalstr[0], &endptr, 10);
				if (*endptr != '\0' || (decimalsig < INT32_MIN || decimalsig > INT32_MAX) ){  
				  printf("error: invalid entry for int32\n");          
	 			  exit(EXIT_FAILURE);
 			  }
        convert_int(bytes, decimalsig, 4);
      }
      if (unsign) {
				decimalunsig = strtoull(decimalstr[0], &endptr, 10);
 			  if (*endptr != '\0' || (decimalunsig < 0 || decimalunsig > UINT32_MAX) ){   
			 	  printf("error: invalid entry for uint32\n");
		 		  exit(EXIT_FAILURE);
	 		  }
        convert_uint(bytes, decimalunsig, 4);
      }
      write_int(bytes);
		}
    if (size == 64) {
			if (unsign && neg) {
			  printf("error: invalid entry for uint32\n");
	 		  exit(EXIT_FAILURE);
		  }
			uint8_t bytes[8] = {0, 0, 0, 0, 0, 0, 0, 0};
			if (!unsign) {
				errno = 0;
				decimalsig = strtoll(decimalstr[0], &endptr, 10);
				if (*endptr != '\0' || errno == ERANGE){  
				  printf("error: invalid entry for int64\n");
	 			  exit(EXIT_FAILURE);           
 			  }
        convert_int(bytes, decimalsig, 8);
      }
      if (unsign) {
				errno = 0;
				decimalunsig = strtoull(decimalstr[0], &endptr, 10);
 			  if (*endptr != '\0' || errno == ERANGE){   
			 	  printf("error: invalid entry for uint64\n");
		 		  exit(EXIT_FAILURE);
	 		  }
        convert_uint(bytes, decimalunsig, 8);
      }
      write_int(bytes);
		}
    le = false;
    be = false;
    nnum=0;
		decimalsig=0;
    decimalunsig=0;
	}

  static void check_index(void) {
		if (i > 99) {
			printf("error: invalid number entry\n");
			exit(EXIT_FAILURE);
		}
	}
	
	static void hexout(void) {
		if (!ranging) {
			if (hval == 0)
				fprintf(xout, "X00");
			else
				fprintf(xout, "\'\\x%02x\'", hval);
		}
	}

  /* add rules for ranges and enumerations */
	static void addrules(void) {
		int i ,j, k;
		int64_t l;
		uint64_t m;
		if (anybyte) i = 0;					/* anybyte present, start at 0 */
		else i = 1;									/* otherwise, dont generate range 0 */

		if (anybyte || nextrange > 1)
			fprintf(xout, "\n/* Range Expansions */\n");
		for ( ; i < nextrange; i++) {
			fprintf(xout, "r__%d : ", i);
			Bnonterminals++; 
			for (k=0, j = range[i][RMIN]; j < range[i][RMAX]; k++, j++) {
				Brules++;
				Bterminals++;
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
      Brules++;
      Bterminals++;
		}

    if (nextir16le > 0)
			fprintf(xout, "\n/* int16 Little Endian Range Expansions */\n");
		for (i = 0 ; i < nextir16le; i++) {
			uint8_t bytes[2] = {0, 0};
			size = 16;
			fprintf(xout, "ir16le__%d : ", i);
			Bnonterminals++;
			for (k=0, l = int16lerange[i][RMIN]; l < int16lerange[i][RMAX]; k++, l++) {
				le = true;
				Brules++;
				Bterminals++;
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
      Brules++;
      Bterminals++;
		}
    le = false;

    if (nextuir16le > 0)
			fprintf(xout, "\n/* uint16 Little Endian Range Expansions */\n");
		for (i = 0 ; i < nextuir16le; i++) {
			uint8_t bytes[2] = {0, 0};
			size = 16;
			fprintf(xout, "uir16le__%d : ", i);
			Bnonterminals++;
			for (k=0, m = uint16lerange[i][RMIN]; m < uint16lerange[i][RMAX]; k++, m++) {
				le = true;
				Brules++;
				Bterminals++;
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
      Brules++;
      Bterminals++;
		}
    le = false;

    if (nextir32le > 0)
			fprintf(xout, "\n/* int32 Little Endian Range Expansions */\n");
		for (i = 0 ; i < nextir32le; i++) {
			uint8_t bytes[4] = {0, 0, 0, 0};
			size = 32;
			fprintf(xout, "ir32le__%d : ", i);
			Bnonterminals++;
			for (k=0, l = int32lerange[i][RMIN]; l < int32lerange[i][RMAX]; k++, l++) {
				le = true;
				Brules++;
				Bterminals++;
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
      Brules++;
      Bterminals++;
		}
    le = false;

    if (nextuir32le > 0)
			fprintf(xout, "\n/* uint32 Little Endian Range Expansions */\n");
		for (i = 0 ; i < nextuir32le; i++) {
			uint8_t bytes[4] = {0, 0, 0, 0};
			size = 32;
			fprintf(xout, "uir32le__%d : ", i);
			Bnonterminals++;
			for (k=0, m = uint32lerange[i][RMIN]; m < uint32lerange[i][RMAX]; k++, m++) {
				le = true;
				Brules++;
				Bterminals++;
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
      Brules++;
      Bterminals++;
		}
    le = false;

    if (nextir64le > 0)
			fprintf(xout, "\n/* int64 Little Endian Range Expansions */\n");
		for (i = 0 ; i < nextir64le; i++) {	
			uint8_t bytes[8] = {0, 0, 0, 0, 0, 0, 0, 0};
			size = 64;
			fprintf(xout, "ir64le__%d : ", i);
			Bnonterminals++;
			for (k=0, l = int64lerange[i][RMIN]; l < int64lerange[i][RMAX]; k++, l++) {
				le = true;
				Brules++;
				Bterminals++;
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
      Brules++;
      Bterminals++;
		}
    le = false;

    if (nextuir64le > 0)
			fprintf(xout, "\n/* uint64 Little Endian Range Expansions */\n");
		for (i = 0 ; i < nextuir64le; i++) {
			uint8_t bytes[8] = {0, 0, 0, 0, 0, 0, 0, 0};
			size = 64;
			fprintf(xout, "uir64le__%d : ", i);
			Bnonterminals++;
			for (k=0, m = uint64lerange[i][RMIN]; m < uint64lerange[i][RMAX]; k++, m++) {
				le = true;
				Brules++;
				Bterminals++;
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
      Brules++;
      Bterminals++;
		}
    le = false;

    if (nextir16be > 0)
			fprintf(xout, "\n/* int16 Big Endian Range Expansions */\n");
		for (i = 0 ; i < nextir16be; i++) {	
			uint8_t bytes[2] = {0, 0};
			size = 16;
			fprintf(xout, "ir16be__%d : ", i);
			Bnonterminals++;
			for (k=0, l = int16berange[i][RMIN]; l < int16berange[i][RMAX]; k++, l++) {
				be = true;
				Brules++;
				Bterminals++;
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
      Brules++;
      Bterminals++;
		}
    be = false;

    if (nextuir16be > 0)
			fprintf(xout, "\n/* uint16 Big Endian Range Expansions */\n");
		for (i = 0 ; i < nextuir16be; i++) {
			uint8_t bytes[2] = {0, 0};
			size = 16;
			fprintf(xout, "uir16be__%d : ", i);
			Bnonterminals++;
			for (k=0, m = uint16berange[i][RMIN]; m < uint16berange[i][RMAX]; k++, m++) {
				be = true;
				Brules++;
				Bterminals++;
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
      Brules++;
      Bterminals++;
		}
    be = false;

    if (nextir32be > 0)
			fprintf(xout, "\n/* int32 Big Endian Range Expansions */\n");
		for (i = 0 ; i < nextir32be; i++) {	
			uint8_t bytes[4] = {0, 0, 0, 0};
			size = 32;
			fprintf(xout, "ir32be__%d : ", i);
			Bnonterminals++;
			for (k=0, l = int32berange[i][RMIN]; l < int32berange[i][RMAX]; k++, l++) {
				be = true;
				Brules++;
				Bterminals++;
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
      Brules++;
      Bterminals++;
		}
    be = false;

    if (nextuir32be > 0)
			fprintf(xout, "\n/* uint32 Big Endian Range Expansions */\n");
		for (i = 0 ; i < nextuir32be; i++) {
			uint8_t bytes[4] = {0, 0, 0, 0};
			size = 32;
			fprintf(xout, "uir32be__%d : ", i);
			Bnonterminals++;
			for (k=0, m = uint32berange[i][RMIN]; m < uint32berange[i][RMAX]; k++, m++) {
				be = true;
				Brules++;
				Bterminals++;
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
      Brules++;
      Bterminals++;
		}
    be = false;

    if (nextir64be > 0)
			fprintf(xout, "\n/* int64 Big Endian Range Expansions */\n");
		for (i = 0 ; i < nextir64be; i++) {	
			uint8_t bytes[8] = {0, 0, 0, 0, 0, 0, 0, 0};
			size = 64;
			fprintf(xout, "ir64be__%d : ", i);
			Bnonterminals++;
			for (k=0, l = int64berange[i][RMIN]; l < int64berange[i][RMAX]; k++, l++) {
				be = true;
				Brules++;
				Bterminals++;
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
      Brules++;
      Bterminals++;
		}
    be = false;

    if (nextuir64be > 0)
			fprintf(xout, "\n/* uint64 Big Endian Range Expansions */\n");
		for (i = 0 ; i < nextuir64be; i++) {
			uint8_t bytes[8] = {0, 0, 0, 0, 0, 0, 0, 0};
			size = 64;
			fprintf(xout, "uir64be__%d : ", i);
			Bnonterminals++;
			for (k=0, m = uint64berange[i][RMIN]; m < uint64berange[i][RMAX]; k++, m++) {
				be = true;
				Brules++;
				Bterminals++;
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
      Brules++;
      Bterminals++;
		}
    be = false;
		
		if (nextenum > 0)
			fprintf(xout, "\n/* Enumeration Expansions */\n");
		for (i = 0; i < nextenum; i++) {		             /* for each enumeration */
			fprintf(xout, "e__%d : ", i);
			j = 0;
			k = 0;
			Bnonterminals++;
			Brules++;
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
			Bnonterminals++;
			Brules++;
			while (strings[i][j+1] >= 0){
				Bterminals++;
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
			Bterminals++;
			if (k%8 == 0)                     
				fprintf(xout, "\n  ");    
			if (strings[i][j] == 0)                                                                               
				fprintf(xout, "X00 ;\n");
			else
				fprintf(xout, "\'%c\' ;\n", strings[i][j]);
		}
	}
	
	static void print_counts(void){
		counts = fopen("gmr_counts", "w");
		fprintf(counts, "XBNF %d %d %d ", Xrules, Xterminals, Xnonterminals);
		fprintf(counts, "Bison %d %d %d\n", Brules, Bterminals, Bnonterminals);
		fclose(counts);
	}
	
%}	

	
%token X00
		 
%%

bnf: { init(); } rules ws0 { addrules(); print_counts(); }
 
rules: rule
		 | rules rule
		 ;

rule: ws0 nonterminal ws0 ':' { fprintf(xout,":"); } rhs ';' { fprintf(xout,";"); }
		| ws0 comment
		;

rhs: terms ws1
   | rhs '|' { fprintf(xout,"|" ); } terms ws1
	 ;

terms: /* empty */
		 | terms ws1 term
		 ;

term: terminal
		| nonterminal
	  | range
	  | comment
		| string
		;

string: '"' letters '"' { c = 0; fprintf(xout,"s__%d", nstr); nstr++; }
			;

letters: c
			 | letters c
			 ;

c : alphanumeric   { cval = $1; strings[nstr][c] = cval; c++; cval = -1; }  
  | punct          { cval = $1; strings[nstr][c] = cval; c++; cval = -1; }
  | ws             { cval = $1; strings[nstr][c] = cval; c++; cval = -1; } 
  | '\\' escchar   { cval = $2; strings[nstr][c] = '\\'; strings[nstr][c+1] = cval; c=c+2; cval=-1; }
  ;

ws: '\t' '\n' '\r'
	;

terminal: '\'' termval '\''
				;

fwi: type sizebe '(' sign fullnumber ')'         {be = true; fixed_width(); fixedW=true;}
| le '(' sign fullnumber ws0 ',' ws0 type sizele ')'  {le = true; fixed_width(); fixedW=true;}
	 ;

type: 'u' 'i' 'n' 't'    { unsign = true; }
    | 'i' 'n' 't'        { unsign = false; }
    ;


sizele: '1' '6' { size = 16; }
      | '3' '2' { size = 32; }
    	| '6' '4' { size = 64; }
      ;

sizebe: '8'     { size = 8; }
			| '1' '6' { size = 16; }
      | '3' '2' { size = 32; }
    	| '6' '4' { size = 64; }
      ;

sign: '-'         { neg = true; }
    | /*empty */  { neg = false; }
		;

fullnumber: number { decimalstr[nnum][i] = '\0'; i = 0; nnum++; }
				  ;

number: digit        { if (neg) decimalstr[nnum][i++] = '-'; decimalstr[nnum][i++] = $1; }
			| number digit { decimalstr[nnum][i++] = $2; check_index(); }
      ;

le: 'l' 'i' 't' 't' 'l' 'e' '_' 'e' 'n' 'd' 'i' 'a' 'n'
	;

termval: charval
			 | hexval
			 | fwi
			 ;

charval: alphanumeric       { cval = $1; if (!ranging) fprintf(xout, "\'%c\'", (char)$1); }
       | punct              { cval = $1; if (!ranging) fprintf(xout, "\'%c\'", (char)$1); }
       | '"'                { cval = $1; if (!ranging) fprintf(xout, "\'%c\'", (char)$1); }
       | '\\' escchar       { cval = $2; if (!ranging) fprintf(xout, "\'\\%c\'", (char)$2); }
			 ;

alphanumeric: uchar
						| lchar
						| digit
						;

hexval: '\\' 'x' { hval = 0; } hexdigit { hval = 16*hval; } hexdigit { hexout(); }
      ;

hexdigit: digit             { hval += $1 - '0'; }
        | uhex              { hval += $1 - 'A' + 10; }
        | lhex              { hval += $1 - 'a' + 10; }
        ;

nonterminal: symbolchars
           ;

symbolchars: symbolchar
					 | symbolchars symbolchar
					 ;

symbolchar: alphanumeric   { fprintf(xout, "%c", (char)$1); }
          | '_'            { fprintf(xout, "_"); }
					| '.'            { fprintf(xout, "."); }
					;

range: '[' { rbegin(); } elements ']'
     | '*' { fprintf(xout,"r__0"); anybyte = true; }
     ;

elements: ws0 terminal ws0 { setrlow(); } '-' ws0 terminal ws0 { setrhigh(); }
        | enumeration { eend(); }
        ;

enumeration: ws0 terminal ws0 { setenum0(); } | enumeration ',' ws0 terminal ws0 { setnextenum(); }
           ;

/* echo comments */
comment: '/' '*' { fprintf(xout,"/*"); } commentchars '*' '/' { fprintf(xout,"*/"); }
       ;

commentchars: commentchar
						| commentchars commentchar
						;

commentchar : alphanumeric      { fprintf(xout, "%c", (char)$1); }
            | '*'               { fprintf(xout, "%c", (char)$1); }
            | '('               { fprintf(xout, "%c", (char)$1); }
            | ')'               { fprintf(xout, "%c", (char)$1); }
            | '-'               { fprintf(xout, "%c", (char)$1); }
            | ','               { fprintf(xout, "%c", (char)$1); } 
            | '\''              { fprintf(xout, "%c", (char)$1); }
            | '_'               { fprintf(xout, "%c", (char)$1); } 
            | wschar /* already output */
            ;

uchar: uhex | 'G' | 'H' | 'I' | 'J' 
      | 'K' | 'L' | 'M' | 'N' | 'O' | 'P' | 'Q' | 'R' | 'S' | 'T' 
      | 'U' | 'V' | 'W' | 'X' | 'Y' | 'Z'
			;

lchar: lhex | 'g' | 'h' | 'i' | 'j' 
      | 'k' | 'l' | 'm' | 'n' | 'o' | 'p' | 'q' | 'r' | 's' | 't' 
      | 'u' | 'v' | 'w' | 'x' | 'y' | 'z'
			;

uhex: 'A' | 'B' | 'C' | 'D' | 'E' | 'F'
		;

lhex: 'a'| 'b' | 'c' | 'd' | 'e' | 'f'
	  ;

punct:  ' ' | '!' | '#' | '$' | '%' | '&' | '\'' | '(' | ')' | '*' | '+' 
     | ',' | '-' | '.' | '/' | ':' | ';' | '<' | '=' | '>' | '?' | '@'
     | '[' | ']' | '^' | '_' | '`' | '{' | '|' | '}' | '~'
     ;

escchar: 'b' | 'f' | 'n' | 'r' | 't' | '\"' | '\\' | '/' | 'v' | '?' | 'a' | 'e'
			 ;

digit: '0' | onenine
		 ;

onenine: '1' | '2' | '3' | '4' | '5'| '6' | '7' | '8' | '9'
			 ;


ws0: /* empty */ | ws1
	 ;

ws1: wschar | ws1 wschar
	 ;

/* echo whitespace */
wschar:  ' '   { fprintf(xout," "); } 
      | '\n'   { linenum++; fprintf(xout,"\n"); } 
      | '\t'   { fprintf(xout,"\t"); } 
      | '\r'   { fprintf(xout,"\r"); } 
			;
