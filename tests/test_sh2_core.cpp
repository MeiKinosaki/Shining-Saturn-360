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
    bus.put16(0x134, 0x91e5); // MOV.W @(229,PC),R1 -> 0x300
    bus.put16(0x136, 0xd0b2); // MOV.L @(178,PC),R0 -> 0x400
    cpu.r[4] = 0x500; cpu.r[5] = 0x600;
    bus.put16(0x138, 0x1542); // MOV.L R4,@(2,R5)
    bus.put16(0x13a, 0x5643); // MOV.L @(3,R4),R6
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[1] == 0xffff8123u);
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[0] == 0x89abcdefu);
    assert(cpu.step(bus) == StepResult::Executed && bus.read32(0x608) == 0x500);
    bus.write32(0x50c, 0xcafebabe);
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[6] == 0xcafebabeu);

    bus.write32(0x200, 0x1234abcd);
    assert(bus.read8(0x200) == 0x12 && bus.read16(0x200) == 0x1234);
    assert(bus.read32(0x200) == 0x1234abcd);
    bus.put16(0x13c, 0xffff);
    assert(cpu.step(bus) == StepResult::UnsupportedOpcode);
    assert(cpu.fault_pc() == 0x13c && cpu.last_opcode() == 0xffff);
    std::cout << "SH-2 core foundation tests passed\n";
    return 0;
}
