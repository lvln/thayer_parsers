
def Main =
  block
    $$ = P
    END

def P = $['a'] | { $['(']; M; $[')'] }

def M = {| empty =  Accept |} | {| nonempty = P |}
