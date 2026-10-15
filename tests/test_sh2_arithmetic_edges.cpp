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

    std::cout << "SH-2 arithmetic edge-case tests passed\n";
    return 0;
}
