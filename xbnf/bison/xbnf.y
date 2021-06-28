%{

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
	
#define YYDEBUG 1
#define MAXENTRY 100						/* max number of separate ranges */
#define RMIN 0									/* index of min range value  */
#define RMAX 1									/* index of max range value */
#define ENUMMAX 10							/* max number of entries in an enumeration */
#define CHARMAX 50                 /*max number of characters in a string*/

	int yylex(void);
	void yyerror(char *s);

	extern FILE* xout;
	extern int yylval;

	static int linenum=1;
	static int hval;						/* variable used to form a hex value from ascii chars */
	static int cval;              /* variable used to hold a character value */
	static int nextrange=0;				/* number of ranges */
	static int nextenum=0;				/* number of enumerations */
	static int eindex=0;          /* index into an emumeration */
	static int rlow;							/* low laue in a range */
	static int rhigh;							/* high value in a range */
	static int range[MAXENTRY][2];			/* the ranges */
	static int enumeration[MAXENTRY][ENUMMAX]; /* the enumerations */
	static bool anybyte=false;		/* true if anybytes is used in the grammar */
	static bool ranging=false;		/* true if insize a range */
	static int strings[MAXENTRY][CHARMAX];   /*any strings that are found within rules*/
	static int c=0;               /*variable used to hold character value for a string*/
	static int nstr=0;            /*number of strings */

	static void init() {	/* clear the tables */
		int i,j;
		linenum=1;
		range[0][RMIN] = 0;					/* r__0 if used is anybyte */
		range[0][RMAX] = 255;
		nextrange++;								/* first range is 1 */
		anybyte=false;							/* anybyte not yet detected */
		ranging=false;
		for(i=1; i<MAXENTRY; i++) {	/* ranges */
			range[i][RMIN] = -1;
			range[i][RMAX] = -1 ;
		}
		for(i=0; i<MAXENTRY; i++) {	/* enumerations */
			for(j=0; j<ENUMMAX; j++) 
				enumeration[i][j] = -1;
		}
		for(i=0; i<MAXENTRY; i++) { /* strings */                                                                                             
      for(j=0; j<CHARMAX; j++)                                                                                                                 
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

	static void rbegin() { /* clear the range indexes */
		rlow = -1;
		rhigh = -1;
		cval = -1;
		hval = -1;
		eindex = 0; 								/* first enumeration index */
		ranging=true;
	}

	static void setrlow() {
		if(hval<0 && cval<0) {
			printf("error: line %d - invalid range start\n",linenum);
			exit(EXIT_FAILURE);
		}
		if(hval>=0) rlow=hval;
  		else rlow=cval;
		hval=-1;										/* reset for next value */
		cval=-1;
	}
	
	static void setrhigh() {
		if(hval<0 && cval<0) {
			printf("error: line %d - invalid range end\n",linenum);
			exit(EXIT_FAILURE);
		}
		if(hval>=0)
			rhigh=hval;
		else
			rhigh=cval;
		hval=-1;										/* reset for next value */
		cval=-1;

		if(rhigh<=rlow) {   /* legal range */
			printf("error: line %d - invalid range [%d-%d] -- low to high required\n",linenum,rlow,rhigh);
			exit(EXIT_FAILURE);
		}

		if(rlow==0 && rhigh==255) {							/* same as anybyte */
			fprintf(xout,"r__0");											/* r__0 is anybyte */
			anybyte=true;													/* at least one use */
		}
		else {
			range[nextrange][RMIN] = rlow;				/* store it */
			range[nextrange][RMAX] = rhigh;
			fprintf(xout,"r__%d",nextrange); /* generate a symbol */
			nextrange++;
		}
		ranging=false;
	}
	
	static void setenum0() {
		if(hval<0 && cval<0) {
			printf("error: line %d - invalid enumeration\n",linenum);
			exit(EXIT_FAILURE);
		}
		if(hval>=0)
			enumeration[nextenum][0]=hval;
		else
			enumeration[nextenum][0]=cval;
		hval=-1;
		cval=-1;
		eindex=1;
	}

	static void setnextenum() {
		if(hval<0 && cval<0) {
			printf("error: line %d - invalid enumeration\n",linenum);
			exit(EXIT_FAILURE);
		}
		if(hval>=0)
			enumeration[nextenum][eindex]=hval;
		else
			enumeration[nextenum][eindex]=cval;
		hval=-1;
		cval=-1;
		eindex++;																 
	}

	static void eend() {
		if(eindex==1) {
			printf("error: line %d - single entry enumeration invalid\n",linenum);
			exit(EXIT_FAILURE);
		}
		fprintf(xout,"e__%d",nextenum);
		nextenum++;
		eindex=0;
		ranging=false;
	}
	
	static void hexout() {
		if(!ranging) {
			if(hval==0)
				fprintf(xout,"X00");
			else
				fprintf(xout,"\'\\x%02x\'",hval);
		}
	}
	
	static void addrules() {							/* add rules for ranges and enumerations */
		int i,j,k;
		
		if(anybyte)	i=0;						/* anybyte present, start at 0 */
		else i=1;									/* otherwise, dont generate range 0 */
		
		if(anybyte || nextrange>1)
			fprintf(xout,"\n/* Range Expansions */\n");
		for(; i<nextrange; i++) {		/* for each range -- only valid ranges in table*/
			fprintf(xout,"r__%d : ",i);
			for(k=0,j=range[i][RMIN]; j<range[i][RMAX]; k++,j++) {
				if(k%8==0)
					fprintf(xout,"\n  ");
				if(j==0)
					fprintf(xout,"X00 | ");
				else
					fprintf(xout,"\'\\x%02x\' | ",j);
			}
			if(k%8==0)
				fprintf(xout,"\n  ");
			fprintf(xout,"\'\\x%02x\' ;\n",j);
		}
		
		if(nextenum>0)
			fprintf(xout,"\n/* Enumeration Expansions */\n");
		for(i=0; i<nextenum; i++) {		/* for each enumeration */
			fprintf(xout,"e__%d : ",i);
			j=0;
			k=0;
		  while(enumeration[i][j+1]>=0) {	/* next element present */
				if(k%8==0)
					fprintf(xout,"\n  ");
				if(enumeration[i][j]==0)
					fprintf(xout,"X00 | ");
				else
					fprintf(xout,"\'\\x%02x\' | ",enumeration[i][j]);
				j++;
				k++;
			}
			if(k%8==0) 
				fprintf(xout,"\n  ");
			if(enumeration[i][j]==0)
				fprintf(xout,"X00 ;\n");
			else
				fprintf(xout,"\'\\x%02x\' ;\n",enumeration[i][j]);
		}
	
		if (nstr>0)
			fprintf(xout, "\n/* String Expansions */\n");
		for(i=0; i < nstr; i++){
			fprintf(xout, "s_%d : ", i);
			j=0;
			k=0;
			while (strings[i][j+1] >= 0){
				if (k%8==0)
					fprintf(xout, "\n ");
				if(strings[i][j] == 0)
					fprintf(xout,"X00 ");
				if (strings[i][j] == '\\')
					fprintf(xout, "'\\\\'");
				else                                                                                                               
					fprintf(xout,"\'%c\' ", strings[i][j]);   
				j++;              
				k++;    
			}                                                                                                                    
			if(k%8==0)                     
				fprintf(xout,"\n  ");                                                                                         
			if(strings[i][j]==0)                                                                               
				fprintf(xout,"X00 ;\n");                                  
			else    
				fprintf(xout,"\'%c\' ;\n", strings[i][j]);
				}
	}
%}	
	
	
%token X00
		 
%%

bnf: { init(); } rules ws0 { addrules(); } ;

rules: rule | rules rule ;

rule : ws0 nonterminal ws0 ':' { fprintf(xout,":"); } rhs ';' { fprintf(xout,";"); } | ws0 comment ;

rhs : terms ws1 | rhs '|' { fprintf(xout,"|" ); } terms ws1 ;

terms : /* empty */ | terms ws1 term ;

term : terminal | nonterminal | range | comment | string ;

string : '\"' letters '\"' {c = 0; fprintf(xout,"s_%d", nstr); nstr++;};

letters : c | letters c ;

c : alphanumeric   { cval = $1; strings[nstr][c] = cval; c++; cval = -1;}  
  | punct          { cval = $1; strings[nstr][c] = cval; c++; cval = -1;}
  | '\\' escchar   { cval = $2; strings[nstr][c] = '\\'; strings[nstr][c+1] = cval; c=c+2; cval=-1; }
  ;

terminal : '\'' termval '\'' ;

termval : charval | hexval ;

charval: alphanumeric       { cval = $1; if(!ranging) fprintf(xout,"\'%c\'",(char)$1); }
       | punct              { cval = $1; if(!ranging) fprintf(xout,"\'%c\'",(char)$1); }
       | '\\' escchar       { cval = $2; if(!ranging) fprintf(xout,"\'\\%c\'",(char)$2); }
			 ;

alphanumeric: uchar | lchar | digit ;

hexval: '\\' 'x' { hval=0; } hexdigit { hval=16*hval; } hexdigit { hexout(); } ;

hexdigit: digit             { hval += $1 - '0'; }
        | uhex              { hval += $1 - 'A' + 10; }
        | lhex              { hval += $1 - 'a' + 10; }
        ;

nonterminal : symbolchars ;
symbolchars : symbolchar | symbolchars symbolchar ;

symbolchar : alphanumeric   { fprintf(xout,"%c",(char)$1); }
           | '_'            { fprintf(xout,"_"); }
					 | '.'            { fprintf(xout,"."); }
					 ;

range : '[' { rbegin(); } elements ']' 
      | '*' { fprintf(xout,"r__0"); anybyte=true; }
      ;

elements : ws0 terminal ws0 { setrlow(); } '-' ws0 terminal ws0 { setrhigh(); }
         |  enumeration { eend(); }
         ;

enumeration : ws0 terminal ws0 { setenum0(); } | enumeration ',' ws0 terminal ws0 { setnextenum(); } ;

/* echo comments */
comment: '/' '*' { fprintf(xout,"/*"); } commentchars '*' '/' { fprintf(xout,"*/"); }
       ;
commentchars : commentchar | commentchars commentchar ;
commentchar : alphanumeric      { fprintf(xout,"%c",(char)$1); }
            | '*'               { fprintf(xout,"*"); }
            | '_'               { fprintf(xout,"_"); }
            | ','               { fprintf(xout,","); }
            | '-'               { fprintf(xout,"-"); }
            | '.'               { fprintf(xout,"."); }
            | ':'               { fprintf(xout,":"); }
            | ';'               { fprintf(xout,";"); }
            | '\''              { fprintf(xout,"\'"); }
            | '('              { fprintf(xout,"("); }
            | ')'              { fprintf(xout,")"); }
            | wschar       			/* whitespace already echod */
            ;

uchar: uhex | 'G' | 'H' | 'I' | 'J' 
      | 'K' | 'L' | 'M' | 'N' | 'O' | 'P' | 'Q' | 'R' | 'S' | 'T' 
      | 'U' | 'V' | 'W' | 'X' | 'Y' | 'Z' ;

lchar: lhex | 'g' | 'h' | 'i' | 'j' 
      | 'k' | 'l' | 'm' | 'n' | 'o' | 'p' | 'q' | 'r' | 's' | 't' 
      | 'u' | 'v' | 'w' | 'x' | 'y' | 'z' ;

uhex: 'A' | 'B' | 'C' | 'D' | 'E' | 'F' ;
lhex: 'a'| 'b' | 'c' | 'd' | 'e' | 'f' ;

punct:  ' ' | '!' | '#' | '$' | '%' | '&' | '\'' | '(' | ')' | '*' | '+' 
      | ',' | '-' | '.' | '/' | ':' | ';' | '<' | '=' | '>' | '?' | '@'
      | '[' | ']' | '^' | '_' | '`' | '{' | '|' | '}' | '~' ;

escchar: 'b' | 'f' | 'n' | 'r' | 't' | '\"' | '\\' | '/' | 'v' | '?' | 'a' | 'e';

digit: '0' | onenine ;
onenine: '1' | '2' | '3' | '4' | '5'| '6' | '7' | '8' | '9' ;


ws0: /* empty */ | ws1 ;
ws1: wschar | ws1 wschar ;

/* echo whitespace */
wschar :  ' '   { fprintf(xout," "); } 
       | '\n'   { linenum++; fprintf(xout,"\n"); } 
       | '\t'   { fprintf(xout,"\t"); } 
       | '\r'   { fprintf(xout,"\r"); } 
			 ;

