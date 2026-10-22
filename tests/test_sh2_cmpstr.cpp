#include "../src/saturn/sh2_core.hpp"
#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
using namespace shining_saturn;
class CmpstrBus final : public Bus {
public:
 std::array<std::uint8_t,64> bytes{};
 std::uint8_t read8(std::uint32_t a) override { return bytes.at(a); }
 void write8(std::uint32_t a,std::uint8_t v) override { bytes.at(a)=v; }
 void put16(std::uint32_t a,std::uint16_t v) { write16(a,v); }
};
int main() {
 CmpstrBus bus; Sh2Core cpu; cpu.reset(0x10);
 bus.put16(0x10,0x212c); // CMP/STR R2,R1
 cpu.r[1]=0x11223344u; cpu.r[2]=0x55667744u; cpu.sr=0x100u;
 assert(cpu.step(bus)==StepResult::Executed);
 assert((cpu.sr & Sh2Core::kTBit)!=0);
 assert(cpu.r[1]==0x11223344u && cpu.r[2]==0x55667744u);
 bus.put16(0x12,0x212c);
 cpu.r[1]=0x11223344u; cpu.r[2]=0x55667788u; cpu.sr=Sh2Core::kTBit;
 assert(cpu.step(bus)==StepResult::Executed);
 assert((cpu.sr & Sh2Core::kTBit)==0);
 assert(cpu.r[1]==0x11223344u && cpu.r[2]==0x55667788u);
 std::cout << "SH-2 CMP/STR tests passed\\n";
}
