%{
#define YYDBUG 1
int yylex(void);
void yyerror(char *s);

int a_count = 0;
%}

%token X00

%%

/* Here is the grammar */
p : /* empty */ | p a_or_c ;

a_or_c : 'a' { a_count++; if (a_count > 5) YYERROR; }
| 'c' ;

%%
