#! /usr/bin/env bash
cd "$(dirname "$0")" || exit
matrix_a="${1:-"../test/matrix_a.csv"}"
matrix_b="${2:-"../test/matrix_b.csv"}"
result="${3:-"../test/result.csv"}"

python matmul.py "$matrix_a" "$matrix_b" "$result"
