%{
  #define YYDEBUG 1
  int yylex(void);
  void yyerror(char *s);
%}
%token X00
%%
STRING: '"' chars '"' ;

chars: /* empty */ | chars CHAR	;

CHAR: uchar | lchar | digit | punct | '\\' escchar ;

digit: '0' | onenine ;

onenine: r__1 ;

escchar: 'b' | 'f' | 'n' | 'r' | 't' | '"' | '\\' | '/' | 'u' hex hex hex hex ;

punct: ' ' | '!' | r__2 | r__3 | '[' | r__4 | r__5 ; 

uhex:     r__6 ;                                                        
                                                                                                 
lhex:     r__7 ;

lchar: lhex |  r__8 ;                                                                                        
                                                                                                                  
uchar: uhex | r__9 ;                                                                                         

hex: digit | uhex | lhex ;

/* Range Expansions */
r__1 : 
  '\x31' | '\x32' | '\x33' | '\x34' | '\x35' | '\x36' | '\x37' | '\x38' | 
  '\x39' ;
r__2 : 
  '\x23' | '\x24' | '\x25' | '\x26' | '\x27' | '\x28' | '\x29' | '\x2a' | 
  '\x2b' | '\x2c' | '\x2d' | '\x2e' | '\x2f' ;
r__3 : 
  '\x3a' | '\x3b' | '\x3c' | '\x3d' | '\x3e' | '\x3f' | '\x40' ;
r__4 : 
  '\x5d' | '\x5e' | '\x5f' | '\x60' ;
r__5 : 
  '\x7b' | '\x7c' | '\x7d' | '\x7e' | '\x7f' ;
r__6 : 
  '\x41' | '\x42' | '\x43' | '\x44' | '\x45' | '\x46' ;
r__7 : 
  '\x61' | '\x62' | '\x63' | '\x64' | '\x65' | '\x66' ;
r__8 : 
  '\x67' | '\x68' | '\x69' | '\x6a' | '\x6b' | '\x6c' | '\x6d' | '\x6e' | 
  '\x6f' | '\x70' | '\x71' | '\x72' | '\x73' | '\x74' | '\x75' | '\x76' | 
  '\x77' | '\x78' | '\x79' | '\x7a' ;
r__9 : 
  '\x47' | '\x48' | '\x49' | '\x4a' | '\x4b' | '\x4c' | '\x4d' | '\x4e' | 
  '\x4f' | '\x50' | '\x51' | '\x52' | '\x53' | '\x54' | '\x55' | '\x56' | 
  '\x57' | '\x58' | '\x59' | '\x5a' ;
