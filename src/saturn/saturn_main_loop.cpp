#include "saturn_main_loop.hpp"

namespace shining_saturn {

SaturnMainLoop::SaturnMainLoop(const std::vector<std::uint8_t>& boot_rom)
    : bus_(boot_rom) {}

Vdp1CommandList::Result SaturnMainLoop::execute_vdp1_commands(
    const std::vector<Vdp1CommandList::Command>& commands, std::size_t budget) {
    const auto result = vdp1_command_list_.execute(vdp1_, commands, budget);
    pending_vdp1_commands_executed_ += result.executed;
    pending_vdp1_command_list_ended_ = pending_vdp1_command_list_ended_ || result.ended;
    return result;
}

MainLoopFrame SaturnMainLoop::run_frame(std::size_t instruction_budget,
                                        std::size_t dma_byte_budget) {
    MainLoopFrame frame{};
    // Prototype integration order: CPU writes/reads, bounded DMA transfer,
    // then software VDP2 composition. This is not cycle-accurate arbitration.
    frame.cpu = run_instructions(cpu_, bus_, instruction_budget);
    frame.dma_bytes_moved = dma_.step(bus_, dma_byte_budget);
    frame.vdp1_nonzero_pixels = vdp1_.nonzero_pixels();
    frame.vdp1_commands_executed = pending_vdp1_commands_executed_;
    frame.vdp1_command_list_ended = pending_vdp1_command_list_ended_;
    pending_vdp1_commands_executed_ = 0;
    pending_vdp1_command_list_ended_ = false;
    vdp2_.compose();
    frame.vdp2_composed = true;
    ++frame_number_;
    frame.frame_number = frame_number_;
    return frame;
}

} // namespace shining_saturn
