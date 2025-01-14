#! /usr/bin/env bash
cd "$(dirname "$0")" || exit
zig build-exe matmul.zig -O ReleaseFast --name matmul_zig