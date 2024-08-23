%option noyywrap

%%
.|\n  { return *yytext; }
%%

