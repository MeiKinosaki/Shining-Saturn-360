#!/usr/bin/env bash
set -euo pipefail
root="$(cd "$(dirname "$0")/.." && pwd)"
tmpdir="$(mktemp -d)"; trap 'rm -rf "$tmpdir"' EXIT
cxx="${CXX:-g++}"; flags=(-std=c++17 -Wall -Wextra -Werror -pedantic)
"$cxx" "${flags[@]}" "$root/src/saturn/sh2_core.cpp" "$root/tests/test_sh2_core.cpp" -o "$tmpdir/test_sh2_core"; "$tmpdir/test_sh2_core"
"$cxx" "${flags[@]}" "$root/src/saturn/sh2_core.cpp" "$root/src/saturn/memory_bus.cpp" "$root/tests/test_memory_bus.cpp" -o "$tmpdir/test_memory_bus"; "$tmpdir/test_memory_bus"
"$cxx" "${flags[@]}" "$root/src/saturn/game_library.cpp" "$root/tests/test_game_library.cpp" -o "$tmpdir/test_game_library"; "$tmpdir/test_game_library"
"$cxx" "${flags[@]}" "$root/src/saturn/menu_music.cpp" "$root/tests/test_menu_music.cpp" -o "$tmpdir/test_menu_music"; "$tmpdir/test_menu_music"

"$cxx" "${flags[@]}" "$root/src/saturn/game_library.cpp" "$root/src/saturn/rom_scanner.cpp" "$root/tests/test_rom_scanner.cpp" -o "$tmpdir/test_rom_scanner"; "$tmpdir/test_rom_scanner"
"$cxx" "${flags[@]}" "$root/src/saturn/settings.cpp" "$root/tests/test_settings.cpp" -o "$tmpdir/test_settings"; "$tmpdir/test_settings"
"$cxx" "${flags[@]}" "$root/tests/test_saturn_memory_map.cpp" -o "$tmpdir/test_saturn_memory_map"; "$tmpdir/test_saturn_memory_map"
"$cxx" "${flags[@]}" "$root/tests/test_saturn_address_decoder.cpp" -o "$tmpdir/test_saturn_address_decoder"; "$tmpdir/test_saturn_address_decoder"
"$cxx" "${flags[@]}" "$root/src/saturn/memory_bus.cpp" "$root/src/saturn/saturn_memory_bus.cpp" "$root/tests/test_saturn_memory_bus.cpp" -o "$tmpdir/test_saturn_memory_bus"; "$tmpdir/test_saturn_memory_bus"
"$cxx" "${flags[@]}" "$root/src/saturn/sh2_core.cpp" "$root/src/saturn/memory_bus.cpp" "$root/src/saturn/saturn_memory_bus.cpp" "$root/tests/test_sh2_saturn_bus_integration.cpp" -o "$tmpdir/test_sh2_saturn_bus_integration"; "$tmpdir/test_sh2_saturn_bus_integration"
"$cxx" "${flags[@]}" "$root/src/saturn/sh2_core.cpp" "$root/src/saturn/sh2_runner.cpp" "$root/src/saturn/memory_bus.cpp" "$root/src/saturn/saturn_memory_bus.cpp" "$root/tests/test_sh2_runner.cpp" -o "$tmpdir/test_sh2_runner"; "$tmpdir/test_sh2_runner"
