#!/bin/bash
echo "[PACKRAT RUNS]"
echo [BUG1]
./bug1.pr -k
echo [BUG2]
./bug2.pr -k
echo [BUG3]
./bug3.pr -k
echo "---------------"
echo "[LALR RUNS]"
echo [BUG1]
./bug1 -k
echo [BUG2]
./bug2 -k
echo [BUG3]
./bug3 -k
echo "[DONE]"
exit 0
