def Main =
  block
    $$ = P
    END

def P = $['b'] <| { $['c']; $['a' .. 'f'] }
