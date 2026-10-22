#include "../src/saturn/sh2_core.hpp"
#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>

using namespace shining_saturn;

class ByteBus final : public Bus {
public:
    std::array<std::uint8_t, 4096> bytes{};
    std::uint8_t read8(std::uint32_t address) override { return bytes.at(address); }
    void write8(std::uint32_t address, std::uint8_t value) override { bytes.at(address) = value; }
    void put16(std::uint32_t address, std::uint16_t opcode) { write16(address, opcode); }
};

int main() {
    ByteBus bus;
    Sh2Core cpu;
    cpu.reset(0x100);
    cpu.gbr = 0x200;
    cpu.r[0] = 0x10;

    // TST.B #imm,@(R0,GBR) sets T only when all selected bits are clear.
    bus.write8(0x210, 0xA0);
    bus.put16(0x100, 0xCC0F); // TST.B #0x0f -> zero
    bus.put16(0x102, 0xCCF0); // TST.B #0xf0 -> nonzero
    assert(cpu.step(bus) == StepResult::Executed);
    assert((cpu.sr & Sh2Core::kTBit) != 0);
    assert(cpu.step(bus) == StepResult::Executed);
    assert((cpu.sr & Sh2Core::kTBit) == 0);

    // AND.B, XOR.B and OR.B update only the addressed byte.
    bus.put16(0x104, 0xCD3C); // 0xa0 & 0x3c = 0x20
    bus.put16(0x106, 0xCE0F); // 0x20 ^ 0x0f = 0x2f
    bus.put16(0x108, 0xCF80); // 0x2f | 0x80 = 0xaf
    assert(cpu.step(bus) == StepResult::Executed && bus.read8(0x210) == 0x20);
    assert(cpu.step(bus) == StepResult::Executed && bus.read8(0x210) == 0x2f);
    assert(cpu.step(bus) == StepResult::Executed && bus.read8(0x210) == 0xaf);

    // Addressing uses the full R0 offset and GBR, without modifying either.
    assert(cpu.gbr == 0x200 && cpu.r[0] == 0x10);
    assert(bus.read8(0x20f) == 0);
    std::cout << "SH-2 GBR byte logical tests passed\n";
    return 0;
}
