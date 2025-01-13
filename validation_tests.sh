#! /usr/bin/env bash

# Build all apps
./matmul/c/build.sh

# Run against test data
./matmul/c/run.sh ../test/matrix_a.csv ../test/matrix_b.csv ../test/result.csv

# Compare result with expected
cmp ./matmul/test/expected_result.csv ./matmul/test/result.csv && echo "Result and Expected Match"
