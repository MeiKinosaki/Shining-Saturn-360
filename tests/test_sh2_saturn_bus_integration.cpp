#include "../src/saturn/saturn_memory_bus.hpp"
#include "../src/saturn/sh2_core.hpp"
#include <cassert>
#include <iostream>

using namespace shining_saturn;

int main() {
    // Tiny synthetic SH-2 program placed in writable RAM. This verifies that
    // the CPU can fetch and execute through the Saturn memory adapter.
    SaturnMemoryBus bus;
    bus.write16(0x00200000u, 0xe105u); // MOV #5,R1
    bus.write16(0x00200002u, 0x7203u); // ADD #3,R2
    bus.write16(0x00200004u, 0x312cu); // ADD R2,R1
    Sh2Core cpu;
    cpu.reset(0x00200000u);
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.r[1] == 5);
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.r[2] == 3);
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.r[1] == 8);

    // Unsupported opcodes are reported and PC is restored to the faulting
    // instruction so a debugger or diagnostic screen can inspect it.
    cpu.vbr = 0x00201000u; cpu.r[15] = 0x0020f000u;
    bus.write32(0x00201010u, 0x00200280u);
    bus.write16(0x00200006u, 0xffffu);
    assert(cpu.step(bus) == StepResult::UnsupportedOpcode);
    assert(cpu.pc == 0x00200280u);
    assert(cpu.fault_pc() == 0x00200006u);
    assert(cpu.last_opcode() == 0xffffu);

    // Exception vectors, stack frame and RTE traverse SaturnMemoryBus.
    cpu.reset(0x00200010u);
    cpu.vbr = 0x00201000u;
    cpu.r[15] = 0x0020f000u;
    cpu.sr = 0x10u;
    bus.write32(0x00201100u, 0x00200200u); // vector 0x40
    bus.write16(0x00200200u, 0x002bu); // RTE
    bus.write16(0x00200202u, 0xe309u); // delay slot
    bus.write16(0x00200010u, 0xe40bu); // resumed instruction
    cpu.request_interrupt(4, 0x40);
    assert(cpu.step(bus) == StepResult::Executed && cpu.pc == 0x00200200u);
    assert(bus.read32(0x0020eff8u) == 0x00200010u);
    assert(bus.read32(0x0020effcu) == 0x10u);
    assert(cpu.step(bus) == StepResult::Executed && cpu.pc == 0x00200202u);
    assert(cpu.step(bus) == StepResult::Executed && cpu.pc == 0x00200010u);
    assert(cpu.r[3] == 9 && cpu.sr == 0x10u);
    assert(cpu.step(bus) == StepResult::Executed && cpu.r[4] == 11);

    std::cout << "SH-2 execution over Saturn memory adapter passed\n";
}
