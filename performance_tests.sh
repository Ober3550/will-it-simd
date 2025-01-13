#! /usr/bin/env bash

# Generate test data
size=2000
./matmul/test/generate_test_data.sh $size

#! /usr/bin/env bash

dir_list="$(ls -d matmul/*)"
# shellcheck disable=SC2206
targets=(${dir_list[@]/'matmul/test'/})

for target in "${targets[@]}"; do
  if [[ -f "$target/build.sh" ]]; then
    echo "Building $target"
    "$target/build.sh"
  fi
  if [[ -f "$target/run.sh" ]]; then
    echo "Running $target"
    time "$target/run.sh" ../test/matrix_a_"$size".csv ../test/matrix_b_"$size".csv ../test/result.csv
    echo
  fi
done
