#pragma once
#include "sh2_runner.hpp"
#include "saturn_memory_bus.hpp"
#include "scu_dma.hpp"
#include "vdp2_compositor.hpp"
#include "vdp1_renderer.hpp"
#include "vdp1_command_list.hpp"
#include <cstddef>
#include <cstdint>
#include <vector>

namespace shining_saturn {

// Experimental scheduler: SH-2, byte-copy DMA and software VDP2 composition
// advance in deterministic order. Hardware timing and IRQ arbitration remain TODO.
struct MainLoopFrame {
    RunResult cpu;
    std::size_t dma_bytes_moved = 0;
    bool vdp2_composed = false;
    std::size_t vdp1_nonzero_pixels = 0;
    std::size_t vdp1_commands_executed = 0;
    bool vdp1_command_list_ended = false;
    std::uint64_t frame_number = 0;
};

class SaturnMainLoop {
public:
    explicit SaturnMainLoop(const std::vector<std::uint8_t>& boot_rom = {});
    Sh2Core& cpu() { return cpu_; }
    SaturnMemoryBus& bus() { return bus_; }
    ScuDma& dma() { return dma_; }
    Vdp2Compositor& vdp2() { return vdp2_; }
    Vdp1Renderer& vdp1() { return vdp1_; }
    Vdp1CommandList::Result execute_vdp1_commands(
        const std::vector<Vdp1CommandList::Command>& commands, std::size_t budget);
    MainLoopFrame run_frame(std::size_t instruction_budget, std::size_t dma_byte_budget = 256);
    std::uint64_t frame_number() const { return frame_number_; }
private:
    Sh2Core cpu_;
    SaturnMemoryBus bus_;
    ScuDma dma_;
    Vdp2Compositor vdp2_;
    Vdp1Renderer vdp1_;
    Vdp1CommandList vdp1_command_list_;
    std::uint64_t frame_number_ = 0;
};

} // namespace shining_saturn
