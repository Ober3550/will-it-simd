#! /usr/bin/env bash
cd "$(dirname "$0")" || exit

size=$1

# Don't regenerate test data if it already exists
if [ -f matrix_a_"$size".csv ] && [ -f matrix_b_"$size".csv ]; then
    exit 0
fi

python ./matrix_generator.py matrix_a "$size"
python ./matrix_generator.py matrix_b "$size"