#include "../src/saturn/sh2_core.hpp"
#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>

using namespace shining_saturn;

class MemoryOpsBus final : public Bus {
public:
    std::array<std::uint8_t, 4096> bytes{};
    std::uint8_t read8(std::uint32_t address) override { return bytes.at(address); }
    void write8(std::uint32_t address, std::uint8_t value) override { bytes.at(address) = value; }
    void put16(std::uint32_t address, std::uint16_t opcode) { write16(address, opcode); }
};

int main() {
    MemoryOpsBus bus;
    Sh2Core cpu;
    cpu.reset(0x100);

    // Signed byte/word loads through Rm, with post-increment.
    bus.write8(0x300, 0x80);
    bus.write16(0x301, 0x8001);
    bus.write32(0x303, 0x1234abcd);
    cpu.r[3] = 0x300;
    bus.put16(0x100, 0x6234); // MOV.B @R3+,R2
    bus.put16(0x102, 0x6235); // MOV.W @R3+,R2
    bus.put16(0x104, 0x6236); // MOV.L @R3+,R2
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[2] == 0xffffff80u && cpu.r[3] == 0x301);
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[2] == 0xffff8001u && cpu.r[3] == 0x303);
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[2] == 0x1234abcdu && cpu.r[3] == 0x307);

    // Pre-decrement stores must update the address before writing big-endian data.
    cpu.r[4] = 0x380;
    cpu.r[5] = 0x89abcdefu;
    bus.put16(0x106, 0x2454); // MOV.B R5,@-R4
    bus.put16(0x108, 0x2455); // MOV.W R5,@-R4
    bus.put16(0x10a, 0x2456); // MOV.L R5,@-R4
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[4] == 0x37f && bus.read8(0x37f) == 0xef);
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[4] == 0x37d && bus.read16(0x37d) == 0xcdef);
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[4] == 0x379 && bus.read32(0x379) == 0x89abcdefu);

    // Direct stores preserve register values and byte order.
    cpu.r[6] = 0x3a0;
    cpu.r[7] = 0x76543210u;
    bus.put16(0x10c, 0x2670); // MOV.B R7,@R6
    bus.put16(0x10e, 0x2671); // MOV.W R7,@R6
    bus.put16(0x110, 0x2672); // MOV.L R7,@R6
    assert(cpu.step(bus) == StepResult::Executed && bus.read8(0x3a0) == 0x10);
    assert(cpu.step(bus) == StepResult::Executed && bus.read16(0x3a0) == 0x3210);
    assert(cpu.step(bus) == StepResult::Executed && bus.read32(0x3a0) == 0x76543210u);
    assert(cpu.r[6] == 0x3a0 && cpu.r[7] == 0x76543210u);

    std::cout << "SH-2 memory operation tests passed\n";
    return 0;
}
