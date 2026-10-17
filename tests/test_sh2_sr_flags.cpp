#include "../src/saturn/sh2_core.hpp"
#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>

using namespace shining_saturn;

class TestBus final : public Bus {
public:
    std::array<std::uint8_t, 64> bytes{};
    std::uint8_t read8(std::uint32_t address) override { return bytes.at(address); }
    void write8(std::uint32_t address, std::uint8_t value) override { bytes.at(address) = value; }
    void put16(std::uint32_t address, std::uint16_t opcode) { write16(address, opcode); }
};

int main() {
    TestBus bus;
    Sh2Core cpu;
    cpu.reset(0);

    // CLRS clears only SR.S and preserves T and unrelated status bits.
    cpu.sr = 0x00000305u;
    bus.put16(0, 0x0048);
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.sr == 0x00000305u); // bit 1 was already clear

    cpu.reset(2);
    cpu.sr = 0x00000307u;
    bus.put16(2, 0x0048);
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.sr == 0x00000305u);

    // SETS sets only SR.S and leaves the other SR bits unchanged.
    cpu.reset(4);
    cpu.sr = 0x00000105u;
    bus.put16(4, 0x0058);
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.sr == 0x00000107u);

    // Idempotence: repeating SETS/CLRS must not alter other state.
    cpu.reset(6);
    cpu.sr = 0x00000207u;
    bus.put16(6, 0x0058);
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.sr == 0x00000207u);
    bus.put16(8, 0x0048);
    assert(cpu.step(bus) == StepResult::Executed);
    assert(cpu.sr == 0x00000205u);

    std::cout << "SH-2 SR S-bit tests passed\n";
    return 0;
}
