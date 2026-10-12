#!/usr/bin/env bash
set -euo pipefail
root="$(cd "$(dirname "$0")/.." && pwd)"
tmpdir="$(mktemp -d)"; trap 'rm -rf "$tmpdir"' EXIT
cxx="${CXX:-g++}"; flags=(-std=c++17 -Wall -Wextra -Werror -pedantic)
"$cxx" "${flags[@]}" "$root/src/saturn/sh2_core.cpp" "$root/tests/test_sh2_core.cpp" -o "$tmpdir/test_sh2_core"; "$tmpdir/test_sh2_core"
"$cxx" "${flags[@]}" "$root/src/saturn/sh2_core.cpp" "$root/src/saturn/memory_bus.cpp" "$root/tests/test_memory_bus.cpp" -o "$tmpdir/test_memory_bus"; "$tmpdir/test_memory_bus"
"$cxx" "${flags[@]}" "$root/src/saturn/game_library.cpp" "$root/tests/test_game_library.cpp" -o "$tmpdir/test_game_library"; "$tmpdir/test_game_library"
