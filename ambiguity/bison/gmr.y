%{
  #define YYDEBUG 1
  int yylex(void);
  void yyerror(char *s);
%}
%token X00
%%

alt:
   star_alt_prefix con
   ;

star_alt_prefix:
               /* empty */
               | star_alt_prefix alt_prefix
               ;

alt_prefix:
          con '/' star_wsp
          | con one_star_wsp '/' star_wsp
          ;

con: CHAR
   | con one_star_wsp CHAR
   ;

star_wsp:
        /* empty */
        | star_wsp ' '
        ;

one_star_wsp:
            ' '
            | one_star_wsp ' '
            ;

CHAR:
    'a' | 'b' | 'c' | 'd' | 'e'
    ;


