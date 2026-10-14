#include "vdp1_renderer.hpp"
#include <algorithm>

namespace shining_saturn {

Vdp1Renderer::Vdp1Renderer() : pixels_(static_cast<std::size_t>(Width) * Height, 0) {}

void Vdp1Renderer::clear(std::uint16_t rgb555) {
    std::fill(pixels_.begin(), pixels_.end(), static_cast<std::uint16_t>(rgb555 & 0x7FFFu));
}

void Vdp1Renderer::fill_rect(int x, int y, int width, int height, std::uint16_t rgb555) {
    if (width <= 0 || height <= 0) return;
    const int left = std::max(0, x);
    const int top = std::max(0, y);
    const int right = std::min(static_cast<int>(Width), x + std::min(width, static_cast<int>(Width) * 2));
    const int bottom = std::min(static_cast<int>(Height), y + std::min(height, static_cast<int>(Height) * 2));
    if (left >= right || top >= bottom) return;
    const auto color = static_cast<std::uint16_t>(rgb555 & 0x7FFFu);
    for (int row = top; row < bottom; ++row) {
        const auto start = pixels_.begin() + static_cast<std::ptrdiff_t>(row) * Width + left;
        std::fill(start, start + (right - left), color);
    }
}

std::uint16_t Vdp1Renderer::pixel(std::uint16_t x, std::uint16_t y) const {
    if (x >= Width || y >= Height) return 0;
    return pixels_[static_cast<std::size_t>(y) * Width + x];
}

std::size_t Vdp1Renderer::nonzero_pixels() const {
    return static_cast<std::size_t>(std::count_if(pixels_.begin(), pixels_.end(),
        [](std::uint16_t value) { return value != 0; }));
}

} // namespace shining_saturn
