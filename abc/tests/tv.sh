#!/bin/bash

# g : 'b' | 'c' ['a'-'f'] ;
# the range compression should occur on ['a', 'd', 'e', 'f']
echo -n "b"  > pass.1
echo -n "ca" > pass.2
echo -n "cb" > pass.3
echo -n "cc" > pass.4
echo -n "cd" > pass.5
echo -n "ce" > pass.6
echo -n "cf" > pass.7

echo -n "a"   > fail.1
echo -n "c"   > fail.2
echo -n "f"   > fail.3
echo -n "cg"  > fail.4
echo -n "cab" > fail.5


