#!/bin/bash
echo "[PASSING TESTS]"
./gmr ./tests/t1.input -d &> ./tests/t1.trace
./gmr ./tests/t2.input -d &> ./tests/t2.trace
./gmr ./tests/t3.input -d &> ./tests/t3.trace
./gmr ./tests/t4.input -d &> ./tests/t4.trace
./gmr ./tests/t5.input -d &> ./tests/t5.trace

echo ""

echo "[FAILING TESTS]"
./gmr ./tests/t100.input -d &> ./tests/t100.trace
./gmr ./tests/t101.input -d &> ./tests/t101.trace
./gmr ./tests/t102.input -d &> ./tests/t102.trace

