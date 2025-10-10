
def Main =
  block
    $$ = L
    END

def L = { E; $[';']; L } | E

def E = { P; $[',']; E } | P

def P = $['a'] | { $['(']; M; $[')'] }

def M = {| empty =  Accept |} | {| nonempty = L |}
