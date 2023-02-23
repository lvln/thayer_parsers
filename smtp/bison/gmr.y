%{
	#define YYDEBUG 1
	int yylex(void);
	void yyerror(char *s);
%}

%token X00
// https://www.rfc-editor.org/rfc/rfc5321 -- Verify

%% /* The ABNF SMTP Grammar definition -- TODO */

// Command Argument Syntax

   Reverse-path   = Path / "<>"

   Forward-path   = Path

   Path           = "<" [ A-d-l ":" ] Mailbox ">"

   A-d-l          = At-domain *( "," At-domain )
                  ; Note that this form, the so-called "source
                  ; route", MUST BE accepted, SHOULD NOT be
                  ; generated, and SHOULD be ignored.

   At-domain      = "@" Domain

   Mail-parameters  = esmtp-param *(SP esmtp-param)

   Rcpt-parameters  = esmtp-param *(SP esmtp-param)

   esmtp-param    = esmtp-keyword ["=" esmtp-value]

   esmtp-keyword  = (ALPHA / DIGIT) *(ALPHA / DIGIT / "-")

   esmtp-value    = 1*(%d33-60 / %d62-126)
                  ; any CHAR excluding "=", SP, and control
                  ; characters.  If this string is an email address,
                  ; i.e., a Mailbox, then the "xtext" syntax [32]
                  ; SHOULD be used.

   Keyword        = Ldh-str

   Argument       = Atom

   Domain         = sub-domain *("." sub-domain)

      sub-domain     = Let-dig [Ldh-str]

   Let-dig        = ALPHA / DIGIT

   Ldh-str        = *( ALPHA / DIGIT / "-" ) Let-dig

   address-literal  = "[" ( IPv4-address-literal /
                    IPv6-address-literal /
                    General-address-literal ) "]"
                    ; See Section 4.1.3

   Mailbox        = Local-part "@" ( Domain / address-literal )

   Local-part     = Dot-string / Quoted-string
                  ; MAY be case-sensitive


   Dot-string     = Atom *("."  Atom)

   Atom           = 1*atext

   Quoted-string  = DQUOTE *QcontentSMTP DQUOTE

   QcontentSMTP   = qtextSMTP / quoted-pairSMTP

   quoted-pairSMTP  = %d92 %d32-126
                    ; i.e., backslash followed by any ASCII
                    ; graphic (including itself) or SPace

   qtextSMTP      = %d32-33 / %d35-91 / %d93-126
                  ; i.e., within a quoted string, any
                  ; ASCII graphic or space is permitted
                  ; without blackslash-quoting except
                  ; double-quote and the backslash itself.

   String         = Atom / Quoted-string


// Address Literals



   IPv4-address-literal  = Snum 3("."  Snum)

   IPv6-address-literal  = "IPv6:" IPv6-addr

   General-address-literal  = Standardized-tag ":" 1*dcontent

   Standardized-tag  = Ldh-str
                     ; Standardized-tag MUST be specified in a
                     ; Standards-Track RFC and registered with IANA


   dcontent       = %d33-90 / ; Printable US-ASCII
                  %d94-126 ; excl. "[", "\", "]"

   Snum           = 1*3DIGIT
                  ; representing a decimal integer
                  ; value in the range 0 through 255

   IPv6-addr      = IPv6-full / IPv6-comp / IPv6v4-full / IPv6v4-comp

   IPv6-hex       = 1*4HEXDIG

   IPv6-full      = IPv6-hex 7(":" IPv6-hex)

   IPv6-comp      = [IPv6-hex *5(":" IPv6-hex)] "::"
                  [IPv6-hex *5(":" IPv6-hex)]
                  ; The "::" represents at least 2 16-bit groups of
                  ; zeros.  No more than 6 groups in addition to the
                  ; "::" may be present.

   IPv6v4-full    = IPv6-hex 5(":" IPv6-hex) ":" IPv4-address-literal

   IPv6v4-comp    = [IPv6-hex *3(":" IPv6-hex)] "::"
                  [IPv6-hex *3(":" IPv6-hex) ":"]
                  IPv4-address-literal
                  ; The "::" represents at least 2 16-bit groups of
                  ; zeros.  No more than 4 groups in addition to the
                  ; "::" and IPv4-address-literal may be present.




%% /* C-Code */
