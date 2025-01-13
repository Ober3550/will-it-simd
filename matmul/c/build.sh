#! /usr/bin/env bash
cd "$(dirname "$0")" || exit
clang -O3 matmul.c -o matmul_c