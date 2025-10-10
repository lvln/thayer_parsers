def Main = 
  block
   $$ = Rule

def Rule = { Sentence; CRLF; Address; CRLF; Sentence; MultiDigits }

def Address = { MultiDigits; Sentence; MultiDigits } | { MultiDigits; Sentence; SP; $['#']; MultiDigits }

def MultiDigits = {| empty = Accept |} | {| nonempty1 = { WSP; MultiDigit } |}
| {| nonempty2 = { MultiDigit; WSP } |}

def MultiDigit = Many (1 ..) Digit

def Sentence = Word | { Word; WSP; Sentence }

def Word = Many (1 ..) Alpha

def Alpha = $['a' .. 'z'] <| $['A' .. 'Z']

def Digit = $['0' .. '9']

def WSP = Many (1 ..) WSPS

def WSPS = SP <| HTAB

def HTAB = $['\t']

def SP = $[' ']

def CRLF = CR <| LF

def CR = $['\r']

def LF = $['\n'] 
