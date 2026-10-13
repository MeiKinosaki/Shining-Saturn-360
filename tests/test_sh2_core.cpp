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

    // Immediate logical operations and TST on R0.
    bus.put16(0x112, 0xc801); // TST #1,R0 -> T=0
    bus.put16(0x114, 0xc901); // AND #1,R0 -> R0=1
    bus.put16(0x116, 0xca02); // XOR #2,R0 -> R0=3
    bus.put16(0x118, 0xcb04); // OR #4,R0 -> R0=7
    bus.put16(0x11a, 0xc807); // TST #7,R0 -> T=0
    bus.put16(0x11c, 0xc808); // TST #8,R0 -> T=1
    assert(cpu.step(bus) == StepResult::Executed && (cpu.sr & Sh2Core::kTBit) == 0);
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[0] == 1);
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[0] == 3);
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[0] == 7);
    assert(cpu.step(bus) == StepResult::Executed && (cpu.sr & Sh2Core::kTBit) == 0);
    assert(cpu.step(bus) == StepResult::Executed && (cpu.sr & Sh2Core::kTBit) != 0);

    // Post-increment loads and pre-decrement stores.
    bus.write8(0x300, 0xfe);
    bus.write16(0x301, 0x8123);
    bus.write32(0x303, 0x1234abcd);
    cpu.r[3] = 0x300;
    bus.put16(0x11e, 0x6234); // MOV.B @R3+,R2
    bus.put16(0x120, 0x6235); // MOV.W @R3+,R2
    bus.put16(0x122, 0x6236); // MOV.L @R3+,R2
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[2] == 0xfffffffeu && cpu.r[3] == 0x301);
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[2] == 0xffff8123u && cpu.r[3] == 0x303);
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[2] == 0x1234abcdu && cpu.r[3] == 0x307);
    cpu.r[4] = 0x400; cpu.r[5] = 0x12345678;
    bus.put16(0x124, 0x2454); // MOV.B R5,@-R4
    bus.put16(0x126, 0x2455); // MOV.W R5,@-R4
    bus.put16(0x128, 0x2456); // MOV.L R5,@-R4
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[4] == 0x3ff && bus.read8(0x3ff) == 0x78);
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[4] == 0x3fd && bus.read16(0x3fd) == 0x5678);
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[4] == 0x3f9 && bus.read32(0x3f9) == 0x12345678);

    // Arithmetic carry, borrow, signed/unsigned compares and byte-string compare.
    cpu.r[1] = 0xffffffffu; cpu.r[2] = 1; cpu.sr = 0;
    bus.put16(0x12a, 0x312e); // ADDC R2,R1
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[1] == 0 && (cpu.sr & Sh2Core::kTBit));
    cpu.r[1] = 1;
    bus.put16(0x12c, 0x312a); // SUBC R2,R1 (1 - 1 - 1 wraps)
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[1] == 0xffffffffu && (cpu.sr & Sh2Core::kTBit));
    cpu.r[1] = 0xffffffffu; cpu.r[2] = 1;
    bus.put16(0x12e, 0x3123); // CMP/GE signed: -1 >= 1 false
    assert(cpu.step(bus) == StepResult::Executed && !(cpu.sr & Sh2Core::kTBit));
    bus.put16(0x130, 0x3122); // CMP/HS unsigned: 0xffffffff >= 1 true
    assert(cpu.step(bus) == StepResult::Executed && (cpu.sr & Sh2Core::kTBit));
    cpu.r[1] = 0x11223344; cpu.r[2] = 0x99223300;
    bus.put16(0x132, 0x212c); // CMP/STR -> matching middle bytes
    assert(cpu.step(bus) == StepResult::Executed && (cpu.sr & Sh2Core::kTBit));

    // PC-relative reads and register-plus-displacement longword accesses.
    bus.write16(0x300, 0x8123);
    bus.write32(0x400, 0x89abcdef);
    bus.put16(0x134, 0x91e4); // MOV.W @(228,PC),R1 -> 0x300
    bus.put16(0x136, 0xd0b2); // MOV.L @(178,PC),R0 -> 0x400
    cpu.r[4] = 0x500; cpu.r[5] = 0x600;
    bus.put16(0x138, 0x1542); // MOV.L R4,@(2,R5)
    bus.put16(0x13a, 0x5643); // MOV.L @(3,R4),R6
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[1] == 0xffff8123u);
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[0] == 0x89abcdefu);
    assert(cpu.step(bus) == StepResult::Executed && bus.read32(0x608) == 0x500);
    bus.write32(0x50c, 0xcafebabe);
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[6] == 0xcafebabeu);

    // Displacement stores and R0 displacement loads.
    cpu.reset(0x13c); cpu.r[1] = 0x700; cpu.r[0] = 0x1234abcd;
    bus.put16(0x13c, 0x8130); // MOV.B R0,@(3,R1) -> 0x703
    bus.put16(0x13e, 0x8141); // MOV.W R0,@(4,R1) -> 0x708
    bus.write8(0x702, 0x80); bus.write16(0x704, 0x8123);
    bus.put16(0x140, 0x8412); // MOV.B @(2,R1),R0
    bus.put16(0x142, 0x8512); // MOV.W @(2,R1),R0 -> address 0x704
    assert(cpu.step(bus) == StepResult::Executed && bus.read8(0x703) == 0xcd);
    assert(cpu.step(bus) == StepResult::Executed && bus.read16(0x708) == 0xabcd);
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[0] == 0xffffff80u);
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[0] == 0xffff8123u);

    // Register bit manipulation and sign/zero extension.
    cpu.reset(0x13c);
    cpu.r[2] = 0x1234abcd; cpu.r[7] = 0x55667788;
    bus.put16(0x13c, 0x6728); // SWAP.B R2,R7
    bus.put16(0x13e, 0x6729); // SWAP.W R2,R7
    bus.put16(0x140, 0x672c); // EXTU.B R2,R7
    bus.put16(0x142, 0x672d); // EXTU.W R2,R7
    bus.put16(0x144, 0x672e); // EXTS.B R2,R7
    bus.put16(0x146, 0x672f); // EXTS.W R2,R7
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[7] == 0x1234cdab);
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[7] == 0xabcd1234);
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[7] == 0xcd);
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[7] == 0xabcd);
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[7] == 0xffffffcd);
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[7] == 0xffffabcd);
    cpu.r[2] = 0x12345678; cpu.r[7] = 0xabcdef01;
    bus.put16(0x148, 0x272d); // XTRCT R2,R7
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[7] == 0x5678abcd);

    // Rotates and multiply families.
    cpu.r[3] = 0x80000001u; cpu.sr = 0;
    bus.put16(0x14a, 0x4328); // ROTL R3
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[3] == 3 && (cpu.sr & Sh2Core::kTBit));
    bus.put16(0x14c, 0x4329); // ROTR R3
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[3] == 0x80000001u && (cpu.sr & Sh2Core::kTBit));
    cpu.r[3] = 0x80000000u; cpu.sr = Sh2Core::kTBit;
    bus.put16(0x14e, 0x4324); // ROTCL R3
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[3] == 1 && (cpu.sr & Sh2Core::kTBit));
    cpu.r[3] = 2; cpu.sr = Sh2Core::kTBit;
    bus.put16(0x150, 0x4325); // ROTCR R3
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[3] == 0x80000001u && !(cpu.sr & Sh2Core::kTBit));

    cpu.r[1] = 0xfffffff0u; cpu.r[2] = 3;
    bus.put16(0x152, 0x0127); // MUL.L R2,R1
    assert(cpu.step(bus) == StepResult::Executed && cpu.macl == 0xffffffd0u);
    cpu.r[1] = 0xffffu; cpu.r[2] = 2;
    bus.put16(0x154, 0x212e); // MULU.W R2,R1
    assert(cpu.step(bus) == StepResult::Executed && cpu.macl == 0x1fffeu);
    cpu.r[1] = 0xffffu; cpu.r[2] = 2;
    bus.put16(0x156, 0x212f); // MULS.W R2,R1
    assert(cpu.step(bus) == StepResult::Executed && cpu.macl == 0xfffffffeu);
    cpu.r[1] = 0xffffffffu; cpu.r[2] = 2;
    bus.put16(0x158, 0x3125); // DMULU.L R2,R1
    assert(cpu.step(bus) == StepResult::Executed && cpu.mach == 1 && cpu.macl == 0xfffffffeu);
    cpu.r[1] = 0xfffffffeu; cpu.r[2] = 3;
    bus.put16(0x15a, 0x312d); // DMULS.L R2,R1
    assert(cpu.step(bus) == StepResult::Executed && cpu.mach == 0xffffffffu && cpu.macl == 0xfffffffau);

    // MOVA computes an aligned PC-relative address into R0.
    cpu.reset(0x15c);
    bus.put16(0x15c, 0xc703); // MOVA @(3,PC),R0 -> aligned (0x160) + 12 = 0x16c
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[0] == 0x16c);

    // Delayed conditional branches and PC-relative branch/call forms.
    cpu.reset(0x160); cpu.sr = Sh2Core::kTBit;
    bus.put16(0x160, 0x8d02); // BT/S +2 -> target 0x168
    bus.put16(0x162, 0xe007); // delay slot must execute
    bus.put16(0x168, 0xe109); // target instruction
    assert(cpu.step(bus) == StepResult::Executed && cpu.pc == 0x162);
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[0] == 7 && cpu.pc == 0x168);
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[1] == 9);

    cpu.reset(0x170); cpu.sr = 0;
    bus.put16(0x170, 0x8f02); // BF/S +2 -> target 0x178
    bus.put16(0x172, 0xe208); // delay slot
    bus.put16(0x178, 0xe30a);
    assert(cpu.step(bus) == StepResult::Executed && cpu.pc == 0x172);
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[2] == 8 && cpu.pc == 0x178);
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[3] == 10);

    cpu.reset(0x180); cpu.r[4] = 8;
    bus.put16(0x180, 0x0423); // BRAF R4 -> PC+4+R4 = 0x18c
    bus.put16(0x182, 0xe505); // delay slot
    bus.put16(0x18c, 0xe606);
    assert(cpu.step(bus) == StepResult::Executed && cpu.pc == 0x182);
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[5] == 5 && cpu.pc == 0x18c);
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[6] == 6);

    cpu.reset(0x190); cpu.r[4] = 8;
    bus.put16(0x190, 0x0403); // BSRF R4; PR = 0x194, target = 0x19c
    bus.put16(0x192, 0xe707);
    bus.put16(0x19c, 0xe808);
    assert(cpu.step(bus) == StepResult::Executed && cpu.pr == 0x194 && cpu.pc == 0x192);
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[7] == 7 && cpu.pc == 0x19c);
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[8] == 8);

    // Reset must cancel a pending delayed branch and clear halted state.
    cpu.reset(0x1a0); cpu.sr = Sh2Core::kTBit;
    bus.put16(0x1a0, 0x8d02); // BT/S schedules a branch to 0x1a8
    assert(cpu.step(bus) == StepResult::Executed && cpu.pc == 0x1a2);
    cpu.halted = true;
    cpu.reset(0x1b0);
    bus.put16(0x1b0, 0xe409); // MOV #9,R4
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[4] == 9 && cpu.pc == 0x1b2);

    // Reset must clear architectural registers and diagnostic fault metadata.
    cpu.r[0] = 0xdeadbeefu; cpu.pr = 0x11112222u; cpu.gbr = 0x33334444u;
    cpu.vbr = 0x55556666u; cpu.mach = 0x77778888u; cpu.macl = 0x9999aaaau;
    cpu.sr = Sh2Core::kTBit; cpu.halted = true;
    cpu.reset(0x1c0);
    assert(cpu.pc == 0x1c0 && cpu.r[0] == 0 && cpu.pr == 0 && cpu.gbr == 0 &&
           cpu.vbr == 0 && cpu.mach == 0 && cpu.macl == 0 && cpu.sr == 0 &&
           !cpu.halted && cpu.fault_pc() == 0 && cpu.last_opcode() == 0);
    bus.put16(0x1c0, 0xe10b); // MOV #11,R1 proves execution resumes after reset.
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[1] == 11 && cpu.pc == 0x1c2);

    bus.write32(0x200, 0x1234abcd);
    assert(bus.read8(0x200) == 0x12 && bus.read16(0x200) == 0x1234);
    cpu.reset(0x1a0);
    bus.put16(0x1a0, 0xffff);
    assert(cpu.step(bus) == StepResult::UnsupportedOpcode);
    assert(cpu.fault_pc() == 0x1a0 && cpu.last_opcode() == 0xffff);
    // JSR @Rn and RTS must both honor their delay slots and preserve PR.
    cpu.reset(0x220); cpu.r[1] = 0x230;
    bus.put16(0x220, 0x410b); // JSR @R1 -> target 0x230, PR = 0x224
    bus.put16(0x222, 0xe209); // call delay slot
    bus.put16(0x230, 0x000b); // RTS -> PR
    bus.put16(0x232, 0xe30a); // return delay slot
    bus.put16(0x224, 0xe40b); // return address instruction
    assert(cpu.step(bus) == StepResult::Executed && cpu.pr == 0x224 && cpu.pc == 0x222);
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[2] == 9 && cpu.pc == 0x230);
    assert(cpu.step(bus) == StepResult::Executed && cpu.pc == 0x232);
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[3] == 10 && cpu.pc == 0x224);
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[4] == 11);

    // JMP @Rn must transfer control only after its delay-slot instruction.
    cpu.reset(0x240); cpu.r[1] = 0x250;
    bus.put16(0x240, 0x412b); // JMP @R1
    bus.put16(0x242, 0xe509); // delay slot: MOV #9,R5
    bus.put16(0x250, 0xe60c); // target: MOV #12,R6
    assert(cpu.step(bus) == StepResult::Executed && cpu.pc == 0x242);
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[5] == 9 && cpu.pc == 0x250);
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[6] == 12);

    std::cout << "SH-2 core foundation tests passed\n";
    return 0;
}
