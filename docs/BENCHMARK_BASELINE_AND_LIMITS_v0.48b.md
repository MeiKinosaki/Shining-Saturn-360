# Benchmark interpretation and next measurements — v0.48b

## Existing baseline

`tests/benchmark_sh2_core.cpp` currently runs two fixed workloads of 100,000 instructions:
- `sh2_nop_stream`: instruction-fetch/decode baseline;
- `sh2_movl_indirect_load`: repeated load from a fixed address.

The harness prints elapsed host seconds and instructions/second and checks final PC; the load workload also checks the loaded value. Run it using `tests/run-sh2-benchmark.sh`. These figures are host-side interpreter microbenchmarks only, not Saturn frame rate or Xbox 360 target performance.

## Reproducibility improvements required

- [ ] Print compiler identity/version, build flags, OS/architecture and commit SHA alongside results.
- [ ] Repeat each workload multiple times and report median/min/max, keeping warm-up policy fixed.
- [ ] Emit a stable machine-readable result format for CI comparison.
- [ ] Add checksums or complete observable-state assertions for every workload.
- [ ] Keep NOP and memory workloads separate; don't combine their rates into a single score.
- [ ] Add DMA transfer workload only after its API and completion semantics are stable.
- [ ] Add VDP1 command workload and VDP2 composition workload using deterministic input buffers and output checksums.
- [ ] Measure integrated main-loop frame time separately from isolated device throughput.
- [ ] Run target benchmarks on Xenon hardware before publishing any Xbox 360 performance claim.

## Current interpretation

No VDP1, VDP2, or DMA performance result is established by the existing SH-2 microbenchmark. Do not infer device speed from the CPU-only benchmark. Record every missing benchmark as pending, not as zero cost or a pass.

## Version gate

This document is a measurement plan. It does not claim that benchmark changes, device benchmarks, exception/IRQ handling, whole-machine save states, or the Xenon core link have been implemented. Keep the release marker at v0.47b until code and CI evidence satisfy the v0.48b acceptance criteria.
