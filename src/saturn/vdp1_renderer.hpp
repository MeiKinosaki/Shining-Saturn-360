#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>

namespace shining_saturn {

// Experimental software framebuffer/command helper; not yet a complete Saturn VDP1.
class Vdp1Renderer {
public:
    static constexpr std::uint16_t Width = 320;
    static constexpr std::uint16_t Height = 224;

    Vdp1Renderer();
    void clear(std::uint16_t rgb555 = 0);
    void fill_rect(int x, int y, int width, int height, std::uint16_t rgb555);
    std::uint16_t pixel(std::uint16_t x, std::uint16_t y) const;
    const std::vector<std::uint16_t>& pixels() const { return pixels_; }
    std::size_t nonzero_pixels() const;

private:
    std::vector<std::uint16_t> pixels_;
};

} // namespace shining_saturn
