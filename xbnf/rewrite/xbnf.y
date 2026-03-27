%{

#include <stdio.h>
#include <stdbool.h>

/* native bison functions */
int yylex(void);
void yyerror(char *s);

/* functions defined in xbnftobison.c */
extern void init(void);
extern void hex_out(void);
extern void char_out(void);
extern void set_r_low(void);
extern void set_r_high(void);
extern void add_rules(void);
extern void free_mem(void);

extern void e_start(void);
extern void set_next_enum(void);
extern void e_end(void);

extern void s_begin(void);
extern void s_add(void);
extern void s_end(void);
extern bool in_string;

/* variables defined in xbnftobison.c */
extern int line_num;         /* current line number used for error reporting */
extern int val;
extern bool any_byte;
extern bool ranging;

/* variables defined in xbnf_tb.c */
extern FILE *xout;          /* output bison file */
extern int yylval;          /* value of token read in */

%}

%token X00

%%

bnf: { init(); } rules ws0 { add_rules(); free_mem(); }
   ;

rules: rule
     | rules rule
     ;

rule: ws0 nonterminal ws0 ':' { fprintf(xout, ":"); } rhs ';' { fprintf(xout, ";\n\n"); }
    ;

rhs: terms ws1
   | rhs '|' { fprintf(xout, "\n  |" ); } terms ws1
   ;

terms: /* empty */          { fprintf(xout, " "); }
     | terms ws1 term       { fprintf(xout, " "); }
     ;

term: terminal
    | nonterminal
    | comment
    | range
    | string
    ;

string: '"' letters '"'             { s_end(); }
      ;

letters: charval                  { s_begin(); }
       | letters charval          { s_add(); }
       ;

terminal: '\'' termval '\''
        ;

termval: hexval                 { hex_out(); }
       | charval                { char_out(); }
       ;

charval: alphanumeric       { val = $1; }
       | punct              { val = $1; }
       | '\\' escchar       { val = $2; }
       ;

hexval: '\\' 'x' { val = 0; } hexdigit { val = 16*val; } hexdigit
      ;

hexdigit: digit             { val += $1 - '0'; }
        | uhex              { val += $1 - 'A' + 10; }
        | lhex              { val += $1 - 'a' + 10; }
        ;

nonterminal: symbolchars
           ;

symbolchars: symbolchar                 { fprintf(xout, "%c", (char)$1); }
           | symbolchars symbolchar     { fprintf(xout, "%c", (char)$2); }
           ;

symbolchar: alphanumeric
          | '_'
          | '.'
          ;

alphanumeric: uchar
            | lchar
            | digit
            ;

range: '[' { ranging = true; } elements ']'
     | '*' { fprintf(xout, "r__0"); any_byte = true; }
     ;

elements: ws0 terminal ws0 { set_r_low(); } '-' ws0 terminal ws0 { set_r_high(); }
        | enumeration { e_end(); }
        ;

enumeration: ws0 terminal ws0 { e_start(); }
           | enumeration ',' ws0 terminal ws0 { set_next_enum(); }
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
wschar: ' '
      | '\n'        { line_num++; }
      | '\t'
      | '\r'
      ;
