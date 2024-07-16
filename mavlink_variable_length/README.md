# variable_length

A parser that accepts messages of variable length.

This parser accepts messages containing 0-5 repetitions of the character 'a' and 0 or more repetitons of the character `a`.

Calls `YYERROR` to terminate parsing process if the grammar rule is violated in that more then 5 `a`'s have been parsed. If there is a desire to print an error message as opposed to simply termination, it is necessary to ammend `yyerror()` in `gmr_tb.c` to exit with code `EXIT_FAILURE` and then call `yyerror()` when more than 5 `a`'s have been parsed.
