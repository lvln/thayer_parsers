%{
	#define YYDEBUG 1
	int yylex(void);
	void yyerror(const char *s);
%}

%token X00

/* generate a GLR parser to handle whitespace ambiguity */
%glr-parser
%expect 8
%expect-rr 4

%%

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                         *
 *                      ABNF GRAMMAR                       *
 *                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

rulelist:
        one_star_rulelist_elemt
        ;

rule:
    rulename defined_as elements c_nl
    ;

rulename:
        ALPHA star_alpha_digit_dash
        ;

defined_as:
          star_c_wsp eq_option star_c_wsp
          ;

elements:
        alternation star_c_wsp
        ;

c_wsp:
     WSP | c_nl WSP
     ;

c_nl:
    comment | CRLF
    ;

comment:
       ';' star_wsp_or_vchar CRLF
       ;

alternation:
           concatenation star_alternation_elemt
           ;

concatenation:
             repetition star_concatenation_elemt
             ;

repetition:
          opt_repeat element
          ;

repeat:
      one_star_digit | star_digit '*' star_digit
      ;

element:
       group | option | char_val | num_val | prose_val
       ;

group:
     '(' star_c_wsp alternation star_c_wsp ')'
     ;

option:
      '[' star_c_wsp alternation star_c_wsp ']'
      ;

char_val:
        DQUOTE star_char_val_char DQUOTE
        ;

num_val:
       '%' num_val_ch
       ;

bin_val:
       'b' one_star_bit opt_bin_val
       ;

dec_val:
       'd' one_star_digit opt_dec_val
       ;

hex_val:
       'x' one_star_hexdig opt_hex_val
       ;

prose_val:
         '<' star_prose_val_char '>'
         ;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                         *
 *                      HELPER RULES                       *
 *                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* rulelist */

one_star_rulelist_elemt:
                       rulelist_elemt | one_star_rulelist_elemt rulelist_elemt
                       ;

rulelist_elemt:
              rule | star_c_wsp_c_nl
              ;

star_c_wsp_c_nl:
               star_c_wsp c_nl
               ;

/* rulename */

star_alpha_digit_dash:
                     /* empty */ | star_alpha_digit_dash alpha_digit_dash
                     ;

alpha_digit_dash:
                ALPHA | DIGIT | '-'
                ;

/* defined_as */

eq_option :
          '=' | '=' '/'
          ;

/* char_val */

star_char_val_char:
                  /* empty */ | star_char_val_char char_val_char
                  ;

char_val_char:
             '\x20' | '\x21' | '\x23' | '\x24' | '\x25' | '\x26' | '\x27' | '\x28' | '\x29' | '\x2A' |
             '\x2B' | '\x2C' | '\x2D' | '\x2E' | '\x2F' | '\x30' | '\x31' | '\x32' | '\x33' | '\x34' |
             '\x35' | '\x36' | '\x37' | '\x38' | '\x39' | '\x3A' | '\x3B' | '\x3C' | '\x3D' | '\x3E' |
             '\x3F' | '\x40' | '\x41' | '\x42' | '\x43' | '\x44' | '\x45' | '\x46' | '\x47' | '\x48' |
             '\x49' | '\x4A' | '\x4B' | '\x4C' | '\x4D' | '\x4E' | '\x4F' | '\x50' | '\x51' | '\x52' |
             '\x53' | '\x54' | '\x55' | '\x56' | '\x57' | '\x58' | '\x59' | '\x5A' | '\x5B' | '\x5C' |
             '\x5D' | '\x5E' | '\x5F' | '\x60' | '\x61' | '\x62' | '\x63' | '\x64' | '\x65' | '\x66' |
             '\x67' | '\x68' | '\x69' | '\x6A' | '\x6B' | '\x6C' | '\x6D' | '\x6E' | '\x6F' | '\x70' |
             '\x71' | '\x72' | '\x73' | '\x74' | '\x75' | '\x76' | '\x77' | '\x78' | '\x79' | '\x7A' |
             '\x7B' | '\x7C' | '\x7D' | '\x7E'
             ;

/* comment */

star_wsp_or_vchar:
                 /* empty */ | star_wsp_or_vchar wsp_or_vchar
                 ;

wsp_or_vchar:
            VCHAR | WSP
            ;

/* alternation */

star_alternation_elemt:
                      /* empty */ | star_alternation_elemt alternation_elemt
                      ;

alternation_elemt:
                 star_c_wsp '/' star_c_wsp concatenation
                 ;

/* concatenation */

star_concatenation_elemt:
                        /* empty */ | star_concatenation_elemt concatenation_elemt
                        ;

concatenation_elemt:
                   one_star_c_wsp repetition
                   ;

/* repetition */

opt_repeat:
          /* empty */ | repeat
          ;

/* num_val */

num_val_ch:
          bin_val | dec_val | hex_val
          ;

/* bin_val */

opt_bin_val:
           /* empty */ | one_star_dot_bit | one_star_dash_bit
           ;

one_star_dot_bit:
                '.' one_star_bit | one_star_dot_bit '.' one_star_bit
                ;

one_star_dash_bit:
                 '-' one_star_bit | one_star_dash_bit '.' one_star_bit
                 ;

/* dec_val */

opt_dec_val:
           /* empty */ | one_star_dot_digit | one_star_dash_digit
           ;

one_star_dot_digit:
                  '.' one_star_digit | one_star_dot_digit '.' one_star_digit
                  ;

one_star_dash_digit:
                   '-' one_star_digit | one_star_dash_digit '.' one_star_digit
                   ;

/* hex_val */

opt_hex_val:
           /* empty */ | one_star_dot_hexdig | one_star_dash_hexdig
           ;

one_star_dot_hexdig:
                   '.' one_star_hexdig | one_star_dot_hexdig '.' one_star_hexdig
                   ;

one_star_dash_hexdig:
                    '-' one_star_hexdig | one_star_dash_hexdig '.' one_star_hexdig
                    ;

/* prose_val */

star_prose_val_char:
                   /* empty */ | prose_val_char
                   ;

prose_val_char:
              '\x20' | '\x21' | '\x22' | '\x23' | '\x24' | '\x25' | '\x26' | '\x27' | '\x28' | '\x29' |
              '\x2A' | '\x2B' | '\x2C' | '\x2D' | '\x2E' | '\x2F' | '\x30' | '\x31' | '\x32' | '\x33' |
              '\x34' | '\x35' | '\x36' | '\x37' | '\x38' | '\x39' | '\x3A' | '\x3B' | '\x3C' | '\x3D' |
              '\x3F' | '\x40' | '\x41' | '\x42' | '\x43' | '\x44' | '\x45' | '\x46' | '\x47' | '\x48' |
              '\x49' | '\x4A' | '\x4B' | '\x4C' | '\x4D' | '\x4E' | '\x4F' | '\x50' | '\x51' | '\x52' |
              '\x53' | '\x54' | '\x55' | '\x56' | '\x57' | '\x58' | '\x59' | '\x5A' | '\x5B' | '\x5C' |
              '\x5D' | '\x5E' | '\x5F' | '\x60' | '\x61' | '\x62' | '\x63' | '\x64' | '\x65' | '\x66' |
              '\x67' | '\x68' | '\x69' | '\x6A' | '\x6B' | '\x6C' | '\x6D' | '\x6E' | '\x6F' | '\x70' |
              '\x71' | '\x72' | '\x73' | '\x74' | '\x75' | '\x76' | '\x77' | '\x78' | '\x79' | '\x7A' |
              '\x7B' | '\x7C' | '\x7D' | '\x7E'
              ;

/* repetitions of basic and shared rules */

star_digit:
          /* empty */ | one_star_digit          /* expressed atypically to remove shift/reduce conflicts */
          ;

one_star_digit:
              DIGIT | one_star_digit DIGIT
              ;

one_star_hexdig:
               HEXDIG | one_star_hexdig HEXDIG
               ;

one_star_bit:
            BIT | one_star_bit BIT
            ;

star_c_wsp:
          /* empty */ | star_c_wsp c_wsp
          ;

one_star_c_wsp:
              c_wsp | one_star_c_wsp c_wsp
              ;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                         *
 *                       BASIC RULES                       *
 *                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ALPHA:
     '\x41' | '\x42' | '\x43' | '\x44' | '\x45' | '\x46' | '\x47' | '\x48' | '\x49' | '\x4A' |
     '\x4B' | '\x4C' | '\x4D' | '\x4E' | '\x4F' | '\x50' | '\x51' | '\x52' | '\x53' | '\x54' |
     '\x55' | '\x56' | '\x57' | '\x58' | '\x59' | '\x5A' | '\x61' | '\x62' | '\x63' | '\x64' |
     '\x65' | '\x66' | '\x67' | '\x68' | '\x69' | '\x6A' | '\x6B' | '\x6C' | '\x6D' | '\x6E' |
     '\x6F' | '\x70' | '\x71' | '\x72' | '\x73' | '\x74' | '\x75' | '\x76' | '\x77' | '\x78' |
     '\x79' | '\x7A'
     ;

BIT:
   '0' | '1'
   ;

CR:
  '\x0D'

CRLF:
    CR LF
    ;

DIGIT:
     '\x30' | '\x31' | '\x32' | '\x33' | '\x34' | '\x35' | '\x36' | '\x37' | '\x38' | '\x39'
     ;

DQUOTE:
      '\x22'
      ;

HEXDIG:
      DIGIT | 'A' | 'B' | 'C' | 'D' | 'E' | 'F'
      ;

HTAB:
    '\x09'
    ;

LF:
  '\x0A'
  ;

SP:
  '\x20'
  ;

VCHAR:
     '\x21' | '\x22' | '\x23' | '\x24' | '\x25' | '\x26' | '\x27' | '\x28' | '\x29' | '\x2A' |
     '\x2B' | '\x2C' | '\x2D' | '\x2E' | '\x2F' | '\x30' | '\x31' | '\x32' | '\x33' | '\x34' |
     '\x35' | '\x36' | '\x37' | '\x38' | '\x39' | '\x3A' | '\x3B' | '\x3C' | '\x3D' | '\x3E' |
     '\x3F' | '\x40' | '\x41' | '\x42' | '\x43' | '\x44' | '\x45' | '\x46' | '\x47' | '\x48' |
     '\x49' | '\x4A' | '\x4B' | '\x4C' | '\x4D' | '\x4E' | '\x4F' | '\x50' | '\x51' | '\x52' |
     '\x53' | '\x54' | '\x55' | '\x56' | '\x57' | '\x58' | '\x59' | '\x5A' | '\x5B' | '\x5C' |
     '\x5D' | '\x5E' | '\x5F' | '\x60' | '\x61' | '\x62' | '\x63' | '\x64' | '\x65' | '\x66' |
     '\x67' | '\x68' | '\x69' | '\x6A' | '\x6B' | '\x6C' | '\x6D' | '\x6E' | '\x6F' | '\x70' |
     '\x71' | '\x72' | '\x73' | '\x74' | '\x75' | '\x76' | '\x77' | '\x78' | '\x79' | '\x7A' |
     '\x7B' | '\x7C' | '\x7D' | '\x7E'
     ;

WSP:
   SP | HTAB
   ;

%%
