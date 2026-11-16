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
};

int main() {
    TestBus bus;
    Sh2Core cpu;
    cpu.reset(0x100);
    cpu.vbr = 0x1000;
    cpu.r[15] = 0x8000;
    cpu.sr = 0x00000010u; // interrupt mask = 1
    bus.write32(0x1000 + 0x40 * 4, 0x200);
    bus.write16(0x200, 0x002b); // RTE
    bus.write16(0x202, 0xe109); // delay slot: MOV #9,R1
    bus.write16(0x300, 0xe20a); // resumed instruction

    cpu.request_interrupt(4, 0x40);
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.pc == 0x200);
    assert(cpu.r[15] == 0x7ff8);
    assert(bus.read32(0x7ff8) == 0x100); // saved PC
    assert(bus.read32(0x7ffc) == 0x10);  // saved SR
    assert(cpu.sr == 0x40u); // interrupt mask raised to accepted priority
    assert(((cpu.sr >> 4) & 0xfu) == 4u);

    assert(cpu.step(bus) == StepResult::Executed); // RTE schedules delayed return
    assert(cpu.pc == 0x202 && cpu.r[15] == 0x8000);
    assert(cpu.sr != 0x10); // SR restore is deferred until after delay slot
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.pc == 0x100 && cpu.r[1] == 9);
    assert(cpu.sr == 0x10);
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[2] == 10);

    // Synchronous exception entry uses the same stack frame and vector table.
    cpu.reset(0x400);
    cpu.vbr = 0x1000;
    cpu.r[15] = 0x9000;
    cpu.sr = 0x12345678u;
    bus.write32(0x1000 + 6 * 4, 0x500);
    assert(cpu.raise_exception(bus, 6, 0x444));
    assert(cpu.pc == 0x500 && cpu.r[15] == 0x8ff8);
    assert(bus.read32(0x8ff8) == 0x444);
    assert(bus.read32(0x8ffc) == 0x12345678u);
    assert(cpu.sr == 0x12345678u); // synchronous entry preserves SR until handler changes it

    // Masked interrupts remain pending and are accepted after the mask is lowered.
    cpu.reset(0x600);
    cpu.vbr = 0x1000;
    cpu.r[15] = 0xa000;
    cpu.sr = 0x00000050u; // mask = 5
    bus.write32(0x1000 + 0x41 * 4, 0x700);
    cpu.request_interrupt(3, 0x41);
    bus.write16(0x600, 0x0009); // NOP
    assert(cpu.step(bus) == StepResult::Executed && cpu.pc == 0x602);
    cpu.sr = 0; // software lowers IMASK
    assert(cpu.step(bus) == StepResult::Executed && cpu.pc == 0x700);
    assert(bus.read32(0x9ff8) == 0x602);

    std::cout << "SH-2 exception and interrupt tests passed\n";
}
