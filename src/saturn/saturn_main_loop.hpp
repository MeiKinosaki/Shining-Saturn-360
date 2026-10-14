#pragma once
#include "sh2_runner.hpp"
#include "saturn_memory_bus.hpp"
#include <cstddef>
#include <cstdint>

namespace shining_saturn {

// Experimental scheduler slice: runs a bounded SH-2 budget against the Saturn bus.
// Timing conversion and device arbitration are intentionally not modeled yet.
struct MainLoopFrame {
    RunResult cpu;
    std::uint64_t frame_number = 0;
};

class SaturnMainLoop {
public:
    explicit SaturnMainLoop(const std::vector<std::uint8_t>& boot_rom = {});
    Sh2Core& cpu() { return cpu_; }
    SaturnMemoryBus& bus() { return bus_; }
    MainLoopFrame run_frame(std::size_t instruction_budget);
    std::uint64_t frame_number() const { return frame_number_; }
private:
    Sh2Core cpu_;
    SaturnMemoryBus bus_;
    std::uint64_t frame_number_ = 0;
};

} // namespace shining_saturn
