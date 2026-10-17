#include "../src/saturn/sh2_core.hpp"
#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
using namespace shining_saturn;
class TestBus final : public Bus {
public:
 std::array<std::uint8_t,64> bytes{};
 std::uint8_t read8(std::uint32_t a) override { return bytes.at(a); }
 void write8(std::uint32_t a,std::uint8_t v) override { bytes.at(a)=v; }
 void put16(std::uint32_t a,std::uint16_t v) { write16(a,v); }
};
int main() {
 TestBus bus; Sh2Core cpu; cpu.reset(0);
 cpu.sr=0x12345678u; cpu.gbr=0x23456789u; cpu.vbr=0x3456789au;
 bus.put16(0,0x0002); // STC SR,R0
 bus.put16(2,0x0112); // STC GBR,R1
 bus.put16(4,0x0222); // STC VBR,R2
 assert(cpu.step(bus)==StepResult::Executed && cpu.r[0]==0x12345678u);
 assert(cpu.step(bus)==StepResult::Executed && cpu.r[1]==0x23456789u);
 assert(cpu.step(bus)==StepResult::Executed && cpu.r[2]==0x3456789au);
 cpu.r[3]=0x87654321u; bus.put16(6,0x430e); // LDC R3,SR
 assert(cpu.step(bus)==StepResult::Executed && cpu.sr==0x87654321u);
 cpu.r[4]=0x10203040u; bus.put16(8,0x441e); // LDC R4,GBR
 assert(cpu.step(bus)==StepResult::Executed && cpu.gbr==0x10203040u);
 cpu.r[5]=0x50607080u; bus.put16(10,0x452e); // LDC R5,VBR
 assert(cpu.step(bus)==StepResult::Executed && cpu.vbr==0x50607080u);
 std::cout << "SH-2 system register transfer tests passed\n";
}
