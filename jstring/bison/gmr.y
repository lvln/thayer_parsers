%{
  #define YYDEBUG 1
  extern int yylex(void);
  void yyerror(char *s);
%}
%token X00
%token BYTE
%%
STRING: '"' chars '"' ;

chars: /* empty */ | chars CHAR	;

CHAR: uchar | lchar | digit | punct | '\\' escchar ;

digit: '0' | onenine ;

onenine: r__0 ;

escchar: 'b' | 'f' | 'n' | 'r' | 't' | '"' | '\\' | '/' | 'u' hex hex hex hex ;

punct: ' ' | '!' | r__1 | r__2 | '[' | r__3 | r__4 ; 

uhex:     r__5 ;                                                        
                                                                                                 
lhex:     r__6 ;

lchar: lhex |  r__7 ;                                                                                        
                                                                                                                  
uchar: uhex | r__8 ;                                                                                         

hex: digit | uhex | lhex ;

/* Range Expansions */
r__0 : 
  '\x31' | '\x32' | '\x33' | '\x34' | '\x35' | '\x36' | '\x37' | '\x38' | 
  '\x39' ;
r__1 : 
  '\x23' | '\x24' | '\x25' | '\x26' | '\x27' | '\x28' | '\x29' | '\x2a' | 
  '\x2b' | '\x2c' | '\x2d' | '\x2e' | '\x2f' ;
r__2 : 
  '\x3a' | '\x3b' | '\x3c' | '\x3d' | '\x3e' | '\x3f' | '\x40' ;
r__3 : 
  '\x5d' | '\x5e' | '\x5f' | '\x60' ;
r__4 : 
  '\x7b' | '\x7c' | '\x7d' | '\x7e' | '\x7f' ;
r__5 : 
  '\x41' | '\x42' | '\x43' | '\x44' | '\x45' | '\x46' ;
r__6 : 
  '\x61' | '\x62' | '\x63' | '\x64' | '\x65' | '\x66' ;
r__7 : 
  '\x67' | '\x68' | '\x69' | '\x6a' | '\x6b' | '\x6c' | '\x6d' | '\x6e' | 
  '\x6f' | '\x70' | '\x71' | '\x72' | '\x73' | '\x74' | '\x75' | '\x76' | 
  '\x77' | '\x78' | '\x79' | '\x7a' ;
r__8 : 
  '\x47' | '\x48' | '\x49' | '\x4a' | '\x4b' | '\x4c' | '\x4d' | '\x4e' | 
  '\x4f' | '\x50' | '\x51' | '\x52' | '\x53' | '\x54' | '\x55' | '\x56' | 
  '\x57' | '\x58' | '\x59' | '\x5a' ;
