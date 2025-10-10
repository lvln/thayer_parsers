
def Main =
  block
    $$ = Number
    END

def Number = { Integer; Fraction; Exponent }

def Integer = {| one = Digit |} | {| two = { OneNine; Digits } |} | {| three
= { $['-']; Digit } |} | {| four = { $['-']; OneNine; Digits } |}

def Fraction = Optional { $['.']; Digits }

def Exponent = Optional ({ $['E']; Sign; Digits } | { $['e']; Sign; Digits })

def Sign = Optional ($['+'] | $['-'])

def Digits = Many (1 ..) Digit

def Digit = $['0'] | OneNine

def OneNine = $['1' .. '9']
