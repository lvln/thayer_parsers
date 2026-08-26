%{

#define YYDEBUG 1

/* native bison functions */
int yylex(void);
void yyerror(char *s);

%}

%token X00

%%

bnf:
   rules ws0
   ;

rules:
     rule
     | rules rule
     ;

rule:
    ws0 nonterminal ws0 ':' rhs ';'
    | ws0 comment
    ;

rhs:
   elements ws1
   | rhs '|' elements ws1
   ;

elements:
        /* empty */
        | elements ws1 element
        ;

element:
       terminal
       | nonterminal
       | comment
       ;

terminal:
        '\'' charval '\''
        | '\'' hexval '\''
        | string
        | collection
        | anybyte
        | fwi
        ;

string:
      '"' letters '"'
      ;

letters:
       c
       | letters c
       ;

c:
 alphanumeric
 | wschar
 | '\\' escchar
 ;

charval:
       alphanumeric
       | punct
       | '"'
       | '\\' escchar
       ;

alphanumeric:
            uchar
            | lchar
            | digit
            ;

hexval:
      '\\' 'x' hexdigit hexdigit
      ;

hexdigit:
        digit
        | uhex 
        | lhex
        ;

nonterminal:
           'A' nonterminal_chars_star
           | 'B' nonterminal_chars_star
           | 'C' nonterminal_chars_star
           | 'D' nonterminal_chars_star
           | 'E' nonterminal_chars_star
           | 'F' nonterminal_chars_star
           | 'G' nonterminal_chars_star
           | 'H' nonterminal_chars_star
           | 'I' nonterminal_chars_star
           | 'J' nonterminal_chars_star
           | 'K' nonterminal_chars_star
           | 'L' nonterminal_chars_star
           | 'M' nonterminal_chars_star
           | 'N' nonterminal_chars_star
           | 'O' nonterminal_chars_star
           | 'P' nonterminal_chars_star
           | 'Q' nonterminal_chars_star
           | 'R' nonterminal_chars_star
           | 'S' nonterminal_chars_star
           | 'T' nonterminal_chars_star
           | 'U' nonterminal_chars_star
           | 'V' nonterminal_chars_star
           | 'W' nonterminal_chars_star
           | 'X' nonterminal_chars_star
           | 'Y' nonterminal_chars_star
           | 'Z' nonterminal_chars_star
           | 'a' nonterminal_chars_star
           | 'b' nonterminal_chars_star
           | 'c' nonterminal_chars_star
           | 'd' nonterminal_chars_star
           | 'e' nonterminal_chars_star
           | 'f' nonterminal_chars_star
           | 'g' nonterminal_chars_star
           | 'h' nonterminal_chars_star
           | 'i' nonterminal_chars_star
           | 'j' nonterminal_chars_star
           | 'k' nonterminal_chars_star
           | 'l' nonterminal_chars_star
           | 'm' nonterminal_chars_star
           | 'n' nonterminal_chars_star
           | 'o' nonterminal_chars_star
           | 'p' nonterminal_chars_star
           | 'q' nonterminal_chars_star
           | 'r' nonterminal_chars_star
           | 's' nonterminal_chars_star
           | 't' nonterminal_chars_star
           | 'u' nonterminal_chars_star
           | 'v' nonterminal_chars_star
           | 'w' nonterminal_chars_star
           | 'x' nonterminal_chars_star
           | 'y' nonterminal_chars_star
           | 'z' nonterminal_chars_star
           | '_' nonterminal_chars_star
           ;

nonterminal_chars_star:
                      /* empty */
                      | 'A' nonterminal_chars_star
                      | 'B' nonterminal_chars_star
                      | 'C' nonterminal_chars_star
                      | 'D' nonterminal_chars_star
                      | 'E' nonterminal_chars_star
                      | 'F' nonterminal_chars_star
                      | 'G' nonterminal_chars_star
                      | 'H' nonterminal_chars_star
                      | 'I' nonterminal_chars_star
                      | 'J' nonterminal_chars_star
                      | 'K' nonterminal_chars_star
                      | 'L' nonterminal_chars_star
                      | 'M' nonterminal_chars_star
                      | 'N' nonterminal_chars_star
                      | 'O' nonterminal_chars_star
                      | 'P' nonterminal_chars_star
                      | 'Q' nonterminal_chars_star
                      | 'R' nonterminal_chars_star
                      | 'S' nonterminal_chars_star
                      | 'T' nonterminal_chars_star
                      | 'U' nonterminal_chars_star
                      | 'V' nonterminal_chars_star
                      | 'W' nonterminal_chars_star
                      | 'X' nonterminal_chars_star
                      | 'Y' nonterminal_chars_star
                      | 'Z' nonterminal_chars_star
                      | 'a' nonterminal_chars_star
                      | 'b' nonterminal_chars_star
                      | 'c' nonterminal_chars_star
                      | 'd' nonterminal_chars_star
                      | 'e' nonterminal_chars_star
                      | 'f' nonterminal_chars_star
                      | 'g' nonterminal_chars_star
                      | 'h' nonterminal_chars_star
                      | 'i' nonterminal_chars_star
                      | 'j' nonterminal_chars_star
                      | 'k' nonterminal_chars_star
                      | 'l' nonterminal_chars_star
                      | 'm' nonterminal_chars_star
                      | 'n' nonterminal_chars_star
                      | 'o' nonterminal_chars_star
                      | 'p' nonterminal_chars_star
                      | 'q' nonterminal_chars_star
                      | 'r' nonterminal_chars_star
                      | 's' nonterminal_chars_star
                      | 't' nonterminal_chars_star
                      | 'u' nonterminal_chars_star
                      | 'v' nonterminal_chars_star
                      | 'w' nonterminal_chars_star
                      | 'x' nonterminal_chars_star
                      | 'y' nonterminal_chars_star
                      | 'z' nonterminal_chars_star
                      | '0' nonterminal_chars_star
                      | '1' nonterminal_chars_star
                      | '2' nonterminal_chars_star
                      | '3' nonterminal_chars_star
                      | '4' nonterminal_chars_star
                      | '5' nonterminal_chars_star
                      | '6' nonterminal_chars_star
                      | '7' nonterminal_chars_star
                      | '8' nonterminal_chars_star
                      | '9' nonterminal_chars_star
                      | '_' nonterminal_chars_star
                      | '.' nonterminal_chars_star
                      | '-' nonterminal_chars_star
                      ;

collection:
          '[' range ']'
          | '[' enumeration ']'
          ;

range:
     ws0 collection_element ws0 '-' ws0 collection_element ws0
     ;

enumeration:
           ws0 collection_element ws0 | enumeration ',' ws0 collection_element ws0
           ;

collection_element:
                  hexval | charval | fwi
                  ;

anybyte:
       '*'
       ;

fwi:
   uint8 | int8 | uint16 | int16 | uint32 | int32 | uint64 | int64
   ;

uint8:
     'u' '8' '(' number ')'
     | 'U' '8' '(' number ')'
     ;

int8:
    'i' '8' '(' number ')'
    | 'I' '8' '(' number ')'
    ;

uint16:
      'u' '1' '6' '(' number ')'
      | 'U' '1' '6' '(' number ')'
      ;

int16:
     'i' '1' '6' '(' number ')'
     | 'I' '1' '6' '(' number ')'
     ;

uint32:
      'u' '3' '2' '(' number ')'
      | 'U' '3' '2' '(' number ')'
      ;

int32:
     'i' '3' '2' '(' number ')'
     | 'I' '3' '2' '(' number ')'
     ;

uint64:
      'u' '6' '4' '(' number ')'
      | 'U' '6' '4' '(' number ')'
      ;

int64:
     'i' '6' '4' '(' number ')'
     | 'I' '6' '4' '(' number ')'
     ;

number:
      digit | number digit
      ;

comment:
       '/' '*' commentchars '*' '/' 
       ;

commentchars:
            commentchar
            | commentchars commentchar
            ;

commentchar:
           alphanumeric
           | '*'
           | '('
           | ')'
           | '-'
           | ','
           | '\''
           | '_' 
           | wschar
           ;

uchar:
     uhex | 'G' | 'H' | 'I' | 'J' 
     | 'K' | 'L' | 'M' | 'N' | 'O' | 'P' | 'Q' | 'R' | 'S' | 'T' 
     | 'U' | 'V' | 'W' | 'X' | 'Y' | 'Z'
     ;

lchar:
     lhex | 'g' | 'h' | 'i' | 'j' 
     | 'k' | 'l' | 'm' | 'n' | 'o' | 'p' | 'q' | 'r' | 's' | 't' 
     | 'u' | 'v' | 'w' | 'x' | 'y' | 'z'
     ;

uhex: 'A' | 'B' | 'C' | 'D' | 'E' | 'F'
    ;

lhex: 'a'| 'b' | 'c' | 'd' | 'e' | 'f'
    ;

punct:
     '!' | '#' | '$' | '%' | '&' | '\'' | '(' | ')' | '*' | '+' 
     | ',' | '-' | '.' | '/' | ':' | ';' | '<' | '=' | '>' | '?' | '@'
     | '[' | ']' | '^' | '_' | '`' | '{' | '|' | '}' | '~'
     ;

escchar:
       'b' | 'f' | 'n' | 'r' | 't' | '"' | '\\' | '/' | 'v' | '?' | 'a' | 'e'
       ;

digit:
     '0' | onenine
     ;

onenine:
       '1' | '2' | '3' | '4' | '5'| '6' | '7' | '8' | '9'
       ;


ws0:
   /* empty */ | ws1
   ;

ws1:
   wschar | ws1 wschar
   ;

wschar: 
      ' '
      | '\n' 
      | '\t'
      | '\r'
      ;
