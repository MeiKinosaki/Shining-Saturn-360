#include "../src/saturn/sh2_runner.hpp"
#include "../src/saturn/saturn_memory_bus.hpp"
#include <cassert>
#include <iostream>

using namespace shining_saturn;

int main() {
    SaturnMemoryBus bus;
    bus.write16(0x00200000u, 0xe103u); // MOV #3,R1
    bus.write16(0x00200002u, 0x7102u); // ADD #2,R1
    bus.write16(0x00200004u, 0x0009u); // NOP
    bus.write16(0x00200006u, 0xffffu); // unsupported
    Sh2Core cpu;
    cpu.reset(0x00200000u);
    const RunResult first = run_instructions(cpu, bus, 2);
    assert(first.instructions_executed == 2);
    assert(first.last_result == StepResult::Executed);
    assert(first.pc_after_run == 0x00200004u);
    assert(cpu.r[1] == 5);

    const RunResult second = run_instructions(cpu, bus, 10);
    assert(second.instructions_executed == 1);
    assert(second.last_result == StepResult::UnsupportedOpcode);
    assert(cpu.pc == 0x00200006u);

    const RunResult zero = run_instructions(cpu, bus, 0);
    assert(zero.instructions_executed == 0);
    assert(zero.pc_after_run == cpu.pc);
    std::cout << "SH-2 bounded runner tests passed\n";
}
