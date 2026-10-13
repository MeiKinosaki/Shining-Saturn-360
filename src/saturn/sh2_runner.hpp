#pragma once

#include "sh2_core.hpp"
#include <cstddef>
#include <cstdint>

namespace shining_saturn {

struct RunResult {
    std::size_t instructions_executed = 0;
    StepResult last_result = StepResult::Executed;
    std::uint32_t pc_after_run = 0;
};

// Deterministic bounded execution helper. The budget prevents an emulator
// frontend or a test from entering an unbounded interpreter loop.
RunResult run_instructions(Sh2Core& cpu, Bus& bus, std::size_t budget);

} // namespace shining_saturn
