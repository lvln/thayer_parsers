
A parser that accepts the grammar:

accept : P $  
P : 'a'	| '(' M ')' ;  
M : /* nothing */	| P ;  
