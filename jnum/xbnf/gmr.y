%{
  #define YYDEBUG 1
  int yylex(void);
  void yyerror(char *s);
%}
%token X00
%%
json: element ;

value: object
     | array
     | STRING
     | number
     | s_0
     | s_1
     | s_2
     ;

object: '{' ws '}'
      | '{' members '}'
      ;

members: member
       | members ',' member
       ;

member: ws STRING ws ':' element
      ;

array: '[' ws ']'
     | '[' elements ']'
     ;

elements: element
      | elements ',' element
      ;

element: ws value ws ;

STRING: 