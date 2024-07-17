%{
#define YYDEBUG 1
int yylex(void);
void yyerror(char *s);

int hdgCount = 0;
%}
%token X00
%%

p : a_or_more_1 a_or_more_2 ;

a_or_more_1 : /* empty */ | 'a' ;

a_or_more_2 : /* empty */ | 'b' ;
