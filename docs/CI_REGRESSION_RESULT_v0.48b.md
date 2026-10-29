# CI regression validation — v0.48b workstream

- Latest workflow run: https://github.com/MeiKinosaki/Shining-Saturn-360/actions/runs/40832030192
- Tested commit: `7a3d076ff8e8c6060ca4ebdb715b2146afcc8a93`
- Result: **success** (2026-10-29 UTC).
- Repository version file remains `v0.47b`; this is evidence for the next workstream, not a version promotion.

## Successful workflow jobs

- `prepare-source`
- `sh2-profile-smoke`
- `saturn-device-benchmarks`
- `host-regression-tests`
- `build-xenon-elf32`
- `package-xex`

The optional `report-no-makefile` job was skipped, not failed.

## Relevant automated coverage in the host regression runner

- SH-2 interpreter core and unsupported-opcode diagnostics, including an unsupported opcode in a branch delay slot; verifies fault PC/opcode, deterministic halt behavior and that the pending branch target is not applied after the fault.
- SH-2 snapshots/state codec, memory operations, system registers and selected instruction edge cases.
- Saturn memory bus/MMIO integration and main-loop prototype.
- SCU DMA prototype, including byte-copy integrity, bounded stepping, completion, zero-byte budgets, reset behavior and channel scheduling.
- VDP1 renderer/command-list and VDP2 compositor prototype tests.
- Game library, ROM scanner, cover download, menu music and settings tests.

## What this does and does not prove

This run confirms the listed CI jobs completed successfully for the tested commit, including host regression tests, an ELF32 build stage and XEX packaging. It does **not** prove that the XEX boots on a physical Xbox 360, that the emulator runs commercial Saturn games, or that all Saturn hardware behavior is implemented.

The SH-2 unsupported-opcode tests verify diagnostics and state handling. They are not tests of architectural exception vectors or interrupt delivery. SCU DMA and VDP benchmark/test code remains prototype-level and does not establish register-accurate Saturn hardware emulation.

## Remaining v0.48b acceptance gates

1. Implement and test SH-2 exception entry and interrupt delivery against defined vector/priority behavior.
2. Integrate and validate device register behavior beyond prototype memory-copy/render paths.
3. Serialize and restore the complete machine state, including RAM and device state, with round-trip tests.
4. Validate Xenon execution on actual hardware or explicitly keep physical execution marked unverified.
5. Run the complete regression workflow after each integration change.

Until the acceptance gates are satisfied and reviewed, retain official version `v0.47b`.
