#!/usr/bin/env bash
set -euo pipefail
root="$(cd "$(dirname "$0")/.." && pwd)"
tmpdir="$(mktemp -d)"
trap 'rm -rf "$tmpdir"' EXIT
cxx="${CXX:-g++}"
flags=(-std=c++17 -O2 -Wall -Wextra -Werror -pedantic)
"$cxx" "${flags[@]}" \
  "$root/src/saturn/scu_dma.cpp" \
  "$root/src/saturn/vdp1_renderer.cpp" \
  "$root/src/saturn/vdp2_compositor.cpp" \
  "$root/tests/benchmark_saturn_devices.cpp" \
  -o "$tmpdir/benchmark_saturn_devices"
"$tmpdir/benchmark_saturn_devices"
