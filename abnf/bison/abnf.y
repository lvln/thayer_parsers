%{
	#define YYDEBUG 1
	int yylex(void);
	void yyerror(const char *s);
%}

%token X00

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
        alternation star_wsp                                    /* changed from star_c_wsp to star_wsp in accordance with erratum 2968 */
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
      one_star_digit | '*' star_digit | one_star_digit '*' star_digit       /* expressed atypically to remove shift/reduce conflict */
      ;

element:
       rulename | group | option | char_val | num_val | prose_val
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

uint8:
     'u' '8' '_' one_star_digit opt_uint8_val
     ;

int8:
    'i' '8' '_' one_star_digit opt_int8_val
    | 'i' '8' '_' '-' one_star_digit opt_int8_val
    ;

uint16_le:
         'u' '1' '6' '_' 'l' 'e' '_' one_star_digit opt_uint16_val
         ;

int16_le:
        'i' '1' '6' '_' 'l' 'e' '_' one_star_digit opt_int16_val
        | 'i' '1' '6' '_' 'l' 'e' '_' '-' one_star_digit opt_int16_val
        ;

uint16_be:
         'u' '1' '6' '_' 'b' 'e' '_' one_star_digit opt_uint16_val
         ;

int16_be:
        'i' '1' '6' '_' 'b' 'e' '_' one_star_digit opt_int16_val
        | 'i' '1' '6' '_' 'b' 'e' '_' '-' one_star_digit opt_int16_val
        ;

uint32_le:
         'u' '3' '2' '_' 'l' 'e' '_' one_star_digit opt_uint32_val
         ;

int32_le:
        'i' '3' '2' '_' 'l' 'e' '_' one_star_digit opt_int32_val
        | 'i' '3' '2' '_' 'l' 'e' '_' '-' one_star_digit opt_int32_val
        ;

uint32_be:
         'u' '3' '2' '_' 'b' 'e' '_' one_star_digit opt_uint32_val
         ;

int32_be:
        'i' '3' '2' '_' 'b' 'e' '_' one_star_digit opt_int32_val
        | 'i' '3' '2' '_' 'b' 'e' '_' '-' one_star_digit opt_int32_val
        ;

uint64_le:
         'u' '6' '4' '_' 'l' 'e' '_' one_star_digit opt_uint64_val
         ;

int64_le:
        'i' '6' '4' '_' 'l' 'e' '_' one_star_digit opt_int64_val
        | 'i' '6' '4' '_' 'l' 'e' '_' '-' one_star_digit opt_int64_val
        ;

uint64_be:
         'u' '6' '4' '_' 'b' 'e' '_' one_star_digit opt_uint64_val
         ;

int64_be:
        'i' '6' '4' '_' 'b' 'e' '_' one_star_digit opt_int64_val
        | 'i' '6' '4' '_' 'b' 'e' '_' '-' one_star_digit opt_int64_val
        ;

float_le:
        'f' '_' 'l' 'e' '_' one_star_digit '.' one_star_digit opt_float_val
        | 'f' '_' 'l' 'e' '_' '-' one_star_digit '.' one_star_digit opt_float_val
        ;

float_be:
        'f' '_' 'b' 'e' '_' one_star_digit '.' one_star_digit opt_float_val
        | 'f' '_' 'b' 'e' '_' '-' one_star_digit '.' one_star_digit opt_float_val
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
              rule | star_wsp_c_nl                              /* changed from star_c_wsp_c_nl to star_wsp_c_nl in accordance with erratum 3076 */
              ;

star_wsp_c_nl:
             star_wsp c_nl
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
          bin_val | dec_val | hex_val | fw_val
          ;

/* bin_val */

opt_bin_val:
           /* empty */ | one_star_dot_bit | dash_bit | one_star_comma_bit
           ;

one_star_dot_bit:
                '.' one_star_bit | one_star_dot_bit '.' one_star_bit
                ;

dash_bit:
        '-' one_star_bit
        ;

one_star_comma_bit:
                  ',' one_star_bit | one_star_comma_bit ',' one_star_bit
                  ;

/* dec_val */

opt_dec_val:
           /* empty */ | one_star_dot_digit | dash_digit | one_star_comma_digit
           ;

one_star_dot_digit:
                  '.' one_star_digit | one_star_dot_digit '.' one_star_digit
                  ;

dash_digit:
          '-' one_star_digit
          ;

one_star_comma_digit:
                    ',' one_star_digit | one_star_comma_digit ',' one_star_digit
                    ;

/* hex_val */

opt_hex_val:
           /* empty */ | one_star_dot_hexdig | dash_hexdig | one_star_comma_hexdig
           ;

one_star_dot_hexdig:
                   '.' one_star_hexdig | one_star_dot_hexdig '.' one_star_hexdig
                   ;

dash_hexdig:
           '-' one_star_hexdig
           ;

one_star_comma_hexdig:
                     ',' one_star_hexdig | one_star_comma_hexdig ',' one_star_hexdig
                     ;

/* fixed-width data types */

fw_val:
      uint8 | int8 | uint16_le | int16_le | uint16_be | int16_be
      | uint32_le | int32_le | uint32_be | int32_be | uint64_le
      | int64_le | uint64_be | int64_be | float_le | float_be
      ;

/* uint8 */

opt_uint8_val:
             /* empty */ | one_star_dot_uint8 | dash_uint8 | one_star_comma_uint8
             ;

one_star_dot_uint8:
                  '.' one_star_digit | one_star_dot_uint8 '.' one_star_digit
                  ;

dash_uint8:
          '-' one_star_digit
          ;

one_star_comma_uint8:
                    ',' one_star_digit | one_star_comma_uint8 ',' one_star_digit
                    ;

/* int8 */

opt_int8_val:
            /* empty */ | one_star_dot_int8 | dash_int8 | one_star_comma_int8
            ;

one_star_dot_int8:
                 '.' one_star_digit | one_star_dot_int8 '.' one_star_digit
                 | '.' '-' one_star_digit | one_star_dot_int8 '.' '-' one_star_digit
                 ;

dash_int8:
         '-' one_star_digit | '-' '-' one_star_digit
         ;

one_star_comma_int8:
                   ',' one_star_digit | one_star_comma_int8 ',' one_star_digit
                   | ',' '-' one_star_digit | one_star_comma_int8 ',' '-' one_star_digit
                   ;

/* uint16 */

opt_uint16_val:
              /* empty */ | one_star_dot_uint16 | dash_uint16 | one_star_comma_uint16
              ;

one_star_dot_uint16:
                   '.' one_star_digit | one_star_dot_uint16 '.' one_star_digit
                   ;

dash_uint16:
           '-' one_star_digit
           ;

one_star_comma_uint16:
                     ',' one_star_digit | one_star_comma_uint16 ',' one_star_digit
                     ;

/* int16 */

opt_int16_val:
             /* empty */ | one_star_dot_int16 | dash_int16 | one_star_comma_int16
             ;

one_star_dot_int16:
                 '.' one_star_digit | one_star_dot_int16 '.' one_star_digit
                 | '.' '-' one_star_digit | one_star_dot_int16 '.' '-' one_star_digit
                 ;

dash_int16:
          '-' one_star_digit | '-' '-' one_star_digit
          ;

one_star_comma_int16:
                   ',' one_star_digit | one_star_comma_int16 ',' one_star_digit
                   | ',' '-' one_star_digit | one_star_comma_int16 ',' '-' one_star_digit
                   ;

/* uint32 */

opt_uint32_val:
              /* empty */ | one_star_dot_uint32 | dash_uint32 | one_star_comma_uint32
              ;

one_star_dot_uint32:
                   '.' one_star_digit | one_star_dot_uint32 '.' one_star_digit
                   ;

dash_uint32:
           '-' one_star_digit
           ;

one_star_comma_uint32:
                     ',' one_star_digit | one_star_comma_uint32 ',' one_star_digit
                     ;

/* int32 */

opt_int32_val:
             /* empty */ | one_star_dot_int32 | dash_int32 | one_star_comma_int32
             ;

one_star_dot_int32:
                 '.' one_star_digit | one_star_dot_int32 '.' one_star_digit
                 | '.' '-' one_star_digit | one_star_dot_int32 '.' '-' one_star_digit
                 ;

dash_int32:
          '-' one_star_digit | '-' '-' one_star_digit
          ;

one_star_comma_int32:
                   ',' one_star_digit | one_star_comma_int32 ',' one_star_digit
                   | ',' '-' one_star_digit | one_star_comma_int32 ',' '-' one_star_digit
                   ;

/* uint64 */

opt_uint64_val:
              /* empty */ | one_star_dot_uint64 | dash_uint64 | one_star_comma_uint64
              ;

one_star_dot_uint64:
                   '.' one_star_digit | one_star_dot_uint64 '.' one_star_digit
                   ;

dash_uint64:
           '-' one_star_digit
           ;

one_star_comma_uint64:
                     ',' one_star_digit | one_star_comma_uint64 ',' one_star_digit
                     ;

/* int64 */

opt_int64_val:
             /* empty */ | one_star_dot_int64 | dash_int64 | one_star_comma_int64
             ;

one_star_dot_int64:
                 '.' one_star_digit | one_star_dot_int64 '.' one_star_digit
                 | '.' '-' one_star_digit | one_star_dot_int64 '.' '-' one_star_digit
                 ;

dash_int64:
          '-' one_star_digit | '-' '-' one_star_digit
          ;

one_star_comma_int64:
                   ',' one_star_digit | one_star_comma_int64 ',' one_star_digit
                   | ',' '-' one_star_digit | one_star_comma_int64 ',' '-' one_star_digit
                   ;

/* float */

opt_float_val:
             /* empty */ | one_star_dot_float | dash_float | one_star_comma_float
             ;

one_star_dot_float:
                  '.' one_star_digit '.' one_star_digit | one_star_dot_float '.' one_star_digit '.' one_star_digit
                  | '.' '-' one_star_digit '.' one_star_digit | one_star_dot_float '.' '-' one_star_digit '.' one_star_digit
                  ;

dash_float:
          '-' one_star_digit '.' one_star_digit | '-' '-' one_star_digit '.' one_star_digit
          ;

one_star_comma_float:
                    ',' one_star_digit '.' one_star_digit | one_star_comma_float ',' one_star_digit '.' one_star_digit
                    | ',' '-' one_star_digit '.' one_star_digit | one_star_comma_float ',' '-' one_star_digit '.' one_star_digit

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
          /* empty */ | star_digit DIGIT
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

star_wsp:
        /* empty */ | star_wsp WSP
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
