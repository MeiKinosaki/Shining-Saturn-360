#pragma once
#include "vdp1_renderer.hpp"
#include <cstdint>
#include <vector>

namespace shining_saturn {

// Minimal command-list adapter for tests and early renderer integration.
// This is not the Saturn's real VDP1 command encoding.
class Vdp1CommandList {
public:
    enum class Opcode : std::uint8_t { End = 0, Clear = 1, FillRect = 2, DrawLine = 3, FillTriangle = 4 };
    struct Command {
        Opcode opcode = Opcode::End;
        std::int16_t x = 0, y = 0, width = 0, height = 0;
        std::uint16_t color = 0;
        std::int16_t x2 = 0, y2 = 0, x3 = 0, y3 = 0;
    };
    struct Result {
        std::size_t executed = 0;
        bool ended = false;
        bool invalid = false;
        bool budget_exhausted = false;
    };

    Result execute(Vdp1Renderer& renderer, const std::vector<Command>& commands,
                   std::size_t budget) const;
};

} // namespace shining_saturn
