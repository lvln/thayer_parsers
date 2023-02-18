%{
	#define YYDEBUG 1
	int yylex(void);
	void yyerror(char *s);
%}

%token X00


%% /* The ABNF URI Grammar definition -- TODO */

// URI = scheme ":" hier-part [ "?" query ] [ "#" fragment ]
URI:
	scheme ':' hier-part URIs URIss
	;

URIs:
	/*empty*/
	| '?' query
	;

URIss:
	/*empty*/
	| '#' fragment
	;

// hier-part = "//" authority path-abempty / path-absolute / path-rootless / path-empty
hier-part:
	'/' '/' authority path-abempty
	| path-absolute
	| path-rootless
	| path-empty ;

// URI-reference = URI / relative-ref
URI-reference:
	URI
	| relative-ref
	;

// absolute-URI = scheme ":" hier-part [ "?" query ]
absolute-URI:
	scheme ':' hier-part absolute-URIs
	;

absolute-URIs:
	/*empty*/
	| '?' query
	;

// relative-ref = relative-part [ "?" query ] [ "#" fragment ]
relative-ref:
	relative-part relative-refs relative-refss
	;

relative-refs:
	/*empty*/
	| '?' query
	;

relative-refss:
	/*empty*/
	| '#' fragment
	;

// relative-part = "//" authority path-abempty / path-absolute / path-noscheme / path-empty
relative-part:
	'/' '/' authority path-abempty
	| path-absolute
	| path-noscheme
	| path-empty
	;

// scheme = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )
scheme:
	ALPHA schemes
	;

schemes:
	/*empty*/
	| schemes schem
	;

schem:
	ALPHA
	| DIGIT
	| '+'
	| '-'
	| '.'
	;

// authority = [ userinfo "@" ] host [ ":" port ]
authority:
	authoritys host authorityss
	;

authoritys:
	/*empty*/
	| userinfo '@'
	;

authorityss:
	/*empty*/
	| ':' port
	;

// userinfo = *( unreserved / pct-encoded / sub-delims / ":" )
userinfo:
	/*empty*/
	| userinfo userinf
	;

userinf:
	unreserved
	| pct-encoded
	| sub-delims
	| ':'
	;

// host = IP-literal / IPv4address / reg-name
host:
	IP-literal
	| IPv4address
	| reg-name
	;

// port = *DIGIT
port:
	/*empty*/
	| port DIGIT
	;

// IP-literal = "[" ( IPv6address / IPvFuture ) "]"
IP-literal:
	'['  IP-litera
	;

IP-litera:
	IPv6address ']'
	| IPvFuture ']'
	;

// IPvFuture = "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" )
IPvFuture:
	'v' IPvFutures '.' IPvFuturess
	;

IPvFutures:
	HEXDIG
	| IPvFutures HEXDIG
	;

IPvFuturess:
	IPvFutur
	| IPvFuturess IPvFutur
	;

IPvFutur:
	unreserved | sub-delims | ':'
	;

// TODO -- Repitition
// IPv6address = 6( h16 ":" ) ls32 / "::" 5( h16 ":" ) ls32 / [ h16 ] "::" 4( h16 ":" ) ls32 / [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32 / [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32 / [ *3( h16 ":" ) h16 ] "::" h16 ":" ls32 / [ *4( h16 ":" ) h16 ] "::" ls32 / [ *5( h16 ":" ) h16 ] "::" h16 / [ *6( h16 ":" ) h16 ] "::"
IPv6address:
	6( h16 ':' ) ls32
	| ':' ':' 5( h16 ':' ) ls32
	| [ h16 ] ':' ':' 4( h16 ':' ) ls32
	| [ *1( h16 ':' ) h16 ] ':' ':' 3( h16 ':' ) ls32
	| [ *2( h16 ':' ) h16 ] ':' ':' 2( h16 ':' ) ls32
	| [ *3( h16 ':' ) h16 ] ':' ':' h16 ':' ls32
	| [ *4( h16 ':' ) h16 ] ':' ':' ls32
	| [ *5( h16 ':' ) h16 ] ':' ':' h16
	| [ *6( h16 ':' ) h16 ] ':' ':'
	;

// h16 = 1*4HEXDIG
h16:
	4HEXDIG
	| h16 4HEXDIG
	;

// ls32 = ( h16 ":" h16 ) / IPv4address
ls32:
	h16 ':' h16
	| IPv4address
	;

// IPv4address = dec-octet "." dec-octet "." dec-octet "." dec-octet
IPv4address:
	dec-octet '.' dec-octet '.' dec-octet '.' dec-octet
	;

// dec-octet = DIGIT / %x31-39 DIGIT / "1" 2DIGIT / "2" %x30-34 DIGIT / "25" %x30-35 
dec-octet:
	DIGIT
	| %x31-39 DIGIT
	| '1' 2DIGIT
	| '2' %x30-34 DIGIT
	| '25' %x30-35
	;

// reg-name = *( unreserved / pct-encoded / sub-delims )
reg-name:
	/*empty*/
	| reg-name reg-nam
	;

reg-nam:
	unreserved
	| pct-encoded
	| sub-delims
	;

// path = path-abempty / path-absolute / path-noscheme / path-rootless / path-empty
path:
	path-abempty
	| path-absolute
	| path-noscheme
	| path-rootless
	| path-empty
	;

// path-abempty = *( "/" segment )
path-abempty:
	/*empty*/
	| path-abempty '/' segment
	;

// path-absolute = "/" [ segment-nz *( "/" segment ) ]
path-absolute:
	'/' path-absolutess
	;

path-absolutess:
	/*empty*/ | segment-nz path-absolutes
	;

path-absolutes:
	/*empty*/ | path-absolutes '/' segment
	;

// path-noscheme = segment-nz-nc *( "/" segment )
path-noscheme:
	segment-nz-nc path-noschemes
	;

path-noschemes:
	/*empty*/
	| path-noschemes '/' segment
	;

// path-rootless = segment-nz *( "/" segment )
path-rootless:
	segment-nz path-rootlesss
	;

path-rootlesss:
	/*empty*/
	| path-rootlesss '/' segment
	;












// TODO
// path-empty = 0<pchar>
// WTF??

// segment = *pchar
segment: /*empty*/ | segment pchar ;

// segment-nz = 1*pchar
segment-nz: pchar | segment-nz pchar ;





// TODO
// segment-nz-nc = 1*( unreserved / pct-encoded / sub-delims / "@" )
// Error












// pchar = unreserved / pct-encoded / sub-delims / ":" / "@"
pchar:
	unreserved
	| pct-encoded
	| sub-delims
	| ':'
	| '@'
	;

// query = *( pchar / "/" / "?" )
query:
	/*empty*/
	| query quer
	;

quer:
	pchar
	| '/'
	| '?'
	;

// fragment = *( pchar / "/" / "?" )
fragment:
	/*empty*/
	| fragment fragmen
	;

fragmen:
	pchar
	| '/'
	| '?'
	;

// pct-encoded = "%" HEXDIG HEXDIG
pct-encoded:
	'%' HEXDIG HEXDIG
	;

// unreserved = ALPHA / DIGIT / "-" / "." / "_" / "~"
unreserved:
	ALPHA
	| DIGIT
	| '-'
	| '.'
	| '_'
	| '~'
	;

// reserved = gen-delims / sub-delims
reserved:
	gen-delims
	| sub-delims
	;

// gen-delims = ":" / "/" / "?" / "#" / "[" / "]" / "@"
gen-delims:
	':'
	| '/'
	| '?'
	| '#'
	| '['
	| ']'
	| '@'
	;

// sub-delims = "!" / "$" / "&" / "'" / "(" / ")" / "*" / "+" / "," / ";" / "="
sub-delims:
	'!'
	| '$'
	| '&'
	| '''
	| '('
	| ')'
	| '*'
	| '+'
	| ','
	| ';'
	| '='
	;

%% /* C-Code */