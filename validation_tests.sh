#! /usr/bin/env bash
./matmul/c/build.sh
./matmul/c/run.sh ../test/matrix_a.csv ../test/matrix_b.csv ../test/result.csv
cmp ./matmul/test/expected_result.csv ./matmul/test/result.csv && echo "Result and Expected Match"
