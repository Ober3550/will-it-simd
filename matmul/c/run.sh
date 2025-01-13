#! /usr/bin/env bash
cd "$(dirname "$0")" || exit
./matmul_c "$1" "$2" "$3"
