%{
	#define YYDEBUG 1
	int yylex(void);
	void yyerror(char *s);
%}

%token X00

%% /* The URI Grammar */
// scheme ':' hier-part queries fragments
URI:
    scheme ':' double_slashes hier-part queries fragments
    ;

double_slashes:
    /* empty */
    | '/' '/'
    ;

scheme:
    ALPHA schemes
    ;

schemes:
    /* empty */
    | schemes schemess
    ;

schemess:
    ALPHA | DIGIT | '+' | '-' | '.'
    ;

hier-part:
    authority path
    ;

authority:
    host ports
    ;

host:
    unreserved
    | host unreserved
    ;

ports:
    /* empty */
    | ':' port
    ;

port:
    DIGIT
    | port DIGIT
    ;

path:
    /* empty */
    | path '/' segment
    ;

segment:
    /* empty */
    | segment pchar
    ;

queries:
    /* empty */
    | '?' query
    ;

query:
    /* empty */
    | queriess
    ;
    
queriess:
    pchar
    | '/'
    | '?'
    ;

fragments:
    /* empty */
    | '#' fragment
    ;

fragment:
    /* empty */
    | fragmentss
    ;

fragmentss:
    pchar
    | '/'
    | '?'
    ;

pchar:
    unreserved
    | pct-encoded
    | sub-delims
    | ':'
    | '@'
    ;

pct-encoded:
    '%' HEXDIG HEXDIG
    ;

unreserved:
    ALPHA | DIGIT | '-' | '.' | '_' | '~'
    ;

sub-delims:
    '\x21' | '\x24' | '\x26' | '\x27' | '\x28' | '\x29' | '\x2A' | '\x2B' | '\x2C' | '\x3B' | '\x3D'
    ;

ALPHA:
	'\x41' | '\x42' | '\x43' | '\x44' | '\x45' | '\x46' | '\x47' | '\x48' | '\x49' | '\x4A' |
	'\x4B' | '\x4C' | '\x4D' | '\x4E' | '\x4F' | '\x50' | '\x51' | '\x52' | '\x53' | '\x54' |
	'\x55' | '\x56' | '\x57' | '\x58' | '\x59' | '\x5A' | '\x61' | '\x62' | '\x63' | '\x64' |
	'\x65' | '\x66' | '\x67' | '\x68' | '\x69' | '\x6A' | '\x6B' | '\x6C' | '\x6D' | '\x6E' |
	'\x6F' | '\x70' | '\x71' | '\x72' | '\x73' | '\x74' | '\x75' | '\x76' | '\x77' | '\x78' |
	'\x79' | '\x7A'
	;

HEXDIG:
    DIGIT | 'A' | 'B' | 'C' | 'D' | 'E' | 'F'
    ;

DIGIT:
	'\x30' | '\x31' | '\x32' | '\x33' | '\x34' | '\x35' | '\x36' | '\x37' | '\x38' | '\x39'
	;
%%