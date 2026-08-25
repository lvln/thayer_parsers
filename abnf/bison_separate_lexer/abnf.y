/*
 * abnf.y -- ABNF grammar
 *
 * Author: Josh Meise
 * Date: 08-21-2026
 *
 */

%{

extern int yylex(void);

void yyerror(char*);

%}

%start alternation

%token RULENAME REPEAT QUOTED_STRING BIN_VAL DEC_VAL HEX_VAL PROSE_VAL

%%

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                         *
 *                      ABNF GRAMMAR                       *
 *                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

rulelist:
        rule | rulelist rule
        ;

rule:
    RULENAME defined_as elements
    ;

defined_as:
          '=' | '=' '/'
          ;

elements:
        alternation
        ;

alternation:
           concatenation | alternation '/' concatenation
           ;

concatenation:
             repetition | concatenation repetition
             ;

repetition:
          opt_repeat element
          ;

opt_repeat:
          /* empty */ | REPEAT
          ;

element:
       RULENAME | char_val | num_val | PROSE_VAL
       ;

char_val:
        case_insensitive_string | case_sensitive_string
        ;

case_insensitive_string:
                       opt_percent_i QUOTED_STRING
                       ;

opt_percent_i:
             /* empty */ | '%' 'i'
             ;

case_sensitive_string:
                     '%' 's' QUOTED_STRING
                     ;

num_val:
       '%' num_val_ch
       ;

num_val_ch:
          BIN_VAL | DEC_VAL | HEX_VAL
          ;

%%
