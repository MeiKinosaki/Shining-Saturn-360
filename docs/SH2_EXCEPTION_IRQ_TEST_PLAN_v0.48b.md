# SH-2 exception/interrupt validation plan — v0.48b

## Status and scope

This document defines deterministic tests for the next SH-2 integration milestone. It is a test plan, not evidence that exception or interrupt handling is already implemented. Keep the repository version at v0.47b until implementation, CI, regression, and build integration are verified.

The current `Sh2Snapshot` captures CPU registers and delayed-branch bookkeeping only. It does not capture memory or device state and must not be treated as a whole-machine save state.

## Test harness contract

Use a deterministic fake `Bus` backed by byte-addressable memory. Each test must set initial memory and CPU state explicitly, execute a bounded number of steps/events, and assert all architecturally relevant postconditions. Failures must print test name, initial PC/SR, opcode or event, expected values, and actual values.

Do not silently emulate unsupported behavior as a successful no-op. If an architecture feature is not implemented yet, mark its test as an expected pending case rather than a passing test.

## Exception test matrix

- [ ] **Vector fetch:** configure VBR and the relevant vector table entry; verify the handler address selected by the implemented SH-2 exception model.
- [ ] **Saved state:** assert the specified PC/SR save semantics for each exception class; test stack/register effects and ordering.
- [ ] **Handler entry:** verify PC changes to the vector target and SR changes only as required by the architecture.
- [ ] **Return path:** exercise the appropriate return instruction/path and verify restored control flow and status.
- [ ] **Boundary addresses:** vector table near a memory boundary; reject or deterministically handle unmapped reads according to the bus contract.
- [ ] **Unsupported/illegal opcode:** record faulting PC and opcode, and test the chosen exception behavior once that behavior is implemented.
- [ ] **Delay slot:** raise a relevant event for an instruction in a branch delay slot and assert the architecture-defined saved PC/state.
- [ ] **Repeatability:** run each test repeatedly and compare the complete CPU snapshot and touched memory bytes.

## Interrupt test matrix

- [ ] **Masked IRQ:** an interrupt below/equal to the current SR interrupt mask must not be accepted when the architecture says it is masked.
- [ ] **Unmasked IRQ:** a pending eligible interrupt is accepted at the defined instruction/event boundary.
- [ ] **Priority arbitration:** with multiple pending sources, select the source with the highest eligible priority according to the Saturn/SH-2 contract.
- [ ] **Vector selection:** verify the accepted source resolves to the correct vector/handler.
- [ ] **Mask update:** verify SR interrupt-mask changes on entry and are restored/updated on return as specified.
- [ ] **Pending persistence:** a masked request remains pending if required; acceptance clears or acknowledges it only through the correct source contract.
- [ ] **Simultaneous events:** fixed-priority tie cases are deterministic.
- [ ] **Two SH-2 CPUs:** when the second CPU is introduced, test independent interrupt state and shared-device arbitration separately.

## DMA and VDP benchmark contract

Benchmarks must use fixed synthetic workloads and publish compiler, flags, host/target, iteration count, warm-up policy, elapsed-time method, and output checksum/counters.

Record separate measurements for:
1. DMA transfer setup and completion;
2. bytes transferred and bus-visible side effects;
3. VDP1 command processing and rendered primitive count;
4. VDP2 layer/priority composition and output checksum;
5. integrated frame workload.

Do not report target Xbox 360 performance based only on a host benchmark. A host benchmark is useful for repeatability and regression detection, but target performance requires a measured target run.

## Xenon link integration acceptance

- [ ] Build the current `src/saturn/` core sources in the Xenon target, rather than only a legacy ZIP/bootstrap Makefile.
- [ ] Preserve a link map or symbol listing proving that SH-2, bus, and main-loop objects are included.
- [ ] Record toolchain revision and exact build command.
- [ ] Produce ELF and then XEX from the same commit; validate file structure and SHA-256.
- [ ] Label CI build success separately from a real-console launch test.

## Whole-machine save-state acceptance

A whole-machine state must be versioned and include all active CPU contexts, RAM regions, pending interrupts/events, timing/scheduler state, and each implemented device's registers and internal buffers. Define byte order and stable field encoding; avoid dumping raw C++ object memory.

- [ ] Header magic, format version, payload length, and checksum.
- [ ] Reject truncated, oversized, unknown-version, and corrupted files without mutating the running machine.
- [ ] Save -> mutate CPU/RAM/devices -> load -> compare all serialized fields and memory regions.
- [ ] Verify deterministic continuation for a fixed number of steps after restoration.
- [ ] Document any device not yet serializable; do not call the result a complete system save state while such devices are active but omitted.

## Promotion gate for v0.48b

Promote only after relevant tests are implemented and passing in CI, the full host regression passes on the candidate commit, Xenon link evidence shows the actual core is included, and save-state coverage is reported honestly. Until then, `VERSION` remains `v0.47b`.
