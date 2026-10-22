#include "../src/saturn/sh2_core.hpp"
#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>

using namespace shining_saturn;

class MovaBus final : public Bus {
public:
    std::array<std::uint8_t, 4096> bytes{};
    std::uint8_t read8(std::uint32_t address) override { return bytes.at(address); }
    void write8(std::uint32_t address, std::uint8_t value) override { bytes.at(address) = value; }
    void put16(std::uint32_t address, std::uint16_t opcode) { write16(address, opcode); }
};

int main() {
    MovaBus bus;
    Sh2Core cpu;
    cpu.reset(0x100);

    // MOVA uses instruction PC + 4, aligned down to a 4-byte boundary.
    bus.put16(0x100, 0xc700); // MOVA @(0,PC),R0 -> 0x104
    bus.put16(0x102, 0xc703); // MOVA @(3,PC),R0 -> 0x110
    bus.put16(0x104, 0xc7ff); // MOVA @(255,PC),R0 -> 0x504
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.r[0] == 0x104u);
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.r[0] == 0x110u);
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.r[0] == 0x504u);

    std::cout << "SH-2 MOVA PC-relative addressing tests passed\n";
    return 0;
}
