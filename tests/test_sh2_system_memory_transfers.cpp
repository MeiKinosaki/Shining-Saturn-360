#include "../src/saturn/sh2_core.hpp"
#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
using namespace shining_saturn;
class TestBus final : public Bus {
public:
 std::array<std::uint8_t,512> bytes{};
 std::uint8_t read8(std::uint32_t a) override { return bytes.at(a); }
 void write8(std::uint32_t a,std::uint8_t v) override { bytes.at(a)=v; }
 void put16(std::uint32_t a,std::uint16_t v) { write16(a,v); }
};
int main() {
 TestBus bus; Sh2Core cpu; cpu.reset(0x20);
 cpu.sr=0x12345678u; cpu.gbr=0x23456789u; cpu.vbr=0x3456789au;
 cpu.r[1]=0x100; bus.put16(0x20,0x4113); bus.put16(0x22,0x4123); bus.put16(0x24,0x4103);
 assert(cpu.step(bus)==StepResult::Executed && cpu.r[1]==0xfc && bus.read32(0xfc)==0x23456789u);
 assert(cpu.step(bus)==StepResult::Executed && cpu.r[1]==0xf8 && bus.read32(0xf8)==0x3456789au);
 assert(cpu.step(bus)==StepResult::Executed && cpu.r[1]==0xf4 && bus.read32(0xf4)==0x12345678u);
 bus.write32(0x100,0x87654321u); bus.write32(0x104,0x10203040u); bus.write32(0x108,0x50607080u);
 cpu.r[2]=0x100; bus.put16(0x26,0x4217); bus.put16(0x28,0x4227); bus.put16(0x2a,0x4207);
 assert(cpu.step(bus)==StepResult::Executed && cpu.gbr==0x87654321u && cpu.r[2]==0x104);
 assert(cpu.step(bus)==StepResult::Executed && cpu.vbr==0x10203040u && cpu.r[2]==0x108);
 assert(cpu.step(bus)==StepResult::Executed && cpu.sr==0x50607080u && cpu.r[2]==0x10c);
 cpu.mach=0x11112222u; cpu.macl=0x33334444u; cpu.pr=0x55556666u;
 cpu.r[3]=0x140; bus.put16(0x2c,0x4322); bus.put16(0x2e,0x4312); bus.put16(0x30,0x4302);
 assert(cpu.step(bus)==StepResult::Executed && cpu.r[3]==0x13c && bus.read32(0x13c)==0x55556666u);
 assert(cpu.step(bus)==StepResult::Executed && cpu.r[3]==0x138 && bus.read32(0x138)==0x33334444u);
 assert(cpu.step(bus)==StepResult::Executed && cpu.r[3]==0x134 && bus.read32(0x134)==0x11112222u);
 bus.write32(0x140,0xabcdef01u); bus.write32(0x144,0xabcdef02u); bus.write32(0x148,0xabcdef03u);
 cpu.r[4]=0x140; bus.put16(0x32,0x4426); bus.put16(0x34,0x4416); bus.put16(0x36,0x4406);
 assert(cpu.step(bus)==StepResult::Executed && cpu.pr==0xabcdef01u && cpu.r[4]==0x144);
 assert(cpu.step(bus)==StepResult::Executed && cpu.macl==0xabcdef02u && cpu.r[4]==0x148);
 assert(cpu.step(bus)==StepResult::Executed && cpu.mach==0xabcdef03u && cpu.r[4]==0x14c);
 std::cout << "SH-2 system register memory transfer tests passed\n";
}
