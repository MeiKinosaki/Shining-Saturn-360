#include "../src/saturn/sh2_core.hpp"
#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
using namespace shining_saturn;
class AddressBus final : public Bus {
public:
    std::array<std::uint8_t,4096> mem{};
    std::uint8_t read8(std::uint32_t a) override { return mem.at(a); }
    void write8(std::uint32_t a,std::uint8_t v) override { mem.at(a)=v; }
};
int main() {
    AddressBus bus; Sh2Core cpu;
    cpu.reset(0x100); cpu.vbr=0x200; cpu.r[15]=0x800;
    bus.write32(0x200+9*4,0x300);
    bus.write16(0x100,0x6211); // MOV.W @R1,R2
    cpu.r[1]=0x401; // odd word address
    assert(cpu.step(bus)==StepResult::Executed);
    assert(cpu.pc==0x300);
    assert(cpu.r[15]==0x7f8);
    assert(bus.read32(0x7f8)==0x100);
    assert(bus.read32(0x7fc)==0);
    assert(!bus.address_error());

    cpu.reset(0x120); cpu.vbr=0x200; cpu.r[15]=0x900;
    bus.write32(0x200+9*4,0x320);
    bus.write16(0x120,0x2122); // MOV.L R2,@R1
    cpu.r[1]=0x402; cpu.r[2]=0x12345678;
    assert(cpu.step(bus)==StepResult::Executed);
    assert(cpu.pc==0x320);
    assert(bus.read32(0x8f8)==0x120);
    assert(bus.read32(0x8fc)==0);
    std::cout<<"SH-2 alignment exception tests passed\n";
}
