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

    // BRA +1: target is 0x106, but the instruction at 0x102 executes first.
    bus.put16(0x100, 0xa001);
    bus.put16(0x102, 0xe102); // MOV #2,R1 -- delay slot
    bus.put16(0x106, 0xe203); // MOV #3,R2 -- branch target
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.pc == 0x102);
    assert(cpu.r[1] == 0);
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.r[1] == 2);
    assert(cpu.pc == 0x106);
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.r[2] == 3);
    assert(cpu.pc == 0x108);

    // BSR +1 sets PR to the instruction after the delay slot and branches after it.
    cpu.reset(0x200);
    bus.put16(0x200, 0xb001);
    bus.put16(0x202, 0xe304); // MOV #4,R3 -- delay slot
    bus.put16(0x206, 0x0009); // NOP at target
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.pr == 0x204);
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.r[3] == 4);
    assert(cpu.pc == 0x206);

    std::cout << "SH-2 delayed-branch tests passed\n";
    return 0;
}
