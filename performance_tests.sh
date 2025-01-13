#! /usr/bin/env bash

# Build all apps
./matmul/c/build.sh

# Generate test data
size=1500
./matmul/test/generate_test_data.sh $size

# Run performance tests
time ./matmul/c/run.sh ../test/matrix_a_"$size".csv ../test/matrix_b_"$size".csv ../test/result.csv
