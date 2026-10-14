#include "saturn_main_loop.hpp"

namespace shining_saturn {

SaturnMainLoop::SaturnMainLoop(const std::vector<std::uint8_t>& boot_rom)
    : bus_(boot_rom) {}

MainLoopFrame SaturnMainLoop::run_frame(std::size_t instruction_budget) {
    MainLoopFrame frame{};
    frame.cpu = run_instructions(cpu_, bus_, instruction_budget);
    frame_number_ = frame_number_ + 1;
    frame.frame_number = frame_number_;
    return frame;
}

} // namespace shining_saturn
