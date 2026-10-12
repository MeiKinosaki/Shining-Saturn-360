#include "../src/saturn/sh2_core.hpp"
#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
using namespace shining_saturn;
class TestBus final : public Bus {
public:
    std::array<std::uint8_t, 65536> bytes{};
    std::uint8_t read8(std::uint32_t a) override { return bytes.at(a); }
    void write8(std::uint32_t a, std::uint8_t v) override { bytes.at(a) = v; }
    void put16(std::uint32_t a, std::uint16_t op) { write16(a, op); }
};
int main() {
    TestBus bus; Sh2Core cpu; cpu.reset(0x100);
    // MOV #5,R1; ADD #3,R1; MOV #5,R2; CMP/EQ R2,R1; NOP
    bus.put16(0x100, 0xe105); bus.put16(0x102, 0x7103);
    bus.put16(0x104, 0xe205); bus.put16(0x106, 0x3120);
    bus.put16(0x108, 0x0009);
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[1] == 5);
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[1] == 8);
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[2] == 5);
    assert(cpu.step(bus) == StepResult::Executed && (cpu.sr & Sh2Core::kTBit) == 0);
    assert(cpu.step(bus) == StepResult::Executed);

    // CLRT/SETT/MOVT and shift instructions.
    bus.put16(0x10a, 0x0008); // CLRT
    bus.put16(0x10c, 0x0018); // SETT
    bus.put16(0x10e, 0x0029); // MOVT R0
    bus.put16(0x110, 0x4100); // SHLL R1
    assert(cpu.step(bus) == StepResult::Executed && (cpu.sr & Sh2Core::kTBit) == 0);
    assert(cpu.step(bus) == StepResult::Executed && (cpu.sr & Sh2Core::kTBit) != 0);
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[0] == 1);
    cpu.r[1] = 0x80000001u;
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[1] == 2 &&
           (cpu.sr & Sh2Core::kTBit) != 0);

    bus.write32(0x200, 0x1234abcd);
    assert(bus.read8(0x200) == 0x12 && bus.read16(0x200) == 0x1234);
    assert(bus.read32(0x200) == 0x1234abcd);
    bus.put16(0x112, 0xffff);
    assert(cpu.step(bus) == StepResult::UnsupportedOpcode);
    assert(cpu.fault_pc() == 0x112 && cpu.last_opcode() == 0xffff);
    std::cout << "SH-2 core foundation tests passed\n";
    return 0;
}
