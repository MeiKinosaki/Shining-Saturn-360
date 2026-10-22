#include "../src/saturn/sh2_core.hpp"
#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
using namespace shining_saturn;
class ClrmacBus final : public Bus {
public:
 std::array<std::uint8_t,64> bytes{};
 std::uint8_t read8(std::uint32_t a) override { return bytes.at(a); }
 void write8(std::uint32_t a,std::uint8_t v) override { bytes.at(a)=v; }
 void put16(std::uint32_t a,std::uint16_t v) { write16(a,v); }
};
int main() {
 ClrmacBus bus; Sh2Core cpu; cpu.reset(0x10);
 cpu.mach=0x12345678u; cpu.macl=0x9abcdef0u; cpu.sr=0x00000301u;
 bus.put16(0x10,0x0028); // CLRMAC
 assert(cpu.step(bus)==StepResult::Executed);
 assert(cpu.mach==0u && cpu.macl==0u);
 assert(cpu.sr==0x00000301u); // CLRMAC must not alter SR
 assert(cpu.pc==0x12u);
 std::cout << "SH-2 CLRMAC tests passed\n";
}
