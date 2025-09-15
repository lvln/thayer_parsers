%{

#include <stdio.h>
#include <stdbool.h>

/* native bison functions */
int yylex(void);
void yyerror(char *s);

/* functions defined in xbnftobison.c */
extern void init(void);
extern void hexout(void);
extern void rbegin(void);
extern void setrlow(void);
extern void setrhigh(void);
extern void addrules(void);
extern void free_mem(void);

/* variables defined in xbnftobison.c */
extern int linenum;         /* current line number used for error reporting */
extern int hval;            /* hexadecimal value of ascii character */
extern int cval;            /* holds character value */
extern bool anybyte;
extern bool ranging;

/* variables defined in xbnf_tb.c */
extern FILE *xout;          /* output bison file */
extern int yylval;          /* value of token read in */

%}

%token X00

%%

bnf: { init(); } rules ws0 { addrules(); free_mem(); }
   ;

rules: rule
     | rules rule
     ;

rule: ws0 nonterminal ws0 ':' { fprintf(xout, ":"); } rhs ';' { fprintf(xout, ";"); }
    ;

rhs: terms ws1
   | rhs '|' { fprintf(xout, "|" ); } terms ws1
   ;

terms: /* empty */
     | terms ws1 term
     ;

term: terminal
    | nonterminal
    | comment
    | range
    ;

terminal: '\'' termval '\''
        ;

termval: hexval
       | charval
       ;

charval: alphanumeric       { cval = $1; if (!ranging) fprintf(xout, "\'%c\'", (char)$1); }
       | punct              { cval = $1; if (!ranging) fprintf(xout, "\'%c\'", (char)$1); }
       | '"'                { cval = $1; if (!ranging) fprintf(xout, "\'%c\'", (char)$1); }
       | '\\' escchar       { cval = $2; if (!ranging) fprintf(xout, "\'\\%c\'", (char)$2); }
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

symbolchar: alphanumeric    { fprintf(xout, "%c", (char)$1); }
          | '_'             { fprintf(xout, "_"); }
          | '.'             { fprintf(xout, "."); }
          ;

alphanumeric: uchar
            | lchar
            | digit
            ;

range: '[' { rbegin(); } elements ']'
     | '*' { fprintf(xout, "r__0"); anybyte = true; }
     ;

elements: ws0 terminal ws0 { setrlow(); } '-' ws0 terminal ws0 { setrhigh(); }
        ;

/* echo comments */
comment: '/' '*' { fprintf(xout, "/*"); } commentchars '*' '/' { fprintf(xout, "*/"); }

commentchars: commentchar
            | commentchars commentchar
            ;

commentchar: alphanumeric       { fprintf(xout, "%c", (char)$1); }
           | '*'                { fprintf(xout, "%c", (char)$1); }
           | '('                { fprintf(xout, "%c", (char)$1); }
           | ')'                { fprintf(xout, "%c", (char)$1); }
           | '-'                { fprintf(xout, "%c", (char)$1); }
           | ','                { fprintf(xout, "%c", (char)$1); }
           | '\''               { fprintf(xout, "%c", (char)$1); }
           | '_'                { fprintf(xout, "%c", (char)$1); }
           | wschar             /* already written to file */
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

punct: ' ' | '!' | '#' | '$' | '%' | '&' | '\'' | '(' | ')' | '*' | '+'
     | ',' | '-' | '.' | '/' | ':' | ';' | '<' | '=' | '>' | '?' | '@'
     | '[' | ']' | '^' | '_' | '`' | '{' | '|' | '}' | '~'
     ;

escchar: 'b' | 'f' | 'n' | 'r' | 't' | '\"' | '\\' | '/' | 'v' | '?' | 'a' | 'e'
       ;

digit: '0' | onenine
     ;

onenine: '1' | '2' | '3' | '4' | '5'| '6' | '7' | '8' | '9'
       ;

ws0: /* empty */
   | ws1
   ;

ws1: wschar
   | ws1 wschar
   ;

/* echo whitespace */
wschar: ' '         { fprintf(xout, " "); }
      | '\n'        { linenum++; fprintf(xout, "\n"); }
      | '\t'        { fprintf(xout, "\t"); }
      | '\r'        { fprintf(xout, "\r"); }
      ;
