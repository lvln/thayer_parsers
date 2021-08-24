%{
	#define YYDEBUG 1
	int yylex(void);
	void yyerror(char *s);
%}

%token X00

%%

/* Here is the grammar */
document: prolog | inside ;

prolog: XMLDecl inside | XMLDecl doctypedecl inside | doctypedecl inside ;  

doctypedecl: '<' '!' 'D' 'O' 'C' 'T' 'Y' 'P' 'E' space Name   '>' ws ;


XMLDecl: '<' '?' 'x' 'm' 'l' VersionInfo EncodingDecl SDDecl'?' '>' ws ;

VersionInfo: ' ' 'v' 'e' 'r' 's' 'i' 'o' 'n' ws '=' ws quotes VersionNum quotes ws;

VersionNum:  '1' '.' '0' ;

EncodingDecl: /*empty*/  | 'e' 'n' 'c' 'o' 'd' 'i' 'n' 'g' ws '=' ws quotes  Encname quotes ws ;

Encname: Enc | Encname Enc ;

Enc: letter | '-' | digit ;

SDDecl: /*empty */ |'s' 't' 'a' 'n' 'd' 'a' 'l' 'o' 'n' 'e' ws '=' ws quotes yn quotes ws ;  

yn: 'y' 'e' 's' | 'n' 'o' ; 

inside: block  | inside block ;

block: Stag content Etag | EmptyElemTag | Stag Etag ;

content: Reference   | block ;

EmptyElemTag: '<' Name At '/' '>' ;

Stag: '<' Name At '>'  

Etag: '<' '/' Name  ws '>' ;

At: Atts | /*empty */ | space ;

Atts:  Attrib  | Atts Attrib  ;

Attrib: space Name ws '=' ws AttValue  ;

AttValue: '"' Reference '"'  ;

Reference: EntityRef | CharRef | any ; //([^<&"] | Reference)*

any: r | any r ;

r: letter | digit  | punct | s | '\\' escchar ;

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

quotes: '\'' | '"' ; 

ws: /* empty */ | ws ' ' | ws '\n' | ws '\t' | ws '\r' ; 

space: s | space s ;

s: ' ' | '\r' | '\t' | '\n' ;

punct: '!' | '#' | '$' | '%' | '(' | ')' | '*' | '+'  //should not include <>, & ' or "       
      | ',' | '-' | '.' | '/' | ':' | ';' | '=' | '?' | '@'
      | '^' | '_' | '`' | '{' | '|' | '}' | '~' ;


escchar: 'b' | 'f' | 'n' | 'r' | 't' | '\\' | '/' ;

//reserved: '&' alt ;

//alt: 'l' 't' | 'g' 't' | 'a' 'm' 'p' | 'a' 'p' 'o' 's' | 'q' 'u' 'o' 't' ;

%%

