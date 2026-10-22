#include "../src/saturn/saturn_main_loop.hpp"
#include <cassert>
#include <cstdint>

int main() {
    shining_saturn::SaturnMainLoop loop;
    loop.bus().write16(0x00200000, 0x0009);
    loop.bus().write16(0x00200002, 0x0009);
    loop.cpu().reset(0x00200000);
    const auto first = loop.run_frame(1);
    assert(first.frame_number == 1);
    assert(first.cpu.instructions_executed == 1);
    assert(first.cpu.pc_after_run == 0x00200002);
    assert(first.vdp2_composed);
    assert(first.dma_bytes_moved == 0);

    loop.bus().write32(0x00200100, 0x12345678);
    assert(loop.dma().configure(0, 0x00200100, 0x00200200, 4));
    assert(loop.dma().start(0));
    const auto second = loop.run_frame(1, 4);
    assert(second.frame_number == 2);
    assert(second.cpu.instructions_executed == 1);
    assert(second.cpu.pc_after_run == 0x00200004);
    assert(second.dma_bytes_moved == 4);
    assert(loop.bus().read32(0x00200200) == 0x12345678);
    assert(second.vdp2_composed);
}
