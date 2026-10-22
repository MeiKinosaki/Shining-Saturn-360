#include "../src/saturn/sh2_core.hpp"
#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>

using namespace shining_saturn;

class TestBus final : public Bus {
public:
    std::array<std::uint8_t, 1024> bytes{};
    std::uint8_t read8(std::uint32_t address) override { return bytes.at(address); }
    void write8(std::uint32_t address, std::uint8_t value) override { bytes.at(address) = value; }
};

int main() {
    TestBus bus;
    Sh2Core cpu;
    cpu.reset(0x100);
    bus.write16(0x100, 0xA001); // BRA with one delay-slot instruction
    bus.write16(0x102, 0xE102); // MOV #2,R1
    bus.write16(0x106, 0x0009); // NOP at branch target

    assert(cpu.step(bus) == StepResult::Executed);
    const Sh2Snapshot saved = cpu.snapshot();
    assert(saved.pc == 0x102);
    assert(saved.branch_pending);
    assert(saved.branch_target == 0x106);

    cpu.r[1] = 99;
    cpu.pc = 0x200;
    cpu.restore(saved);
    assert(cpu.r[1] == 0);
    assert(cpu.pc == 0x102);
    assert(cpu.snapshot().branch_pending);

    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.r[1] == 2);
    assert(cpu.pc == 0x106);

    Sh2Snapshot halted = cpu.snapshot();
    halted.halted = true;
    halted.r[3] = 0x12345678;
    cpu.restore(halted);
    assert(cpu.halted);
    assert(cpu.r[3] == 0x12345678);
    assert(cpu.step(bus) == StepResult::Halted);

    std::cout << "SH-2 snapshot/restore tests passed\n";
    return 0;
}
