%{
	#define YYDEBUG 1
	int yylex(void);
	void yyerror(char *s);
%}

%token X00

%%

/* Here is the grammar */
document: prolog ws element | element ;

prolog: XMLDecl ;

XMLDecl: '<' '?' 'x' 'm' 'l' VersionInfo EncodingDecl SDDecl'?' '>' ;

VersionInfo: ws 'v' 'e' 'r' 's' 'i' 'o' 'n' ws '=' ws quotes VersionNum quotes ws;

VersionNum:  '1' '.' '0' ;

EncodingDecl: /*empty*/  | 'e' 'n' 'c' 'o' 'd' 'i' 'n' 'g' ws '=' ws quotes  Encname quotes ws ;

Encname: Enc | Encname Enc ;

Enc: letter | '-' | digit ;

SDDecl: /*empty */ |'s' 't' 'a' 'n' 'd' 'a' 'l' 'o' 'n' 'e' ws '=' ws quotes yn quotes ws ;  

yn: 'y' 'e' 's' | 'n' 'o' ; 

element: Stag ws Etag ;

Stag: '<'  Name At '>' // should be < name at ws > 

Etag: '<' '/' Name  ws '>' ;

At: Atts | /*empty */ ;

Atts:  Attrib  | Atts Attrib  ;

Attrib: space Name ws '=' ws AttValue  ;

AttValue: quotes Reference quotes ;

Reference: EntityRef | CharRef | any ; //should be ([^<&"] | Reference)*

any: letters | digits  ;

EntityRef: '&' Name ';' ;

CharRef: '&' '#' digits ';' | '&' '#' 'x' dol ';' ;

Name: letters ;

dol: digits | letters ;

letters: letter | letters letter ;

letter: lchar | uchar ;

lchar: 'a'| 'b' | 'c' | 'd' | 'e' | 'f' | 'g' | 'h' | 'i' | 'j'                                                      
      | 'k' | 'l' | 'm' | 'n' | 'o' | 'p' | 'q' | 'r' | 's' | 't'                        
      | 'u' | 'v' | 'w' | 'x' | 'y' | 'z' ;                                              
                                                                                         
uchar: 'A' | 'B' | 'C' | 'D' | 'E' | 'F'  | 'G' | 'H' | 'I' | 'J'                                                      
      | 'K' | 'L' | 'M' | 'N' | 'O' | 'P' | 'Q' | 'R' | 'S' | 'T'                        
      | 'U' | 'V' | 'W' | 'X' | 'Y' | 'Z' ;                                              
                                                                                         

digits: digit | digits digit ;                                                           
                                                                                         
digit: '0' | onenine ;                                                                   
                                                                                         
onenine: '1' | '2' | '3' | '4' | '5'| '6' | '7' | '8' | '9' ;   

quotes: "'" | '"' ; 

ws: /* empty */ | ws ' ' | ws '\n' | ws '\t' | ws '\r' ; 

space: ' ' | '\r' | '\t' | '\n' | space ' ' | space '\n' | space '\t' | space '\r' ;

//values: value | values value ;

//value: | '!' | '#' | '$' | '%' | '\'' | '(' | ')' | '*' | '+'                                                                     
//    | ',' | '-' | '.' | '/' | ':' | ';' | '=' | '>' | '?' | '@'  
//    | '[' | ']' | '_' | '`' | '{' | '|' | '}' | '~' ; 



%%

