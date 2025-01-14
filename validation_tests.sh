#! /usr/bin/env bash

dir_list="$(ls -d matmul/*)"
# shellcheck disable=SC2206
targets=(${dir_list[@]/'matmul/test'/})
if [[ -n "$1" ]]; then 
  targets=("matmul/$1")
fi

for target in "${targets[@]}"; do
  if [[ -f "$target/build.sh" ]]; then
    echo "Building $target"
    "$target/build.sh"
  fi
  if [[ -f "$target/run.sh" ]]; then
    echo "Running $target"
    "$target/run.sh" ../test/matrix_a.csv ../test/matrix_b.csv ../test/result.csv
    cmp "./matmul/test/expected_result.csv" "./matmul/test/result.csv" && echo "Result Matrix Matches Expected"
    echo
  fi
done
