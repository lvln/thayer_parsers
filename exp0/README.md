# skip_wildcard

A parser that skips over any byte as opposed to comparing it to every byte.

Note: '*' stands as a substitute for any byte from 0x00 - 0xFF

accept : a\*c

Makefile in `xbnf` mute be written to use preprocessor in `xbnf_skip_wildcard`.
