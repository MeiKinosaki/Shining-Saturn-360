# CI device benchmark result — v0.48b workstream

Run: https://github.com/MeiKinosaki/Shining-Saturn-360/actions/runs/40430823323
Commit tested: `290890d48e56316c61122bb8c4cacbe0f4a5d85a`
Workflow result: **success** (2026-10-27 UTC).
Artifact: [saturn-device-benchmark-report](https://github.com/MeiKinosaki/Shining-Saturn-360/actions/runs/40430823323/artifacts/12702490670)

## Recorded host-side results

| Workload | Input | Elapsed seconds | Check |
|---|---:|---:|---|
| SCU DMA prototype copy | 65,536 bytes | 0.000372443 | Byte-by-byte destination equality; completion and remaining-count assertions |
| VDP1 software primitives | 20 iterations | 0.00136835 | 34,689 nonzero pixels; checksum 11240337932454447975 |
| VDP2 software composition | 20 iterations | 0.00154082 | Checksum 11196090878927114352 |

DMA checksum: `16352662612197835651`.

## Scope and caveats

- This run confirms that the host-side benchmark program compiled, ran, checked its prototype outputs, and uploaded a report.
- These elapsed times are from a GitHub-hosted runner. They are not Xbox 360 timings and should not be compared as hardware performance measurements across different runner instances.
- The DMA class explicitly remains a deterministic byte-copy prototype, not a register-accurate Saturn SCU. VDP1 and VDP2 benchmarks exercise software prototypes, not full hardware-accurate graphics behavior.
- The full workflow also reports success for host regression tests and ELF32/XEX packaging. A generated XEX is not proof of booting on a physical Xbox 360, nor does it prove Saturn game compatibility.
- This report does not mark the overall v0.48b gate complete. SH-2 exception/interrupt behavior, full-machine save/load state, real device integration, and physical Xenon execution remain separate acceptance requirements.
