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

    // BT/S taken: the delay-slot instruction executes before the target.
    cpu.reset(0x300);
    cpu.sr |= Sh2Core::kTBit;
    bus.put16(0x300, 0x8d01); // BT/S +1 -> 0x306
    bus.put16(0x302, 0xe105); // MOV #5,R1 (delay slot)
    bus.put16(0x306, 0xe206); // MOV #6,R2
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.pc == 0x302);
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.r[1] == 5 && cpu.pc == 0x306);
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.r[2] == 6);

    // BT/S not taken: no delay slot is introduced.
    cpu.reset(0x320);
    bus.put16(0x320, 0x8d01);
    bus.put16(0x322, 0xe307);
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.pc == 0x322);
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.r[3] == 7 && cpu.pc == 0x324);

    // BT taken: branch target is computed relative to the instruction after the delay-free branch.
    cpu.reset(0x330);
    cpu.sr |= Sh2Core::kTBit;
    bus.put16(0x330, 0x8901); // BT +1 -> 0x336
    bus.put16(0x332, 0xe307); // skipped MOV #7,R3
    bus.put16(0x336, 0xe308); // MOV #8,R3
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.pc == 0x336);
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.r[3] == 8);

    // BF taken when T is clear.
    cpu.reset(0x338);
    bus.put16(0x338, 0x8b01); // BF +1 -> 0x33e
    bus.put16(0x33a, 0xe407); // skipped MOV #7,R4
    bus.put16(0x33e, 0xe408); // MOV #8,R4
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.pc == 0x33e);
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.r[4] == 8);

    // JMP @R1 honors its delay slot and then transfers to the register target.
    cpu.reset(0x340);
    cpu.r[1] = 0x360;
    bus.put16(0x340, 0x412b); // JMP @R1
    bus.put16(0x342, 0xe408); // MOV #8,R4 (delay slot)
    bus.put16(0x360, 0x0009); // NOP
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.pc == 0x342);
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.r[4] == 8 && cpu.pc == 0x360);

    // JSR @R2 stores the address after the delay slot in PR.
    cpu.reset(0x380);
    cpu.r[2] = 0x3a0;
    bus.put16(0x380, 0x420b); // JSR @R2
    bus.put16(0x382, 0xe509); // MOV #9,R5 (delay slot)
    bus.put16(0x3a0, 0x0009);
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.pr == 0x384 && cpu.pc == 0x382);
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.r[5] == 9 && cpu.pc == 0x3a0);

    std::cout << "SH-2 control-flow tests passed\n";
    return 0;
}
