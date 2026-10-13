#include "sh2_runner.hpp"

namespace shining_saturn {

RunResult run_instructions(Sh2Core& cpu, Bus& bus, std::size_t budget) {
    RunResult result{};
    result.pc_after_run = cpu.pc;
    result.last_result = cpu.halted ? StepResult::Halted : StepResult::Executed;
    while (result.instructions_executed < budget) {
        const StepResult step = cpu.step(bus);
        result.last_result = step;
        if (step != StepResult::Executed) break;
        ++result.instructions_executed;
        result.pc_after_run = cpu.pc;
    }
    result.pc_after_run = cpu.pc;
    return result;
}

} // namespace shining_saturn
