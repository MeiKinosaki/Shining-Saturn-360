#include "../src/saturn/saturn_main_loop.hpp"
#include <cassert>
#include <cstdint>

int main() {
    shining_saturn::SaturnMainLoop loop;
    const std::vector<shining_saturn::Vdp1CommandList::Command> vdp1_commands = {
        {shining_saturn::Vdp1CommandList::Opcode::Clear, 0, 0, 0, 0, 0},
        {shining_saturn::Vdp1CommandList::Opcode::FillRect, 10, 10, 2, 2, 0x1234},
        {shining_saturn::Vdp1CommandList::Opcode::End, 0, 0, 0, 0, 0}
    };
    const auto vdp1_result = loop.execute_vdp1_commands(vdp1_commands, 8);
    assert(vdp1_result.ended && vdp1_result.executed == 2);
    assert(loop.vdp1().pixel(10, 10) == 0x1234);
    loop.bus().write16(0x00200000, 0x0009);
    loop.bus().write16(0x00200002, 0x0009);
    loop.cpu().reset(0x00200000);
    const auto first = loop.run_frame(1);
    assert(first.frame_number == 1);
    assert(first.cpu.instructions_executed == 1);
    assert(first.cpu.pc_after_run == 0x00200002);
    assert(first.vdp2_composed);
    assert(first.vdp1_nonzero_pixels == 4);
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
    assert(second.vdp1_nonzero_pixels == 4);
}
