#!/usr/bin/env bash
set -euo pipefail
root="$(cd "$(dirname "$0")/.." && pwd)"
tmpdir="$(mktemp -d)"; trap 'rm -rf "$tmpdir"' EXIT
cxx="${CXX:-g++}"; flags=(-std=c++17 -Wall -Wextra -Werror -pedantic)
"$cxx" "${flags[@]}" "$root/src/saturn/sh2_core.cpp" "$root/tests/test_sh2_core.cpp" -o "$tmpdir/test_sh2_core"; "$tmpdir/test_sh2_core"
"$cxx" "${flags[@]}" "$root/src/saturn/sh2_core.cpp" "$root/tests/test_sh2_memory_ops.cpp" -o "$tmpdir/test_sh2_memory_ops"; "$tmpdir/test_sh2_memory_ops"

"$cxx" "${flags[@]}" "$root/src/saturn/sh2_core.cpp" "$root/tests/test_sh2_mova.cpp" -o "$tmpdir/test_sh2_mova"; "$tmpdir/test_sh2_mova"
"$cxx" "${flags[@]}" "$root/src/saturn/sh2_core.cpp" "$root/tests/test_sh2_tst_register.cpp" -o "$tmpdir/test_sh2_tst_register"; "$tmpdir/test_sh2_tst_register"
"$cxx" "${flags[@]}" "$root/src/saturn/sh2_core.cpp" "$root/tests/test_sh2_clrmac.cpp" -o "$tmpdir/test_sh2_clrmac"; "$tmpdir/test_sh2_clrmac"
"$cxx" "${flags[@]}" "$root/src/saturn/sh2_core.cpp" "$root/tests/test_sh2_arithmetic_edges.cpp" -o "$tmpdir/test_sh2_arithmetic_edges"; "$tmpdir/test_sh2_arithmetic_edges"
"$cxx" "${flags[@]}" "$root/src/saturn/sh2_core.cpp" "$root/tests/test_sh2_sr_flags.cpp" -o "$tmpdir/test_sh2_sr_flags"; "$tmpdir/test_sh2_sr_flags"
"$cxx" "${flags[@]}" "$root/src/saturn/sh2_core.cpp" "$root/tests/test_sh2_system_registers.cpp" -o "$tmpdir/test_sh2_system_registers"; "$tmpdir/test_sh2_system_registers"
"$cxx" "${flags[@]}" "$root/src/saturn/sh2_core.cpp" "$root/tests/test_sh2_mac_registers.cpp" -o "$tmpdir/test_sh2_mac_registers"; "$tmpdir/test_sh2_mac_registers"
"$cxx" "${flags[@]}" "$root/src/saturn/sh2_core.cpp" "$root/tests/test_sh2_system_memory_transfers.cpp" -o "$tmpdir/test_sh2_system_memory_transfers"; "$tmpdir/test_sh2_system_memory_transfers"
"$cxx" "${flags[@]}" "$root/src/saturn/sh2_core.cpp" "$root/tests/test_sh2_delay_slots.cpp" -o "$tmpdir/test_sh2_delay_slots"; "$tmpdir/test_sh2_delay_slots"
"$cxx" "${flags[@]}" "$root/src/saturn/sh2_core.cpp" "$root/tests/test_sh2_control_flow.cpp" -o "$tmpdir/test_sh2_control_flow"; "$tmpdir/test_sh2_control_flow"
"$cxx" "${flags[@]}" "$root/src/saturn/sh2_core.cpp" "$root/src/saturn/memory_bus.cpp" "$root/tests/test_memory_bus.cpp" -o "$tmpdir/test_memory_bus"; "$tmpdir/test_memory_bus"
"$cxx" "${flags[@]}" "$root/src/saturn/game_library.cpp" "$root/tests/test_game_library.cpp" -o "$tmpdir/test_game_library"; "$tmpdir/test_game_library"
"$cxx" "${flags[@]}" "$root/src/saturn/menu_music.cpp" "$root/tests/test_menu_music.cpp" -o "$tmpdir/test_menu_music"; "$tmpdir/test_menu_music"

"$cxx" "${flags[@]}" "$root/src/saturn/game_library.cpp" "$root/src/saturn/rom_scanner.cpp" "$root/tests/test_rom_scanner.cpp" -o "$tmpdir/test_rom_scanner"; "$tmpdir/test_rom_scanner"
"$cxx" "${flags[@]}" "$root/src/saturn/settings.cpp" "$root/tests/test_settings.cpp" -o "$tmpdir/test_settings"; "$tmpdir/test_settings"
"$cxx" "${flags[@]}" "$root/tests/test_saturn_memory_map.cpp" -o "$tmpdir/test_saturn_memory_map"; "$tmpdir/test_saturn_memory_map"
"$cxx" "${flags[@]}" "$root/tests/test_saturn_address_decoder.cpp" -o "$tmpdir/test_saturn_address_decoder"; "$tmpdir/test_saturn_address_decoder"
"$cxx" "${flags[@]}" "$root/src/saturn/sh2_core.cpp" "$root/src/saturn/memory_bus.cpp" "$root/src/saturn/saturn_mmio.cpp" "$root/src/saturn/saturn_memory_bus.cpp" "$root/tests/test_saturn_memory_bus.cpp" -o "$tmpdir/test_saturn_memory_bus"; "$tmpdir/test_saturn_memory_bus"
"$cxx" "${flags[@]}" "$root/src/saturn/sh2_core.cpp" "$root/src/saturn/memory_bus.cpp" "$root/src/saturn/saturn_mmio.cpp" "$root/src/saturn/saturn_memory_bus.cpp" "$root/tests/test_sh2_saturn_bus_integration.cpp" -o "$tmpdir/test_sh2_saturn_bus_integration"; "$tmpdir/test_sh2_saturn_bus_integration"
"$cxx" "${flags[@]}" "$root/src/saturn/sh2_core.cpp" "$root/src/saturn/sh2_runner.cpp" "$root/src/saturn/memory_bus.cpp" "$root/src/saturn/saturn_mmio.cpp" "$root/src/saturn/saturn_memory_bus.cpp" "$root/tests/test_sh2_runner.cpp" -o "$tmpdir/test_sh2_runner"; "$tmpdir/test_sh2_runner"
"$cxx" "${flags[@]}" "$root/src/saturn/saturn_mmio.cpp" "$root/tests/test_saturn_mmio.cpp" -o "$tmpdir/test_saturn_mmio"; "$tmpdir/test_saturn_mmio"

"$cxx" "${flags[@]}" "$root/src/saturn/sh2_core.cpp" "$root/src/saturn/sh2_runner.cpp" "$root/src/saturn/memory_bus.cpp" "$root/src/saturn/saturn_mmio.cpp" "$root/src/saturn/saturn_memory_bus.cpp" "$root/src/saturn/saturn_main_loop.cpp" "$root/tests/test_saturn_main_loop.cpp" -o "$tmpdir/test_saturn_main_loop"; "$tmpdir/test_saturn_main_loop"

"$cxx" "${flags[@]}" "$root/src/saturn/vdp1_renderer.cpp" "$root/tests/test_vdp1_renderer.cpp" -o "$tmpdir/test_vdp1_renderer"; "$tmpdir/test_vdp1_renderer"

"$cxx" "${flags[@]}" "$root/src/saturn/vdp1_renderer.cpp" "$root/src/saturn/vdp1_command_list.cpp" "$root/tests/test_vdp1_command_list.cpp" -o "$tmpdir/test_vdp1_command_list"; "$tmpdir/test_vdp1_command_list"

"$cxx" "${flags[@]}" "$root/src/saturn/vdp2_compositor.cpp" "$root/tests/test_vdp2_compositor.cpp" -o "$tmpdir/test_vdp2_compositor"; "$tmpdir/test_vdp2_compositor"
"$cxx" "${flags[@]}" "$root/src/saturn/scu_dma.cpp" "$root/tests/test_scu_dma.cpp" -o "$tmpdir/test_scu_dma"; "$tmpdir/test_scu_dma"
"$cxx" "${flags[@]}" "$root/src/saturn/sh2_core.cpp" "$root/src/saturn/memory_bus.cpp" "$root/src/saturn/saturn_mmio.cpp" "$root/src/saturn/saturn_memory_bus.cpp" "$root/src/saturn/scu_dma.cpp" "$root/tests/test_scu_dma_saturn_bus.cpp" -o "$tmpdir/test_scu_dma_saturn_bus"; "$tmpdir/test_scu_dma_saturn_bus"
