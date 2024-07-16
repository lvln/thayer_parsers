# variable_length

A parser that accepts messages of variable length.

This parser accepts messages containing 0-5 repetitions of the character 'a' and 0 or more repetitons of the character `a`.

Calls `YYERROR` to terminate parsing process if the grammar rule is violated in that more then 5 `a`'s have been parsed.
