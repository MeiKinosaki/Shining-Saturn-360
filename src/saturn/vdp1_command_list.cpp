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
        default:
            result.invalid = true;
            return result;
        }
    }
    return result;
}

} // namespace shining_saturn
