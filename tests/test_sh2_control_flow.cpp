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

    // BT not taken leaves execution at the next sequential instruction.
    cpu.reset(0x3a0);
    bus.put16(0x3a0, 0x8901); // BT +1, T=0 -> not taken
    bus.put16(0x3a2, 0xe607); // MOV #7,R6
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.pc == 0x3a2);
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[6] == 7);

    // BF not taken when T is set.
    cpu.reset(0x3a8);
    cpu.sr |= Sh2Core::kTBit;
    bus.put16(0x3a8, 0x8b01); // BF +1, T=1 -> not taken
    bus.put16(0x3aa, 0xe707); // MOV #7,R7
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.pc == 0x3aa);
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[7] == 7);

    // BF/S taken executes exactly one delay-slot instruction before branching.
    cpu.reset(0x3b0);
    bus.put16(0x3b0, 0x8f01); // BF/S +1 -> 0x3b6, T=0
    bus.put16(0x3b2, 0xe807); // MOV #7,R8 (delay slot)
    bus.put16(0x3b6, 0xe908); // MOV #8,R9
    assert(cpu.step(bus) == StepResult::Executed && cpu.pc == 0x3b2);
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[8] == 7 && cpu.pc == 0x3b6);
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[9] == 8);

    // BF/S not taken does not schedule a delayed transfer.
    cpu.reset(0x3c0);
    cpu.sr |= Sh2Core::kTBit;
    bus.put16(0x3c0, 0x8f01);
    bus.put16(0x3c2, 0xea09); // MOV #9,R10
    assert(cpu.step(bus) == StepResult::Executed && cpu.pc == 0x3c2);
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[10] == 9 && cpu.pc == 0x3c4);

    // Signed negative BT displacement wraps back to the branch instruction.
    cpu.reset(0x3d0);
    cpu.sr |= Sh2Core::kTBit;
    bus.put16(0x3d0, 0x89fe); // BT -2 -> 0x3d0
    assert(cpu.step(bus) == StepResult::Executed && cpu.pc == 0x3d0);

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

    // BRA executes the delay slot before applying a signed displacement.
    cpu.reset(0x400);
    bus.put16(0x400, 0xafff); // BRA -1 -> 0x402 (PC+4-2)
    bus.put16(0x402, 0xeb0a); // MOV #10,R11 (delay slot)
    assert(cpu.step(bus) == StepResult::Executed && cpu.pc == 0x402);
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.r[11] == 10 && cpu.pc == 0x402);

    // BSR captures the return address after the delay slot.
    cpu.reset(0x420);
    bus.put16(0x420, 0xb001); // BSR +1 -> 0x426
    bus.put16(0x422, 0xec0b); // MOV #11,R12 (delay slot)
    bus.put16(0x426, 0x0009); // NOP target
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.pr == 0x424 && cpu.pc == 0x422);
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.r[12] == 11 && cpu.pc == 0x426);

    // BRAF computes its target from the instruction after the delay slot.
    cpu.reset(0x440);
    cpu.r[3] = 6;
    bus.put16(0x440, 0x0323); // BRAF R3
    bus.put16(0x442, 0xed0c); // MOV #12,R13 (delay slot)
    bus.put16(0x44a, 0x0009); // target = 0x444 + 6
    assert(cpu.step(bus) == StepResult::Executed && cpu.pc == 0x442);
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.r[13] == 12 && cpu.pc == 0x44a);

    // BSRF also records PR while using a register-relative target.
    cpu.reset(0x460);
    cpu.r[4] = 8;
    bus.put16(0x460, 0x0403); // BSRF R4
    bus.put16(0x462, 0xee0d); // MOV #13,R14 (delay slot)
    bus.put16(0x46c, 0x0009); // target = 0x464 + 8
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.pr == 0x464 && cpu.pc == 0x462);
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.r[14] == 13 && cpu.pc == 0x46c);

    // RTS waits for its delay slot before returning to PR.
    cpu.reset(0x480);
    cpu.pr = 0x4a0;
    bus.put16(0x480, 0x000b); // RTS
    bus.put16(0x482, 0xef0e); // MOV #14,R15 (delay slot)
    bus.put16(0x4a0, 0x0009);
    assert(cpu.step(bus) == StepResult::Executed && cpu.pc == 0x482);
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.r[15] == 14 && cpu.pc == 0x4a0);

    std::cout << "SH-2 control-flow tests passed\n";
    return 0;
}
