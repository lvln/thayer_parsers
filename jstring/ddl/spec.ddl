
def Main =
  block
    $$ = String
    END

def String = { $['"']; Chars; $['"'] }

def Chars = Many Char

def Char = UChar | LChar | Digit | Punct | { $['\\']; EscChar }

def Digit = $['0'] | OneNine

def OneNine = $['1' .. '9']

def EscChar = 
  Choose
    $['b']
    $['f']
    $['n']
    $['r']
    $['t']
    $['"']
    $['\\']
    $['/']
    block
      $['u']
      Hex
      Hex
      Hex
      Hex

def Punct = 
  Choose
    $[' ']
    $['!']
    $['#' .. '/']
    $[':' .. '@']
    $['[']
    $[']' .. '`']
    $['{' .. 0x7f]

def UHex = $['A' .. 'F']

def LHex = $['a' .. 'f']

def LChar = LHex | $['g' .. 'z']

def UChar = UHex | $['G' .. 'Z']

def Hex = Digit | UHex | LHex
