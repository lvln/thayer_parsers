%{

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <netinet/in.h>
	
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
	static int hval;						  /* variable used to form a hex value from ascii chars */
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

	/* for uint & int16 ranges and enums*/
	static int STrange=0;
	static int nextSTenum;
  static int STeindex=0;
	static int sixteen_range[MAXENTRY][2];
	static int sixteen_enum[MAXENTRY][ENUMMAX];
	
	static long long decimal=0; /*number to hold decimal in fwi*/
	static int size=16;
	static bool neg=false;
	static bool unsign=true; /*default variable for fwi, intially unsigned*/
	static bool fixedW=false;
  static bool le=false;
  static bool be=false;

	static FILE* counts;
	static int Xrules=0;
	static int Xterminals=0;
	static int Xnonterminals=0;

	static int Brules=0;
	static int Bterminals=0;  
  static int Bnonterminals=0;

	
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
		for(i=1; i<MAXENTRY; i++) { /* ranges */
      sixteen_range[i][RMIN] = -1;
      sixteen_range[i][RMAX] = -1 ;
    } 
		for(i=0; i<MAXENTRY; i++) {	/* enumerations */
			for(j=0; j<ENUMMAX; j++) 
				enumeration[i][j] = -1;
		}
		for(i=0; i<MAXENTRY; i++) { /* enumerations */   
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
		if(hval<0 && cval<0 && !fixedW) {
			printf("error: line %d - invalid range start\n",linenum);
			exit(EXIT_FAILURE);
		}
		if(hval>=0) rlow=hval;
  		else rlow=cval;
		
		if (fixedW) rlow = hval;
		hval=-1;										/* reset for next value */
		cval=-1;
	}
	
	static void setrhigh() {
		if (hval<0 && cval<0 && !fixedW) {
			printf("error: line %d - invalid range end\n",linenum);
			exit(EXIT_FAILURE);
		}
		if(hval>=0)
			rhigh=hval;
		else
			rhigh=cval;
		if (fixedW) rhigh = hval; 
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
		else if (size==16 && fixedW){
			sixteen_range[STrange][RMIN] = rlow;        /* store it */     
      sixteen_range[STrange][RMAX] = rhigh;    
      fprintf(xout,"rr__%d",STrange); /* generate a symbol */        
      STrange++;
		}
		else {
			range[nextrange][RMIN] = rlow;				/* store it */
			range[nextrange][RMAX] = rhigh;
			fprintf(xout,"r__%d",nextrange); /* generate a symbol */
			nextrange++;
		}
		ranging=false;
		fixedW=false;
	}


	static void STenum0(){
		sixteen_enum[nextSTenum][0]=hval;
		hval =-1;
		STeindex=1;
	}
	
	static void setenum0() {
		if(hval<0 && cval<0 && !fixedW) {
			printf("error: line %d - invalid enumeration\n",linenum);
			exit(EXIT_FAILURE);
		}
		if (size ==16 && fixedW) {STenum0(); return;}
		
		if(hval>=0)
			enumeration[nextenum][0]=hval;
		else
			enumeration[nextenum][0]=cval;

		if (fixedW) enumeration[nextenum][0]=hval;
		
		hval=-1;
		cval=-1;
		eindex=1;
	}

	static void setSTenum(){
		sixteen_enum[nextSTenum][STeindex]=hval;
		hval =-1;
		STeindex++;
	}

	static void setnextenum() {
		if(hval<0 && cval<0 && !fixedW) {
			printf("error: line %d - invalid enumeration\n",linenum);
			exit(EXIT_FAILURE);
		}
		if (size ==16 && fixedW) {setSTenum(); return;}
		
		if(hval>=0)
			enumeration[nextenum][eindex]=hval;
		else
			enumeration[nextenum][eindex]=cval;
		
		if (fixedW) enumeration[nextenum][eindex]=hval;
		hval=-1;
		cval=-1;
		eindex++;																 
	}

	static void eend() {
		if(eindex==1 && size!= 16) {
			printf("error: line %d - single entry enumeration invalid\n",linenum);
			exit(EXIT_FAILURE);
		}
		if (size==16 && fixedW){
			fprintf(xout,"ee__%d",nextSTenum);   
			nextSTenum++;      
			STeindex=0;
		}
		else{
			fprintf(xout,"e__%d",nextenum);
			nextenum++; 
			eindex=0; 
		}
		ranging=false;
		fixedW=false;
	}

  static void write_int(uint8_t *bytes){
		int i;
		if (le){
		  for (i=0; i<size/8; i++) {
				if (bytes[i] == 0) fprintf(xout, "X00 ");
				else fprintf(xout, "\'\\x%02x\' ", bytes[i]);
				if (i!=size/8-1) fprintf(xout, " ");
			}
		}
		else if (be){
		  for (i=size/8-1; i>=0; i--) {
				if (bytes[i] == 0) fprintf(xout, "X00");
				else fprintf(xout, "\'\\x%02x\'", bytes[i]);
				if (i!=0) fprintf(xout, " ");
			}
		}
	}
	
	static void fixed_width(){
		if (size==16){
			if (decimal==0 && !ranging) fprintf(xout, "X00 X00 ");
			else{                                                                                                   
				if (!unsign && (decimal < -32768 || decimal > 32767) ){  
					printf("error: invalid entry for int16\n");                                          
					exit(EXIT_FAILURE);                              
				}                                      
				if(unsign && (decimal < 0 || decimal > 65535) ){                                            
					printf("error: invalid entry for uint16\n");
					exit(EXIT_FAILURE);          
				}
				if (!ranging){
					uint8_t bytes[2] = {0,0};  
					memcpy(bytes, &decimal, 2);
          write_int(bytes);
				}
			}
		}
    if (size==32){
			if (decimal==0 && !ranging) fprintf(xout, "X00 X00 X00 X00 ");
			else{                                                                                                   
				if (!unsign && (decimal < -2147483648 || decimal > 2147483647) ){  
					printf("error: invalid entry for int32\n");                                          
					exit(EXIT_FAILURE);                              
				}                                      
				if(unsign && (decimal < 0 || decimal > 4294967295) ){                                            
					printf("error: invalid entry for uint32\n");
					exit(EXIT_FAILURE);          
				}
				if (!ranging){
					uint8_t bytes[4] = {0,0,0,0};  
					memcpy(bytes, &decimal, 4);
          write_int(bytes);
				}
			}
		}
    if (size==64){
			if (decimal==0 && !ranging) fprintf(xout, "X00 X00 X00 X00 X00 X00 X00 X00 ");
			else{                                                                                                   
					if (!unsign && (decimal < -9223372036854775807LL - 1 || decimal > 9223372036854775807) ){  
					printf("error: invalid entry for int64\n");
					exit(EXIT_FAILURE);                              
				}                                      
				if(unsign && (decimal < 0 || decimal > (uint64_t)18446744073709551615) ){     
					printf("error: invalid entry for uint64\n");
					exit(EXIT_FAILURE);          
				}
				if (!ranging){
					uint8_t bytes[8] = {0,0,0,0,0,0,0,0};  
					memcpy(bytes, &decimal, 8);
          write_int(bytes);
				}
			}
		}
		hval=decimal;
		decimal=0;
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
			Bnonterminals++; 
			for(k=0,j=range[i][RMIN]; j<range[i][RMAX]; k++,j++) {
				Brules++; Bterminals++;
				if(k%8==0)
					fprintf(xout,"\n  ");
				if(j==0)
					fprintf(xout,"X00 | ");
				else
					fprintf(xout,"\'\\x%02x\' | ", (uint8_t) j);
			}
			if(k%8==0)
				fprintf(xout,"\n  ");
			fprintf(xout,"\'\\x%02x\' ;\n", (uint8_t) j); Brules++; Bterminals++;
		}
		
		if(nextenum>0)
			fprintf(xout,"\n/* Enumeration Expansions */\n");
		for(i=0; i<nextenum; i++) {		/* for each enumeration */
			fprintf(xout,"e__%d : ",i);
			j=0;
			k=0;
			Bnonterminals++;
			Brules++;
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
			fprintf(xout, "s__%d : ", i);
			j=0;
			k=0;
			Bnonterminals++;
			Brules++;
			while (strings[i][j+1] >= 0){
				Bterminals++;
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
			Bterminals++;
			if(k%8==0)                     
				fprintf(xout,"\n  ");    
			if(strings[i][j]==0)                                                                               
				fprintf(xout,"X00 ;\n");                                  
			else    
				fprintf(xout,"\'%c\' ;\n", strings[i][j]); 
				}

		uint8_t bytes[2] = {0,0};
		
		if(STrange> 0)
      fprintf(xout,"\n/* 16 Range Expansions */\n");
		for(i=0; i<STrange; i++) {   /* for each range -- only valid ranges in table*/    
      fprintf(xout,"rr__%d : ",i);
			Bnonterminals++;
      for(k=0,j=sixteen_range[i][RMIN]; j<sixteen_range[i][RMAX]; k++,j++) {
				Brules++; Bterminals++;
        if(k%8==0)                                                              
          fprintf(xout,"\n  ");                             
        if(j==0)                             
          fprintf(xout,"X00 X00 | "); 
        else{
					memcpy(bytes, &j, 2);
          if (bytes[1] == 0)    fprintf(xout, "\'\\x%02x\' X00 | ", bytes[0]);                            
					else   
						fprintf(xout, "\'\\x%02x\' \'\\x%02x\' | ", bytes[0], bytes[1]);
				}                                                
      }
			
      if(k%8==0)          
        fprintf(xout,"\n  ");
			memcpy(bytes, &j, 2);
			Brules++; Bterminals++;
			if (bytes[1] == 0)    fprintf(xout, "\'\\x%02x\' X00 ;\n", bytes[0]);     
			else                   
				fprintf(xout, "\'\\x%02x\' \'\\x%02x\' ;\n", bytes[0], bytes[1]);
		}
		
		if(nextSTenum>0) 
      fprintf(xout,"\n/* 16 Enumeration Expansions */\n");
    for(i=0; i<nextSTenum; i++) {   /* for each enumeration */   
      fprintf(xout,"ee__%d : ",i);    
      j=0;
      k=0;
			Bnonterminals++; 
      Brules++; 
      while(sixteen_enum[i][j+1]>=0) { /* next element present */    
        if(k%8==0)   
          fprintf(xout,"\n  ");     
        if(sixteen_enum[i][j]==0)             
          fprintf(xout,"X00 X00 | ");      
        else{                                                            
          memcpy(bytes, &j, 2);      
          if (bytes[1] == 0)    fprintf(xout, "\'\\x%02x\' X00 | ", bytes[0]);     
          else   
            fprintf(xout, "\'\\x%02x\' \'\\x%02x\' | ", bytes[0], bytes[1]);
				}
				j++;     
        k++;          
      }    
      if(k%8==0)           
        fprintf(xout,"\n  ");    
      if(sixteen_enum[i][j]==0)         
        fprintf(xout,"X00 X00 ;\n");  
      else{         
				memcpy(bytes, &j, 2);                                                                                   
				if (bytes[1] == 0)    fprintf(xout, "\'\\x%02x\' X00 | ", bytes[0]); 
				else 
					fprintf(xout, "\'\\x%02x\' \'\\x%02x\' | ", bytes[0], bytes[1]);
			}   
		}	
	}
	
	static void print_counts(){
		counts = fopen("gmr_counts", "w");
		fprintf(counts,"XBNF %d %d %d ", Xrules, Xterminals, Xnonterminals);    
		fprintf(counts,"Bison %d %d %d\n", Brules, Bterminals, Bnonterminals);
		fclose(counts);	
	}
	
%}	

	
%token X00
		 
%%

bnf: { init(); } rules ws0 { addrules(); print_counts();}
 
rules: rule | rules rule ;

rule : ws0 nonterminal ws0 ':' { fprintf(xout,":"); } rhs ';' { fprintf(xout,";"); } | ws0 comment ;

rhs : terms ws1 {Xrules++; Brules++; } | rhs '|' { fprintf(xout,"|" ); } terms ws1 {Xrules++; Brules++;};

terms : /* empty */ | terms ws1 term ;

term : terminal | nonterminal | range | comment | string ;

string : '"' letters '"' {c = 0; fprintf(xout,"s__%d", nstr); nstr++; Xnonterminals++; Bnonterminals++;};

letters : c | letters c ;

c : alphanumeric   { cval = $1; strings[nstr][c] = cval; c++; cval = -1;}  
  | punct          { cval = $1; strings[nstr][c] = cval; c++; cval = -1;}
  | ws             { cval = $1; strings[nstr][c] = cval; c++; cval = -1;} 
  | '\\' escchar   { cval = $2; strings[nstr][c] = '\\'; strings[nstr][c+1] = cval; c=c+2; cval=-1; }
  ;

ws: '\t' '\n' '\r' ;

terminal:	terminalfw
				| terminalnofw
				;

terminalfw: '\'' fwi '\'' ;

terminalnofw: '\'' termval '\'' ;

fwi : endianness '(' sign number ',' ows type size')' {if (neg) decimal = 0-decimal; fixed_width(); fixedW=true;};

type: 'u' 'i' 'n' 't'    {unsign=true;}
    | 'i' 'n' 't'        {unsign=false;}
    ;

ows: /* empty */
	 | ' '			 
	 ;

size: '1' '6' {size=16;}
    | '3' '2' {size=32;}
    | '6' '4' {size=64;}								
    ;

sign: '-'       {neg=true;}
    |/*empty */ {neg=false;};

number: digit        {decimal += $1 - '0' ;}
      | number digit {decimal*= 10; decimal += $2 - '0' ;}
      ;

endianness: le {le=true;}
					| be {be=true;}
					;

le: 'l' 'i' 't' 't' 'l' 'e' '_' 'e' 'n' 'd' 'i' 'a' 'n' ;

be: 'b' 'i' 'g' '_' 'e' 'n' 'd' 'i' 'a' 'n' ;

termval : charval | hexval ;

charval: alphanumeric       { cval = $1; if(!ranging) fprintf(xout,"\'%c\'",(char)$1); }
       | punct              { cval = $1; if(!ranging) fprintf(xout,"\'%c\'",(char)$1); }
       | '"'                { cval = $1; if(!ranging) fprintf(xout,"\'%c\'",(char)$1); }
       | '\\' escchar       { cval = $2; if(!ranging) fprintf(xout,"\'\\%c\'",(char)$2); }
			 ;

alphanumeric: uchar | lchar | digit ;

hexval: '\\' 'x' { hval=0; } hexdigit { hval=16*hval; } hexdigit { hexout(); } ;

hexdigit: digit             { hval += $1 - '0'; }
        | uhex              { hval += $1 - 'A' + 10; }
        | lhex              { hval += $1 - 'a' + 10; }
        ;

nonterminal : symbolchars {Xnonterminals++; Bnonterminals++;};
symbolchars : symbolchar | symbolchars symbolchar ;

symbolchar : alphanumeric   { fprintf(xout,"%c",(char)$1); }
           | '_'            { fprintf(xout,"_"); }
					 | '.'            { fprintf(xout,"."); }
					 ;

range : '[' { rbegin(); } elements ']' {Bnonterminals++;} 
      | '*' { fprintf(xout,"r__0"); anybyte=true; Xnonterminals++;Bnonterminals++;}
      ;

elements : ws0 terminalnofw ws0 { setrlow(); } '-' ws0 terminalnofw ws0 { setrhigh(); }
         |  enumeration { eend(); }
         ;

enumeration : ws0 terminalnofw ws0 { setenum0(); } | enumeration ',' ws0 terminal ws0 { setnextenum(); } ;

/* echo comments */
comment: '/' '*' { fprintf(xout,"/*"); } commentchars '*' '/' { fprintf(xout,"*/"); }
       ;
commentchars : commentchar | commentchars commentchar ;
commentchar : alphanumeric      { fprintf(xout,"%c",(char)$1); }
            | '*' { fprintf(xout,"%c",(char)$1); }
            | '(' { fprintf(xout,"%c",(char)$1); }
            | ')' { fprintf(xout,"%c",(char)$1); }
            | '-' { fprintf(xout,"%c",(char)$1); }
            | ',' { fprintf(xout,"%c",(char)$1); } 
            | '\''{ fprintf(xout,"%c",(char)$1); }
            | '_' { fprintf(xout,"%c",(char)$1); } 
            | wschar /* already output */
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

