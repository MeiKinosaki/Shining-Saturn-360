#include "../src/saturn/sh2_core.hpp"
#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>

using namespace shining_saturn;

class TstBus final : public Bus {
public:
    std::array<std::uint8_t, 1024> bytes{};
    std::uint8_t read8(std::uint32_t address) override { return bytes.at(address); }
    void write8(std::uint32_t address, std::uint8_t value) override { bytes.at(address) = value; }
    void put16(std::uint32_t address, std::uint16_t opcode) { write16(address, opcode); }
};

int main() {
    TstBus bus;
    Sh2Core cpu;
    cpu.reset(0x100);
    bus.put16(0x100, 0x2128); // TST R2,R1
    cpu.r[1] = 0x0f00u; cpu.r[2] = 0x00f0u; cpu.sr = 0;
    assert(cpu.step(bus) == StepResult::Executed);
    assert((cpu.sr & Sh2Core::kTBit) != 0);
    assert(cpu.r[1] == 0x0f00u && cpu.r[2] == 0x00f0u);

    bus.put16(0x102, 0x2128);
    cpu.r[1] = 0x1234u; cpu.r[2] = 0x0030u; cpu.sr = Sh2Core::kTBit;
    assert(cpu.step(bus) == StepResult::Executed);
    assert((cpu.sr & Sh2Core::kTBit) == 0);
    assert(cpu.r[1] == 0x1234u && cpu.r[2] == 0x0030u);

    std::cout << "SH-2 TST register tests passed\n";
    return 0;
}
