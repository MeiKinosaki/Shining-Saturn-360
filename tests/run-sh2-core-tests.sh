#!/usr/bin/env bash
set -euo pipefail
root="$(cd "$(dirname "$0")/.." && pwd)"
tmpdir="$(mktemp -d)"
trap 'rm -rf "$tmpdir"' EXIT
"${CXX:-g++}" -std=c++17 -Wall -Wextra -Werror -pedantic \
  "$root/src/saturn/sh2_core.cpp" "$root/tests/test_sh2_core.cpp" \
  -o "$tmpdir/test_sh2_core"
"$tmpdir/test_sh2_core"
