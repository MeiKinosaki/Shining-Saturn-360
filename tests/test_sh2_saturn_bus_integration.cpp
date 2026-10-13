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
    bus.write16(0x00200006u, 0xffffu);
    assert(cpu.step(bus) == StepResult::UnsupportedOpcode);
    assert(cpu.pc == 0x00200006u);
    assert(cpu.fault_pc() == 0x00200006u);
    assert(cpu.last_opcode() == 0xffffu);

    std::cout << "SH-2 execution over Saturn memory adapter passed\n";
}
