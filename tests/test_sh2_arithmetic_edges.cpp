#include "../src/saturn/sh2_core.hpp"
#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>

using namespace shining_saturn;

class TestBus final : public Bus {
public:
    std::array<std::uint8_t, 4096> bytes{};
    std::uint8_t read8(std::uint32_t address) override { return bytes.at(address); }
    void write8(std::uint32_t address, std::uint8_t value) override { bytes.at(address) = value; }
    void put16(std::uint32_t address, std::uint16_t opcode) { write16(address, opcode); }
};

int main() {
    TestBus bus;
    Sh2Core cpu;
    cpu.reset(0x100);

    // ADDV R0,R1: signed overflow when INT_MAX + 1 wraps to INT_MIN.
    cpu.r[0] = 1;
    cpu.r[1] = 0x7fffffffu;
    cpu.sr = 0;
    bus.put16(0x100, 0x310f);
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.r[1] == 0x80000000u);
    assert((cpu.sr & Sh2Core::kTBit) != 0);

    // ADDV without overflow must clear T while preserving unrelated SR bits.
    cpu.r[0] = 2;
    cpu.r[1] = 3;
    cpu.sr = 0x100u | Sh2Core::kTBit;
    bus.put16(0x102, 0x310f);
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.r[1] == 5);
    assert((cpu.sr & Sh2Core::kTBit) == 0);
    assert((cpu.sr & 0x100u) != 0);

    // SUBV R0,R1: signed overflow when INT_MIN - 1 wraps to INT_MAX.
    cpu.r[0] = 1;
    cpu.r[1] = 0x80000000u;
    cpu.sr = 0;
    bus.put16(0x104, 0x310b);
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.r[1] == 0x7fffffffu);
    assert((cpu.sr & Sh2Core::kTBit) != 0);

    // SUBV without overflow clears T.
    cpu.r[0] = 2;
    cpu.r[1] = 9;
    cpu.sr = Sh2Core::kTBit;
    bus.put16(0x106, 0x310b);
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.r[1] == 7);
    assert((cpu.sr & Sh2Core::kTBit) == 0);

    // DT decrements then sets T exactly when the result is zero.
    cpu.r[2] = 1;
    bus.put16(0x108, 0x4210);
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.r[2] == 0 && (cpu.sr & Sh2Core::kTBit) != 0);
    cpu.r[2] = 0;
    bus.put16(0x10a, 0x4210);
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.r[2] == 0xffffffffu && (cpu.sr & Sh2Core::kTBit) == 0);

    // CMP/PZ and CMP/PL test signed boundary cases.
    cpu.r[3] = 0x80000000u;
    bus.put16(0x10c, 0x4311);
    assert(cpu.step(bus) == StepResult::Executed);
    assert((cpu.sr & Sh2Core::kTBit) == 0);
    cpu.r[3] = 0;
    bus.put16(0x10e, 0x4311);
    assert(cpu.step(bus) == StepResult::Executed);
    assert((cpu.sr & Sh2Core::kTBit) != 0);
    bus.put16(0x110, 0x4315);
    assert(cpu.step(bus) == StepResult::Executed);
    assert((cpu.sr & Sh2Core::kTBit) == 0);
    cpu.r[3] = 1;
    bus.put16(0x112, 0x4315);
    assert(cpu.step(bus) == StepResult::Executed);
    assert((cpu.sr & Sh2Core::kTBit) != 0);

    // DIV0U clears Q/M/T but preserves unrelated SR bits.
    cpu.sr = 0x00000301u | 0x00000004u;
    bus.put16(0x114, 0x0019);
    assert(cpu.step(bus) == StepResult::Executed);
    assert((cpu.sr & 0x00000301u) == 0);
    assert((cpu.sr & 0x00000004u) != 0);

    // DIV0S: exercise all sign combinations and preserve unrelated SR bits.
    const std::uint32_t signs[][2] = {
        {0u, 0u}, {0x80000000u, 0u},
        {0u, 0x80000000u}, {0x80000000u, 0x80000000u}
    };
    const std::uint32_t expected_flags[] = {0u, 0x00000101u,
                                             0x00000201u, 0x00000300u};
    for (unsigned i = 0; i < 4; ++i) {
        cpu.r[1] = signs[i][0]; // Rn supplies Q
        cpu.r[2] = signs[i][1]; // Rm supplies M
        cpu.sr = 0x00000301u | 0x00000004u;
        bus.put16(0x116 + i * 2, 0x2127); // DIV0S R2,R1
        assert(cpu.step(bus) == StepResult::Executed);
        assert((cpu.sr & 0x00000301u) == expected_flags[i]);
        assert((cpu.sr & 0x00000004u) != 0);
    }

    // CLRT/SETT affect only T and preserve all unrelated SR bits.
    cpu.sr = 0x00000204u | Sh2Core::kTBit;
    bus.put16(0x11e, 0x0008); // CLRT
    assert(cpu.step(bus) == StepResult::Executed);
    assert((cpu.sr & Sh2Core::kTBit) == 0);
    assert((cpu.sr & 0x00000204u) == 0x00000204u);
    bus.put16(0x120, 0x0018); // SETT
    assert(cpu.step(bus) == StepResult::Executed);
    assert((cpu.sr & Sh2Core::kTBit) != 0);
    assert((cpu.sr & 0x00000204u) == 0x00000204u);

    // ADDC: carry-in participates in the result and carry-out updates T.
    cpu.r[1] = 0xffffffffu;
    cpu.r[2] = 0u;
    cpu.sr = 0x00000204u | Sh2Core::kTBit;
    bus.put16(0x122, 0x312e); // ADDC R2,R1
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.r[1] == 0u);
    assert((cpu.sr & Sh2Core::kTBit) != 0);
    assert((cpu.sr & 0x00000204u) == 0x00000204u);

    // ADDC without carry-out clears T.
    cpu.r[1] = 1u;
    cpu.r[2] = 2u;
    cpu.sr = 0x00000204u;
    bus.put16(0x124, 0x312e);
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.r[1] == 3u);
    assert((cpu.sr & Sh2Core::kTBit) == 0);

    // SUBC: borrow-in participates in subtraction and can create a borrow.
    cpu.r[1] = 0u;
    cpu.r[2] = 0u;
    cpu.sr = 0x00000204u | Sh2Core::kTBit;
    bus.put16(0x126, 0x312a); // SUBC R2,R1
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.r[1] == 0xffffffffu);
    assert((cpu.sr & Sh2Core::kTBit) != 0);
    assert((cpu.sr & 0x00000204u) == 0x00000204u);

    // SUBC without borrow clears T.
    cpu.r[1] = 5u;
    cpu.r[2] = 2u;
    cpu.sr = 0x00000204u | Sh2Core::kTBit;
    bus.put16(0x128, 0x312a);
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.r[1] == 2u);
    assert((cpu.sr & Sh2Core::kTBit) == 0);
    assert((cpu.sr & 0x00000204u) == 0x00000204u);

    // CLRMAC clears both halves of the multiply-accumulate register.
    cpu.mach = 0x12345678u;
    cpu.macl = 0x9abcdef0u;
    cpu.sr = 0x00000204u | Sh2Core::kTBit;
    bus.put16(0x12a, 0x0028);
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.mach == 0u && cpu.macl == 0u);
    assert((cpu.sr & (0x00000204u | Sh2Core::kTBit)) ==
           (0x00000204u | Sh2Core::kTBit));

    // DMULU.L writes the full unsigned 64-bit product to MACH:MACL.
    cpu.r[1] = 0xffffffffu;
    cpu.r[2] = 2u;
    bus.put16(0x12c, 0x3125); // DMULU.L R2,R1
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.mach == 1u);
    assert(cpu.macl == 0xfffffffeu);

    // DMULS.L checks the signed high half for a negative product.
    cpu.r[1] = 0xffffffffu; // -1
    cpu.r[2] = 2u;
    bus.put16(0x12e, 0x312d); // DMULS.L R2,R1
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.mach == 0xffffffffu);
    assert(cpu.macl == 0xfffffffeu);

    // Compare instructions update only T and cover signed/unsigned boundaries.
    cpu.r[1] = 0x80000000u;
    cpu.r[2] = 1u;
    cpu.sr = 0x00000204u;
    bus.put16(0x130, 0x3123); // CMP/GE R2,R1 (signed: INT_MIN >= 1 is false)
    assert(cpu.step(bus) == StepResult::Executed);
    assert((cpu.sr & Sh2Core::kTBit) == 0);
    assert((cpu.sr & 0x00000204u) == 0x00000204u);

    cpu.r[1] = 0xffffffffu;
    cpu.r[2] = 1u;
    bus.put16(0x132, 0x3122); // CMP/HS R2,R1 (unsigned: UINT_MAX >= 1)
    assert(cpu.step(bus) == StepResult::Executed);
    assert((cpu.sr & Sh2Core::kTBit) != 0);
    bus.put16(0x134, 0x3126); // CMP/HI R2,R1 (UINT_MAX > 1)
    assert(cpu.step(bus) == StepResult::Executed);
    assert((cpu.sr & Sh2Core::kTBit) != 0);

    cpu.r[1] = 0x80000000u;
    cpu.r[2] = 0x7fffffffu;
    bus.put16(0x136, 0x3127); // CMP/GT R2,R1 (signed: INT_MIN > INT_MAX)
    assert(cpu.step(bus) == StepResult::Executed);
    assert((cpu.sr & Sh2Core::kTBit) == 0);

    cpu.r[1] = 0x12345678u;
    cpu.r[2] = 0x87654378u; // one matching byte is sufficient for CMP/STR
    bus.put16(0x138, 0x212c);
    assert(cpu.step(bus) == StepResult::Executed);
    assert((cpu.sr & Sh2Core::kTBit) != 0);

    // Register logical operations preserve SR, including T and unrelated flags.
    cpu.r[1] = 0xf0f00ff0u;
    cpu.r[2] = 0x0ff0330fu;
    cpu.sr = 0x00000204u | Sh2Core::kTBit;
    bus.put16(0x13a, 0x2129); // AND R2,R1
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.r[1] == 0x00f00300u);
    assert(cpu.sr == (0x00000204u | Sh2Core::kTBit));

    cpu.r[1] = 0xaaaa5555u;
    cpu.r[2] = 0x0f0f0f0fu;
    bus.put16(0x13c, 0x212au); // XOR R2,R1
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.r[1] == 0xa5a55a5au);
    assert(cpu.sr == (0x00000204u | Sh2Core::kTBit));

    cpu.r[1] = 0xf0000000u;
    cpu.r[2] = 0x0000000fu;
    bus.put16(0x13e, 0x212bu); // OR R2,R1
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.r[1] == 0xf000000fu);
    assert(cpu.sr == (0x00000204u | Sh2Core::kTBit));

    // Shift/rotate boundaries update T from the shifted-out bit.
    cpu.r[1] = 0x80000001u;
    cpu.sr = 0x00000204u;
    bus.put16(0x140, 0x4100); // SHLL R1
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.r[1] == 0x00000002u);
    assert((cpu.sr & Sh2Core::kTBit) != 0);
    assert((cpu.sr & 0x00000204u) == 0x00000204u);

    cpu.r[1] = 3u;
    cpu.sr = 0x00000204u;
    bus.put16(0x142, 0x4101); // SHLR R1
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.r[1] == 1u);
    assert((cpu.sr & Sh2Core::kTBit) != 0);

    cpu.r[1] = 0x80000000u;
    cpu.sr = 0x00000204u | Sh2Core::kTBit;
    bus.put16(0x144, 0x4124); // ROTCL R1
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.r[1] == 1u);
    assert((cpu.sr & Sh2Core::kTBit) != 0);
    assert((cpu.sr & 0x00000204u) == 0x00000204u);

    cpu.r[1] = 1u;
    cpu.sr = 0x00000204u | Sh2Core::kTBit;
    bus.put16(0x146, 0x4125); // ROTCR R1
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.r[1] == 0x80000000u);
    assert((cpu.sr & Sh2Core::kTBit) != 0);
    assert((cpu.sr & 0x00000204u) == 0x00000204u);

    // DIV1 Rm,Rn: one unsigned division step updates Q/T and preserves M.
    // Q=0, M=0, positive shifted dividend; subtraction does not borrow.
    cpu.reset(0x148);
    cpu.r[1] = 0x00000003u;
    cpu.r[2] = 0x00000001u;
    cpu.sr = 0x00000004u;
    bus.put16(0x148, 0x3124); // DIV1 R2,R1
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.r[1] == 0x00000005u);
    assert((cpu.sr & 0x00000301u) == 0x00000001u);
    assert((cpu.sr & 0x00000004u) != 0);

    // Q=0, M=1 follows the add path and keeps M unchanged.
    cpu.reset(0x14a);
    cpu.r[1] = 0x00000003u;
    cpu.r[2] = 0x00000001u;
    cpu.sr = 0x00000200u;
    bus.put16(0x14a, 0x3124);
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.r[1] == 0x00000007u);
    assert((cpu.sr & 0x00000200u) != 0);

    // TAS.B @Rn tests the previous byte for zero, then sets its sign bit in memory.
    cpu.reset(0x14c);
    cpu.r[3] = 0x200;
    bus.write8(0x200, 0x00);
    cpu.sr = 0x00000204u;
    bus.put16(0x14c, 0x431b); // TAS.B @R3
    assert(cpu.step(bus) == StepResult::Executed);
    assert(bus.read8(0x200) == 0x80u);
    assert((cpu.sr & Sh2Core::kTBit) != 0);
    assert((cpu.sr & 0x00000204u) == 0x00000204u);

    // A nonzero byte clears T and still sets the high bit without touching other bytes.
    cpu.reset(0x14e);
    cpu.r[3] = 0x201;
    bus.write8(0x201, 0x35);
    cpu.sr = 0x00000204u | Sh2Core::kTBit;
    bus.put16(0x14e, 0x431b);
    assert(cpu.step(bus) == StepResult::Executed);
    assert(bus.read8(0x201) == 0xb5u);
    assert((cpu.sr & Sh2Core::kTBit) == 0);
    assert((cpu.sr & 0x00000204u) == 0x00000204u);

    std::cout << "SH-2 arithmetic edge-case tests passed\n";
    return 0;
}
