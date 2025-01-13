#! /usr/bin/env bash
cd "$(dirname "$0")" || exit
gcc -O3 matmul.c -o matmul_c