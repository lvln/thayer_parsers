#!/bin/sh

if [ ! -d "hdrs" ]; then
		mkdir hdrs
		echo "[hdrs]"
fi
if [ ! -d "pkts" ]; then 
		mkdir pkts
	  echo "[pkts]"
fi

# do just the headers
echo "[HEADER CONVERSIONS]"
for f in pass.*; do
		echo "[$f]"
		od -t x1 $f
		./bit2byte $f ./hdrs/$f
		od -t x1 tests/$f
		od -t x1 hdrs/$f
		echo "---"
done
for f in fail.*; do
		echo "[$f]"
		od -t x1 $f
		./bit2byte $f ./hdrs/$f
		od -t x1 tests/$f
		od -t x1 hdrs/$f
		echo "---"
done
# do the entire packets
echo "[PACKET CONVERSIONS]"
for f in pass.*; do
		echo "[$f]"
		od -t x1 $f
		./bit2byte $f ./pkts/$f -p
		od -t x1 tests/$f
		od -t x1 pkts/$f
		echo "---"
done
for f in fail.*; do
		echo "[$f]"
		od -t x1 $f
		./bit2byte $f ./pkts/$f -p
		od -t x1 tests/$f
		od -t x1 pkts/$f
		echo "---"
done
