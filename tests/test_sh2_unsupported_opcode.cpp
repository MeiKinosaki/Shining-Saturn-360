#include "../src/saturn/sh2_core.hpp"

#include <array>
#include <cstdint>
#include <iostream>

using namespace shining_saturn;

class OpcodeTestBus final : public Bus {
public:
    std::array<std::uint8_t, 512> memory{};
    std::uint8_t read8(std::uint32_t address) override {
        return address < memory.size() ? memory[address] : 0;
    }
    void write8(std::uint32_t address, std::uint8_t value) override {
        if (address < memory.size()) memory[address] = value;
    }
};

static int fail(const char* message) {
    std::cerr << "FAIL: " << message << '\n';
    return 1;
}

int main() {
    OpcodeTestBus bus;
    Sh2Core cpu;
    cpu.reset(0x100);
    cpu.sr = 0x123;
    bus.write16(0x100, 0xffff); // unsupported by the current interpreter subset

    const auto result = cpu.step(bus);
    if (result != StepResult::UnsupportedOpcode)
        return fail("unsupported opcode must be reported");
    if (cpu.pc != 0x100)
        return fail("PC must identify the faulting instruction");
    if (cpu.last_opcode() != 0xffff)
        return fail("last opcode must be retained for diagnostics");
    if (cpu.fault_pc() != 0x100)
        return fail("fault PC must be retained for diagnostics");
    if (cpu.halted)
        return fail("unsupported opcode outside a delay slot must not silently halt");

    const auto saved = cpu.snapshot();
    Sh2Core restored;
    restored.restore(saved);
    if (restored.pc != 0x100 || restored.last_opcode() != 0xffff ||
        restored.fault_pc() != 0x100 || restored.sr != 0x123)
        return fail("snapshot must preserve unsupported-opcode diagnostics and CPU state");

    // Repeating the same initial condition must produce the same diagnostic state.
    Sh2Core repeated;
    repeated.reset(0x100);
    repeated.sr = 0x123;
    if (repeated.step(bus) != StepResult::UnsupportedOpcode)
        return fail("repeat execution must report unsupported opcode");
    const auto repeated_state = repeated.snapshot();
    if (repeated_state.pc != saved.pc ||
        repeated_state.last_opcode != saved.last_opcode ||
        repeated_state.fault_pc != saved.fault_pc ||
        repeated_state.sr != saved.sr)
        return fail("unsupported-opcode result must be deterministic");


    // Unsupported instructions in a branch delay slot stop deterministically.
    // The pending branch target must not be applied after this fault.
    Sh2Core delay_slot_cpu;
    delay_slot_cpu.reset(0x120);
    bus.write16(0x120, 0xa000); // BRA with displacement zero; target 0x124
    bus.write16(0x122, 0xffff); // unsupported opcode in the delay slot
    if (delay_slot_cpu.step(bus) != StepResult::Executed)
        return fail("BRA setup instruction must execute");
    if (delay_slot_cpu.pc != 0x122)
        return fail("BRA must enter its delay slot before applying target");
    if (delay_slot_cpu.step(bus) != StepResult::UnsupportedOpcode)
        return fail("unsupported delay-slot opcode must be reported");
    if (!delay_slot_cpu.halted)
        return fail("unsupported delay-slot opcode must stop deterministically");
    if (delay_slot_cpu.pc != 0x122 || delay_slot_cpu.fault_pc() != 0x122 ||
        delay_slot_cpu.last_opcode() != 0xffff)
        return fail("delay-slot diagnostics must identify the failing instruction");
    if (delay_slot_cpu.pc == 0x124)
        return fail("pending branch target must not be applied after a delay-slot fault");

    std::cout << "SH-2 unsupported-opcode diagnostic tests passed\n";
    return 0;
}
