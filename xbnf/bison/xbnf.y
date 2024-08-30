%{

#include <stdio.h>
#include <stdbool.h>

#define YYDEBUG 1
#define MAXENTRY 100						             /* max number of separate ranges */
#define CHARMAX 50                           /* max number of characters in a string */
#define MAXNUM 100                           /* max number of digits in number */

/* native bison functions */
int yylex(void);
void yyerror(char *s);

/* functions defined in xbnftobison.c */
extern void init(void);                       /* initialises all variables */
extern void rbegin(void);                     /* begins ranging */
extern void setrlow(void);                    /* sets low bound on range */
extern void setrhigh(void);                   /* sets high bound on range */  
extern void setenum0(void);                   /* sets first number in enumeration */
extern void setnextenum(void);                /* sets second through n numbers in enumeration */
extern void eend(void);                       /* ends enumeration */
extern void fixed_width(void);                /* decodes a fixed width integer into hex */
extern void check_index(void);                /* checks to make sure fixed-width integer input is not too long */
extern void hexout(void);                     /* prints out hex values */
extern void addrules(void);                   /* adds rules for ranges, strings and enumrations */

/* variables defined in xbnf_tb.c */
extern FILE* xout;                             /* output bison file */
extern int yylval;                             /* value of token read in */

/* variables defined in xbnftobison.c */
extern int linenum ;                           /* line number - used for error reporting */
extern int hval;                               /* variable used to form a hex value from ascii char */
extern int cval;                               /* variable used to hold a character value */
extern bool anybyte;                           /* true if anybytes is used in the grammar */
extern bool ranging;                           /* true if range */
extern bool fixedwidth;                        /* true if currently dealing with a fixed width integer */
extern int strings[MAXENTRY][CHARMAX];         /* any strings that are found within rules */
extern int c;                                  /* variable used to hold character value for a string */
extern int nstr;                               /* number of strings */
extern char decimalstr[2][MAXNUM];             /* string to read number into */
extern int nnum;                               /* how many numbers have been read in as strings */
extern int i;                                  /* index variable for number string */
extern int size;                               /* size of fixed-width integer */
extern bool neg;                               /* true if negative sign present at start of fixed-witdht integer */
extern bool unsign;                            /* true if fixed-width integer is unsigned */
extern bool le;                                /* true if fixed-width integer is of little-endian representation */
extern bool be;                                /* true if fixed-width integer is network byte order */

%}	
	
%token X00
		 
%%

bnf: { init(); } rules ws0 { addrules(); }
 
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
  | '\\' escchar   { cval = $2; strings[nstr][c] = '\\'; strings[nstr][c+1] = cval; c = c + 2; cval = -1; }
  ;

ws: '\t' '\n' '\r'
	;

terminal: '\'' termval '\''
				;

fwi: type sizebe '(' sign fullnumber ')'         {be = true; fixedwidth = true; fixed_width(); }
   | le '(' sign fullnumber ws0 ',' ws0 type sizele ')'  {le = true; fixedwidth = true; fixed_width(); }
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
comment: '/' '*' { fprintf(xout, "/*"); } commentchars '*' '/' { fprintf(xout, "*/"); }
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
