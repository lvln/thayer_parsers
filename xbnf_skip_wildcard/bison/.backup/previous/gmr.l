%{

#include "gmr.tab.h"

%}

%option noyywrap

%%
.|\n  { return *yytext; }
%%

