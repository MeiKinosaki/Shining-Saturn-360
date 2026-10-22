#include "saturn_main_loop.hpp"

namespace shining_saturn {

SaturnMainLoop::SaturnMainLoop(const std::vector<std::uint8_t>& boot_rom)
    : bus_(boot_rom) {}

MainLoopFrame SaturnMainLoop::run_frame(std::size_t instruction_budget,
                                        std::size_t dma_byte_budget) {
    MainLoopFrame frame{};
    // Prototype integration order: CPU writes/reads, bounded DMA transfer,
    // then software VDP2 composition. This is not cycle-accurate arbitration.
    frame.cpu = run_instructions(cpu_, bus_, instruction_budget);
    frame.dma_bytes_moved = dma_.step(bus_, dma_byte_budget);
    vdp2_.compose();
    frame.vdp2_composed = true;
    ++frame_number_;
    frame.frame_number = frame_number_;
    return frame;
}

} // namespace shining_saturn
