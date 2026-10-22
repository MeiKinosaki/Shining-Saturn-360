#include "vdp1_command_list.hpp"

namespace shining_saturn {

Vdp1CommandList::Result Vdp1CommandList::execute(
    Vdp1Renderer& renderer, const std::vector<Command>& commands,
    std::size_t budget) const {
    return execute_slice(renderer, commands, 0, budget);
}

Vdp1CommandList::Result Vdp1CommandList::execute_slice(
    Vdp1Renderer& renderer, const std::vector<Command>& commands,
    std::size_t start_index, std::size_t budget) const {
    Result result{};
    if (start_index > commands.size()) {
        result.invalid = true;
        result.next_index = commands.size();
        return result;
    }
    const std::size_t remaining = commands.size() - start_index;
    const std::size_t count = remaining < budget ? remaining : budget;
    const std::size_t limit = start_index + count;
    result.next_index = start_index;
    for (std::size_t i = start_index; i < limit; ++i) {
        const Command& command = commands[i];
        switch (command.opcode) {
        case Opcode::End:
            result.ended = true;
            result.next_index = i + 1;
            return result;
        case Opcode::Clear:
            renderer.clear(command.color);
            ++result.executed;
            break;
        case Opcode::FillRect:
            renderer.fill_rect(command.x, command.y, command.width, command.height, command.color);
            ++result.executed;
            break;
        case Opcode::DrawLine:
            renderer.draw_line(command.x, command.y, command.x2, command.y2, command.color);
            ++result.executed;
            break;
        case Opcode::FillTriangle:
            renderer.fill_triangle(command.x, command.y, command.x2, command.y2,
                                   command.x3, command.y3, command.color);
            ++result.executed;
            break;
        default:
            result.invalid = true;
            result.next_index = i;
            return result;
        }
    }
    result.next_index = limit;
    result.budget_exhausted = limit < commands.size();
    result.list_exhausted = limit == commands.size() && !result.ended;
    return result;
}

} // namespace shining_saturn
