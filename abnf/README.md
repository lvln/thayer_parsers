# abnf -- A BNF LALR(1) grammar for ABNF grammars

**abnf** is a Bison-generated parser that validates grammars written in ABNF.

## Notes on ABNF
- Strings are case insensitive
- Non-terminal names are case insensitive
- The end-of-line indicator is a carriage return followed by a line feed

## Examples

The following examples demonstrate simple ABNF rules. The rules listed serve to demonstrate alternative means of espressing grammatical constructs and are inexhaustive.

- Accepts the string "abc"
    - `rule = %x61 %x62 %x63`
    - `rule = %x61.62.63`
    - `rule = %d97 %d98 %d99`
    - `rule = %d97.98.99`
    - `rule = %b01100001 %b01100010 %b01100011`
    - `rule = %b01100001.01100010.01100011`
    - Note `rule = "abc"` is incorrect due to case insensitivity of strings
- Accepts a 16-bit signed integer with little-endian byte order with value -123
    - `rule = %i16_le_-123`
    - `rule = %x7D %x00`
- Accepts a 32-bit unsigned integer with big-endian byte order with value between 12 and 14
    - `rule = %u32_be_12-14`
- Accepts a floating point value with little-endian byte order with value 1.234
    - `rule = %f_le_1.234`
- Accepts a 3-bit value with value 0b101
    - `rule = %b101`
- Accepts byte with hex value 0x32 or 0x42
    - `rule = %x32,42`
    - `rule = %d50,66`
    - `rule = %u8_32,42`

## ABNF Grammar Written in ABNF

The following grammar contains extensions to the grammar in RFC 5234 allowing for representation of fixed-width datatypes and enumerations.

```
rulelist                    =  1*( rule / ( *WSP c-nl )  )

rule                        =  rulename defined-as elements c-nl
                            ; continues if next line starts
                            ;  with white space

rulename                    =  ALPHA *( ALPHA / DIGIT / "-" )

defined-as                  =  *c-wsp ( "=" / "=/" ) *c-wsp
                            ; basic rules definition and
                            ;  incremental alternatives

elements                    =  alternation *WSP

c-wsp                       =  WSP / ( c-nl WSP )

c-nl                        =  comment / CRLF
                            ; comment or newline

comment                     =  ";" *( WSP / VCHAR ) CRLF

alternation                 =  concatenation
                               *( *c-wsp "/" *c-wsp concatenation )

concatenation               =  repetition *( 1*c-wsp repetition )

repetition                  =  [ repeat ] element

repeat                      =  1*DIGIT / ( *DIGIT "*" *DIGIT )

element                     =  rulename / group / option /
                               char-val / num-val / prose-val

group                       =  "( " *c-wsp alternation *c-wsp " )"

option                      =  "[" *c-wsp alternation *c-wsp "]"

char-val                    =  case-insensitive-string
                               / case-sensitive-string

case-insensitive-string     =  ["%i"] quoted-string

case-sensitive-string       =  "%s" quoted-string

quoted-string               =  DQUOTE *(%x20-21 / %x23-7E) DQUOTE
                            ; quoted string of SP and VCHAR
                            ;  without DQUOTE

num-val                     =  "%" ( bin-val / dec-val / hex-val )

bin-val                     =  "b" 1*BIT
                               [ 1*( "." 1*BIT ) / ( "-" 1*BIT ) ]
                            ; series of concatenated bit values
                            ;  or single ONEOF range

dec-val                     =  "d" 1*DIGIT
                               [ 1*( "." 1*DIGIT ) / ( "-" 1*DIGIT ) ]

hex-val                     =  "x" 1*HEXDIG
                               [ 1*( "." 1*HEXDIG ) / ( "-" 1*HEXDIG ) ]

prose-val                   =  "<" *( %x20-3D / %x3F-7E ) ">"
                            ; bracketed string of SP and VCHAR
                            ;  without angles
                            ; prose description, to be used as
                            ;  last resort

ALPHA                       =  %x41-5A / %x61-7A
                            ; A-Z / a-z

BIT                         =  "0" / "1"

CHAR                        =  %x01-7F
                            ; any 7-bit US-ASCII character,
                            ;  excluding NUL

CR                          =  %x0D
                            ; carriage return

CRLF                        =  CR LF
                            ; Internet standard newline

CTL                         =  %x00-1F / %x7F
                            ; controls

DIGIT                       =  %x30-39
                            ; 0-9

DQUOTE                      =  %x22
                            ; " (Double Quote)

HEXDIG                      =  DIGIT / "A" / "B" / "C" / "D" / "E" / "F"

HTAB                        =  %x09
                            ; horizontal tab

LF                          =  %x0A
                            ; linefeed

LWSP                        =  *( WSP / CRLF WSP )
                            ; Use of this linear-white-space rule
                            ;  permits lines containing only white
                            ;  space that are no longer legal in
                            ;  mail headers and have caused
                            ;  interoperability problems in other
                            ;  contexts.
                            ; Do not use when defining mail
                            ;  headers and use with caution in
                            ;  other contexts.

OCTET                       =  %x00-FF
                            ; 8 bits of data

SP                          =  %x20

VCHAR                       =  %x21-7E
                            ; visible ( printing ) characters

WSP                         =  SP / HTAB
                            ; white space
```

* Note: Errata incorporated in grammar in accordance with accepted errata in RFC 5234.

## TODO
- Resolve ambiguities surrounding whitespace
- Bit operations (mask and shift)
