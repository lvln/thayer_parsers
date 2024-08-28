%{
  #define YYDEBUG 1
  int yylex(void);
  void yyerror(char *s);
%}
%token X00
%%
/* Josh Meise */
/* June 17, 2024 */
/* Parser for HTTP messages */
/* Reference - RFC 7230 */

/* HTTP message definition */
HTTPMessage : startLine optHeaderField CRLF messageBody ;

/* Message is any repition of characters */
messageBody : /* empty */ | messageBody OCTET ;

/* Header field is optional and contains a field name followed by a colon, optional white space and followed by a field value */
optHeaderField : /* empty */ | optHeaderField headerField ;

headerField : fieldName ':' OWS fieldValue CRLF | fieldName ':' OWS CRLF ;

fieldValue : fieldValueSS | fieldValue fieldValueSS ;

fieldValueSS : fieldContent | obsFold ; 

fieldContent : fieldVChar optFieldContent ;

optFieldContent : /* empty */ | RWS fieldVChar ;

fieldVChar : obsText | VCHAR ;

/* Even though this line fold is obsolete still account for it for consistency with the RFC */
obsFold : CRLF RWS ;

fieldName : token ;

token : tChar | token tChar ;

tChar : '!' | '#' | '$' | '%' | '&' | '\x27' | '*' | '+' | '-' | '.' | '^' | '_' | '`' | '|' | '~' | DIGIT | ALPHA ;

/* HTTP messages may either be requests or status messages */
startLine : statusLine CRLF | requestLine CRLF ;

/* A status message is a status message, followed by a status code, followed by a reason phrase */
statusLine : HTTPVersion SP statusCode SP reasonPhrase ;

statusCode : '1' DIGIT DIGIT | '2' DIGIT DIGIT | '3' DIGIT DIGIT | '4' DIGIT DIGIT | '5' DIGIT DIGIT ;

reasonPhrase : reasonPhraseSS | reasonPhrase reasonPhraseSS ;

reasonPhraseSS : HTAB | SP | VCHAR | obsText ;

obsText :     r__1 ;

/* A request message is a method, followed by a target (URI), followed by an HTTP version */
requestLine : method SP requestTarget SP HTTPVersion ;

requestTarget : originForm | absoluteForm | asteriskForm ;

originForm : absolutePath queries ;

absoluteForm : absoluteURI ;

asteriskForm : '*' ;

absolutePath : '/' segment | absolutePath '/' segment ;

absoluteURI : scheme ':' relativePart queries ;

relativePart : doubleSlashes hierPart ;

doubleSlashes: /* empty */ | '/' '/' ;

scheme: ALPHA schemes ;

schemes: /* empty */ | schemes schemess ;

schemess: ALPHA | DIGIT | '+' | '-' | '.' ;

hierPart: authority path ;

authority: host ports ;

host: unreserved | host unreserved ;

ports: /* empty */ | ':' port ;

port: DIGIT | port DIGIT ;

path: /* empty */ | path '/' segment ;

segment: /* empty */ | segment pchar ;

queries: /* empty */ | '?' query ;

query: /* empty */ | query queriess ;

queriess: pchar | '/' | '?' ;

pchar: unreserved | pct_encoded | sub_delims | ':' | '@' ;

pct_encoded: '%' HEXDIG HEXDIG ;

unreserved: ALPHA | DIGIT | '-' | '.' | '_' | '~' ;

sub_delims: '!' | '$' |     r__2 | ';' | '=' ;

/* HTTP version is the string HTTP followed by major-minor digit scheme */
HTTPVersion : s__0 '/' DIGIT '.' DIGIT ;

/* Tokens */
ALPHA : LOALPHA | UPALPHA ;

LOALPHA :     r__3 ;

UPALPHA :     r__4 ;

DIGIT :     r__5 ;

HEXDIG : DIGIT |     r__6 ;

SP : '\x20' ;

HTAB : '\x09' ;

WS : SP | HTAB ;

OWS : /* empty */ | OWS WS ;

RWS : WS | RWS WS ;

VCHAR :     r__7 ;

LF : '\x0a' ;

CR : '\x0d' ;

CRLF : CR LF ;

OCTET :     r__0 ;

method : s__1 | s__2 | s__3 | s__4 | s__5 | s__6 | s__7 | s__8 ;
/* Range Expansions */
r__0 : 
  X00 | '\x01' | '\x02' | '\x03' | '\x04' | '\x05' | '\x06' | '\x07' | 
  '\x08' | '\x09' | '\x0a' | '\x0b' | '\x0c' | '\x0d' | '\x0e' | '\x0f' | 
  '\x10' | '\x11' | '\x12' | '\x13' | '\x14' | '\x15' | '\x16' | '\x17' | 
  '\x18' | '\x19' | '\x1a' | '\x1b' | '\x1c' | '\x1d' | '\x1e' | '\x1f' | 
  '\x20' | '\x21' | '\x22' | '\x23' | '\x24' | '\x25' | '\x26' | '\x27' | 
  '\x28' | '\x29' | '\x2a' | '\x2b' | '\x2c' | '\x2d' | '\x2e' | '\x2f' | 
  '\x30' | '\x31' | '\x32' | '\x33' | '\x34' | '\x35' | '\x36' | '\x37' | 
  '\x38' | '\x39' | '\x3a' | '\x3b' | '\x3c' | '\x3d' | '\x3e' | '\x3f' | 
  '\x40' | '\x41' | '\x42' | '\x43' | '\x44' | '\x45' | '\x46' | '\x47' | 
  '\x48' | '\x49' | '\x4a' | '\x4b' | '\x4c' | '\x4d' | '\x4e' | '\x4f' | 
  '\x50' | '\x51' | '\x52' | '\x53' | '\x54' | '\x55' | '\x56' | '\x57' | 
  '\x58' | '\x59' | '\x5a' | '\x5b' | '\x5c' | '\x5d' | '\x5e' | '\x5f' | 
  '\x60' | '\x61' | '\x62' | '\x63' | '\x64' | '\x65' | '\x66' | '\x67' | 
  '\x68' | '\x69' | '\x6a' | '\x6b' | '\x6c' | '\x6d' | '\x6e' | '\x6f' | 
  '\x70' | '\x71' | '\x72' | '\x73' | '\x74' | '\x75' | '\x76' | '\x77' | 
  '\x78' | '\x79' | '\x7a' | '\x7b' | '\x7c' | '\x7d' | '\x7e' | '\x7f' | 
  '\x80' | '\x81' | '\x82' | '\x83' | '\x84' | '\x85' | '\x86' | '\x87' | 
  '\x88' | '\x89' | '\x8a' | '\x8b' | '\x8c' | '\x8d' | '\x8e' | '\x8f' | 
  '\x90' | '\x91' | '\x92' | '\x93' | '\x94' | '\x95' | '\x96' | '\x97' | 
  '\x98' | '\x99' | '\x9a' | '\x9b' | '\x9c' | '\x9d' | '\x9e' | '\x9f' | 
  '\xa0' | '\xa1' | '\xa2' | '\xa3' | '\xa4' | '\xa5' | '\xa6' | '\xa7' | 
  '\xa8' | '\xa9' | '\xaa' | '\xab' | '\xac' | '\xad' | '\xae' | '\xaf' | 
  '\xb0' | '\xb1' | '\xb2' | '\xb3' | '\xb4' | '\xb5' | '\xb6' | '\xb7' | 
  '\xb8' | '\xb9' | '\xba' | '\xbb' | '\xbc' | '\xbd' | '\xbe' | '\xbf' | 
  '\xc0' | '\xc1' | '\xc2' | '\xc3' | '\xc4' | '\xc5' | '\xc6' | '\xc7' | 
  '\xc8' | '\xc9' | '\xca' | '\xcb' | '\xcc' | '\xcd' | '\xce' | '\xcf' | 
  '\xd0' | '\xd1' | '\xd2' | '\xd3' | '\xd4' | '\xd5' | '\xd6' | '\xd7' | 
  '\xd8' | '\xd9' | '\xda' | '\xdb' | '\xdc' | '\xdd' | '\xde' | '\xdf' | 
  '\xe0' | '\xe1' | '\xe2' | '\xe3' | '\xe4' | '\xe5' | '\xe6' | '\xe7' | 
  '\xe8' | '\xe9' | '\xea' | '\xeb' | '\xec' | '\xed' | '\xee' | '\xef' | 
  '\xf0' | '\xf1' | '\xf2' | '\xf3' | '\xf4' | '\xf5' | '\xf6' | '\xf7' | 
  '\xf8' | '\xf9' | '\xfa' | '\xfb' | '\xfc' | '\xfd' | '\xfe' | '\xff' ;
r__1 : 
  '\x80' | '\x81' | '\x82' | '\x83' | '\x84' | '\x85' | '\x86' | '\x87' | 
  '\x88' | '\x89' | '\x8a' | '\x8b' | '\x8c' | '\x8d' | '\x8e' | '\x8f' | 
  '\x90' | '\x91' | '\x92' | '\x93' | '\x94' | '\x95' | '\x96' | '\x97' | 
  '\x98' | '\x99' | '\x9a' | '\x9b' | '\x9c' | '\x9d' | '\x9e' | '\x9f' | 
  '\xa0' | '\xa1' | '\xa2' | '\xa3' | '\xa4' | '\xa5' | '\xa6' | '\xa7' | 
  '\xa8' | '\xa9' | '\xaa' | '\xab' | '\xac' | '\xad' | '\xae' | '\xaf' | 
  '\xb0' | '\xb1' | '\xb2' | '\xb3' | '\xb4' | '\xb5' | '\xb6' | '\xb7' | 
  '\xb8' | '\xb9' | '\xba' | '\xbb' | '\xbc' | '\xbd' | '\xbe' | '\xbf' | 
  '\xc0' | '\xc1' | '\xc2' | '\xc3' | '\xc4' | '\xc5' | '\xc6' | '\xc7' | 
  '\xc8' | '\xc9' | '\xca' | '\xcb' | '\xcc' | '\xcd' | '\xce' | '\xcf' | 
  '\xd0' | '\xd1' | '\xd2' | '\xd3' | '\xd4' | '\xd5' | '\xd6' | '\xd7' | 
  '\xd8' | '\xd9' | '\xda' | '\xdb' | '\xdc' | '\xdd' | '\xde' | '\xdf' | 
  '\xe0' | '\xe1' | '\xe2' | '\xe3' | '\xe4' | '\xe5' | '\xe6' | '\xe7' | 
  '\xe8' | '\xe9' | '\xea' | '\xeb' | '\xec' | '\xed' | '\xee' | '\xef' | 
  '\xf0' | '\xf1' | '\xf2' | '\xf3' | '\xf4' | '\xf5' | '\xf6' | '\xf7' | 
  '\xf8' | '\xf9' | '\xfa' | '\xfb' | '\xfc' | '\xfd' | '\xfe' | '\xff' ;
r__2 : 
  '\x26' | '\x27' | '\x28' | '\x29' | '\x2a' | '\x2b' | '\x2c' ;
r__3 : 
  '\x61' | '\x62' | '\x63' | '\x64' | '\x65' | '\x66' | '\x67' | '\x68' | 
  '\x69' | '\x6a' | '\x6b' | '\x6c' | '\x6d' | '\x6e' | '\x6f' | '\x70' | 
  '\x71' | '\x72' | '\x73' | '\x74' | '\x75' | '\x76' | '\x77' | '\x78' | 
  '\x79' | '\x7a' ;
r__4 : 
  '\x41' | '\x42' | '\x43' | '\x44' | '\x45' | '\x46' | '\x47' | '\x48' | 
  '\x49' | '\x4a' | '\x4b' | '\x4c' | '\x4d' | '\x4e' | '\x4f' | '\x50' | 
  '\x51' | '\x52' | '\x53' | '\x54' | '\x55' | '\x56' | '\x57' | '\x58' | 
  '\x59' | '\x5a' ;
r__5 : 
  '\x30' | '\x31' | '\x32' | '\x33' | '\x34' | '\x35' | '\x36' | '\x37' | 
  '\x38' | '\x39' ;
r__6 : 
  '\x41' | '\x42' | '\x43' | '\x44' | '\x45' | '\x46' | '\x47' | '\x48' | 
  '\x49' | '\x4a' | '\x4b' | '\x4c' | '\x4d' | '\x4e' | '\x4f' | '\x50' | 
  '\x51' | '\x52' | '\x53' | '\x54' | '\x55' | '\x56' | '\x57' | '\x58' | 
  '\x59' | '\x5a' ;
r__7 : 
  '\x21' | '\x22' | '\x23' | '\x24' | '\x25' | '\x26' | '\x27' | '\x28' | 
  '\x29' | '\x2a' | '\x2b' | '\x2c' | '\x2d' | '\x2e' | '\x2f' | '\x30' | 
  '\x31' | '\x32' | '\x33' | '\x34' | '\x35' | '\x36' | '\x37' | '\x38' | 
  '\x39' | '\x3a' | '\x3b' | '\x3c' | '\x3d' | '\x3e' | '\x3f' | '\x40' | 
  '\x41' | '\x42' | '\x43' | '\x44' | '\x45' | '\x46' | '\x47' | '\x48' | 
  '\x49' | '\x4a' | '\x4b' | '\x4c' | '\x4d' | '\x4e' | '\x4f' | '\x50' | 
  '\x51' | '\x52' | '\x53' | '\x54' | '\x55' | '\x56' | '\x57' | '\x58' | 
  '\x59' | '\x5a' | '\x5b' | '\x5c' | '\x5d' | '\x5e' | '\x5f' | '\x60' | 
  '\x61' | '\x62' | '\x63' | '\x64' | '\x65' | '\x66' | '\x67' | '\x68' | 
  '\x69' | '\x6a' | '\x6b' | '\x6c' | '\x6d' | '\x6e' | '\x6f' | '\x70' | 
  '\x71' | '\x72' | '\x73' | '\x74' | '\x75' | '\x76' | '\x77' | '\x78' | 
  '\x79' | '\x7a' | '\x7b' | '\x7c' | '\x7d' | '\x7e' ;

/* String Expansions */
s__0 : 
 'H' 'T' 'T' 'P' ;
s__1 : 
 'G' 'E' 'T' ;
s__2 : 
 'H' 'E' 'A' 'D' ;
s__3 : 
 'P' 'O' 'S' 'T' ;
s__4 : 
 'P' 'U' 'T' ;
s__5 : 
 'D' 'E' 'L' 'E' 'T' 'E' ;
s__6 : 
 'C' 'O' 'N' 'N' 'E' 'C' 'T' ;
s__7 : 
 'O' 'P' 'T' 'I' 'O' 'N' 'S' ;
s__8 : 
 'T' 'R' 'A' 'C' 'E' ;
