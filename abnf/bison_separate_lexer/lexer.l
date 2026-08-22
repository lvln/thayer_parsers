/*
 * lexer.l -- lexer for ABNF grammar
 *
 * Author: Josh Meise
 * Date: 08-21-2026
 *
 */

%{

#include "y.tab.h"

int linenum = 1;

%}

%%


[\r\n]              { linenum++; }
.                   { return yytext[0]; }

%%

int yywrap(void) {
    return 1;
}
