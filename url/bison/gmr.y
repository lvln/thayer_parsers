%{
  #define YYDEBUG 1
  int yylex(void);
  void yyerror(char *s);
%}
%token X00
%%
URI: scheme ':' double_slashes hier_part queries fragments ;

double_slashes: /* empty */ | '/' '/' ;

scheme: ALPHA schemes ;

schemes: /* empty */ | schemes schemess ;

schemess: ALPHA | DIGIT | '+' | '-' | '.' ;

hier_part: authority path ;

authority: host ports ;

host: unreserved | host unreserved ;

ports: /* empty */ | ':' port ;

port: DIGIT | port DIGIT ;

path: /* empty */ | path '/' segment ;

segment: /* empty */ | segment pchar ;

queries: /* empty */ | '?' query ;

query: /* empty */ | query queriess ;

queriess: pchar | '/' | '?' ;

fragments: /* empty */ | '#' fragment ;

fragment: /* empty */ | fragment fragmentss ;

fragmentss: pchar | '/' | '?' ;

pchar: unreserved | pct_encoded | sub_delims | ':' | '@' ;

pct_encoded: '%' HEXDIG HEXDIG ;

unreserved: ALPHA | DIGIT | '-' | '.' | '_' | '~' ;

sub_delims: '!' | '$' |     r__1 | ';' | '=' ;

ALPHA:     r__2 |     r__3 ;

HEXDIG: DIGIT |     r__4 ;

DIGIT:     r__5 ;
/* Range Expansions */
r__1 : 
  '\x26' | '\x27' | '\x28' | '\x29' | '\x2a' | '\x2b' | '\x2c' ;
r__2 : 
  '\x61' | '\x62' | '\x63' | '\x64' | '\x65' | '\x66' | '\x67' | '\x68' | 
  '\x69' | '\x6a' | '\x6b' | '\x6c' | '\x6d' | '\x6e' | '\x6f' | '\x70' | 
  '\x71' | '\x72' | '\x73' | '\x74' | '\x75' | '\x76' | '\x77' | '\x78' | 
  '\x79' | '\x7a' ;
r__3 : 
  '\x41' | '\x42' | '\x43' | '\x44' | '\x45' | '\x46' | '\x47' | '\x48' | 
  '\x49' | '\x4a' | '\x4b' | '\x4c' | '\x4d' | '\x4e' | '\x4f' | '\x50' | 
  '\x51' | '\x52' | '\x53' | '\x54' | '\x55' | '\x56' | '\x57' | '\x58' | 
  '\x59' | '\x5a' ;
r__4 : 
  '\x41' | '\x42' | '\x43' | '\x44' | '\x45' | '\x46' ;
r__5 : 
  '\x30' | '\x31' | '\x32' | '\x33' | '\x34' | '\x35' | '\x36' | '\x37' | 
  '\x38' | '\x39' ;
