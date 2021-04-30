
A parser that accepts the grammar:

accept : L $  
L : L ';' E | E ;  
E : E ',' P | P ;  
P : 'a' | '(' M ')' ;  
M : /* nothing */ | L ;  
