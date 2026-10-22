#include "../src/saturn/sh2_state_codec.hpp"
#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>
using namespace shining_saturn;
int main() {
    Sh2Snapshot a;
    for (std::size_t i=0;i<a.r.size();++i) a.r[i]=0x10203040u+static_cast<std::uint32_t>(i);
    a.pc=0x06001000; a.pr=0x06002000; a.gbr=0x06003000; a.vbr=0x06004000;
    a.mach=0x11223344; a.macl=0x55667788; a.sr=0xF0; a.halted=false;
    a.branch_pending=true; a.branch_target=0x06005000; a.last_opcode=0xA001; a.fault_pc=0x06006000;
    auto bytes=Sh2StateCodec::encode(a);
    Sh2Snapshot b;
    assert(Sh2StateCodec::decode(bytes,b));
    assert(b.r==a.r && b.pc==a.pc && b.pr==a.pr && b.gbr==a.gbr && b.vbr==a.vbr);
    assert(b.mach==a.mach && b.macl==a.macl && b.sr==a.sr);
    assert(b.branch_pending==a.branch_pending && b.branch_target==a.branch_target);
    assert(b.last_opcode==a.last_opcode && b.fault_pc==a.fault_pc && b.halted==a.halted);
    auto corrupt=bytes; corrupt[12]^=0x40;
    assert(!Sh2StateCodec::decode(corrupt,b));
    auto truncated=bytes; truncated.pop_back();
    assert(!Sh2StateCodec::decode(truncated,b));
    auto badVersion=bytes; badVersion[4]=2;
    assert(!Sh2StateCodec::decode(badVersion,b));
    std::cout << "SH-2 state codec tests passed\n";
}
