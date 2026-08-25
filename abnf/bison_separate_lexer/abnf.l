/*
 * abnf.l -- lexer for ABNF grammar
 *
 * Author: Josh Meise
 * Date: 08-21-2026
 *
 */

%option noinput
%option nounput

%{

#include "abnf.tab.h"

extern int linenum;

%}

%%
[a-zA-Z][a-zA-z0-9"-"] { return RULENAME; }
[0-9]+|([0-9]*"*"[0-9]*) { return REPEAT; }
"\""[\x20-\x21\23-\x7E]*"\"" { return QUOTED_STRING; }
"b"[01]+(("."[01])+|("-"[01])) { return BIN_VAL; }
"d"[0-9]+(("."[0-9])+|("-"[0-9])) { return DEC_VAL; }
"x"[0-9A-Fa-f]+(("."[0-9A-Fa-f])+|("-"[0-9A-Fa-f])) { return HEX_VAL; }
"<"[\x20-\x3D\x3F-\x7E]*">" { return PROSE_VAL; }
;[\t\x20-\x7E]* { /* ignore comments */ }
"\r\n" { linenum++; }
[ \t] { /* ignore whitespace */ }
. { return yytext[0]; }

%%

int yywrap(void) {
    return 1;
}
