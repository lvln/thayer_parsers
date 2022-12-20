#!/bin/bash
echo "[PACKRAT RUNS]"
#echo [BUG1]
#./bug1.pr -k
#echo [BUG2]
#./bug2.pr -k
#echo [BUG3]
#./bug3.pr -k
for f in {1..8}
do
	echo "[BUG$f]"
	./bug${f}.pr -k
done
echo "+----------------------------------------------------------+"
echo "[LALR RUNS]"
#echo [BUG1]
#./bug1 -k
#echo [BUG2]
#./bug2 -k
#echo [BUG3]
#./bug3 -k
for f in {1..8}
do
	echo "[BUG$f]"
	./bug${f} -k
done
echo "[DONE]"
exit 0
