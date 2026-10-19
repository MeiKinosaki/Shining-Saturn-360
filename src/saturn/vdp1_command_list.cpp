#include "vdp1_command_list.hpp"

namespace shining_saturn {

Vdp1CommandList::Result Vdp1CommandList::execute(
    Vdp1Renderer& renderer, const std::vector<Command>& commands,
    std::size_t budget) const {
    Result result{};
    const std::size_t limit = commands.size() < budget ? commands.size() : budget;
    for (std::size_t i = 0; i < limit; ++i) {
        const Command& command = commands[i];
        switch (command.opcode) {
        case Opcode::End:
            result.ended = true;
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
            return result;
        }
    }
    result.budget_exhausted = limit < commands.size();
    result.list_exhausted = limit == commands.size() && !result.ended;
    return result;
}

} // namespace shining_saturn
