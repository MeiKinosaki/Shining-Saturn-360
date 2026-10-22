#!/usr/bin/env bash
set -euo pipefail
root="$(cd "$(dirname "$0")/.." && pwd)"
tmpdir="$(mktemp -d)"
trap 'rm -rf "$tmpdir"' EXIT
cxx="${CXX:-g++}"
flags=(-std=c++17 -O2 -Wall -Wextra -Werror -pedantic)
"$cxx" "${flags[@]}" "$root/src/saturn/sh2_core.cpp" "$root/tests/benchmark_sh2_core.cpp" -o "$tmpdir/benchmark_sh2_core"
"$tmpdir/benchmark_sh2_core"
