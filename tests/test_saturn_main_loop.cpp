#include "../src/saturn/saturn_main_loop.hpp"
#include <cassert>
#include <cstdint>

int main() {
    shining_saturn::SaturnMainLoop loop;
    // SH-2 big-endian NOP (0x0009) in mapped low work RAM.
    loop.bus().write16(0x00200000, 0x0009);
    loop.bus().write16(0x00200002, 0x0009);
    loop.cpu().reset(0x00200000);
    const auto first = loop.run_frame(1);
    assert(first.frame_number == 1);
    assert(first.cpu.instructions_executed == 1);
    assert(first.cpu.pc_after_run == 0x00200002);
    const auto second = loop.run_frame(1);
    assert(second.frame_number == 2);
    assert(second.cpu.instructions_executed == 1);
    assert(second.cpu.pc_after_run == 0x00200004);
}
