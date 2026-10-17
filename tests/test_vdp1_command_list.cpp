#include "../src/saturn/vdp1_command_list.hpp"
#include <cassert>
#include <vector>

int main() {
    using namespace shining_saturn;
    Vdp1Renderer renderer;
    Vdp1CommandList executor;
    const std::vector<Vdp1CommandList::Command> commands = {
        {Vdp1CommandList::Opcode::Clear, 0, 0, 0, 0, 0x001F},
        {Vdp1CommandList::Opcode::FillRect, 4, 5, 3, 2, 0x7FFF},
        {Vdp1CommandList::Opcode::End, 0, 0, 0, 0, 0}
    };
    auto result = executor.execute(renderer, commands, 8);
    assert(result.ended && !result.invalid && result.executed == 2);
    assert(renderer.pixel(0, 0) == 0x001F);
    assert(renderer.pixel(4, 5) == 0x7FFF);
    assert(renderer.pixel(6, 6) == 0x7FFF);
    assert(renderer.pixel(7, 6) == 0x001F);

    Vdp1Renderer bounded;
    result = executor.execute(bounded, commands, 1);
    assert(!result.ended && result.executed == 1);
    assert(bounded.pixel(0, 0) == 0x001F);
    assert(bounded.pixel(4, 5) == 0x001F); // untouched pixels retain the clear color

    // A zero execution budget leaves commands untouched and reports no progress.
    Vdp1Renderer zero_budget_renderer;
    const std::vector<Vdp1CommandList::Command> single = {
        {Vdp1CommandList::Opcode::FillRect, 1, 1, 2, 2, 0x03E0},
        {Vdp1CommandList::Opcode::End, 0, 0, 0, 0, 0}
    };
    result = executor.execute(zero_budget_renderer, single, 0);
    assert(!result.ended && !result.invalid && result.executed == 0);
    assert(zero_budget_renderer.pixel(1, 1) == 0);

    // Commands after END must not execute.
    Vdp1Renderer end_renderer;
    const std::vector<Vdp1CommandList::Command> after_end = {
        {Vdp1CommandList::Opcode::End, 0, 0, 0, 0, 0},
        {Vdp1CommandList::Opcode::FillRect, 2, 2, 2, 2, 0x7FFF}
    };
    result = executor.execute(end_renderer, after_end, 8);
    assert(result.ended && !result.invalid && result.executed == 0);
    assert(end_renderer.pixel(2, 2) == 0);

    const std::vector<Vdp1CommandList::Command> invalid = {
        {static_cast<Vdp1CommandList::Opcode>(255), 0, 0, 1, 1, 0x7FFF}
    };
    result = executor.execute(bounded, invalid, 4);
    assert(result.invalid && result.executed == 0);
}
