#include "vdp1_renderer.hpp"
#include <algorithm>
#include <cstdlib>
#include <cstdint>

namespace shining_saturn {

Vdp1Renderer::Vdp1Renderer() : pixels_(static_cast<std::size_t>(Width) * Height, 0) {}

void Vdp1Renderer::clear(std::uint16_t rgb555) {
    std::fill(pixels_.begin(), pixels_.end(), static_cast<std::uint16_t>(rgb555 & 0x7FFFu));
}

void Vdp1Renderer::fill_rect(int x, int y, int width, int height, std::uint16_t rgb555) {
    if (width <= 0 || height <= 0) return;
    // Promote before addition so hostile/extreme coordinates cannot overflow int.
    const auto clamp_x = [](long long value) {
        return static_cast<int>(std::max(0LL, std::min(static_cast<long long>(Width), value)));
    };
    const auto clamp_y = [](long long value) {
        return static_cast<int>(std::max(0LL, std::min(static_cast<long long>(Height), value)));
    };
    const int left = clamp_x(x);
    const int top = clamp_y(y);
    const int right = clamp_x(static_cast<long long>(x) + width);
    const int bottom = clamp_y(static_cast<long long>(y) + height);
    if (left >= right || top >= bottom) return;
    const auto color = static_cast<std::uint16_t>(rgb555 & 0x7FFFu);
    for (int row = top; row < bottom; ++row) {
        const auto start = pixels_.begin() + static_cast<std::ptrdiff_t>(row) * Width + left;
        std::fill(start, start + (right - left), color);
    }
}

void Vdp1Renderer::draw_line(int x0, int y0, int x1, int y1, std::uint16_t rgb555) {
    // Cohen-Sutherland-style clipping keeps Bresenham iterations bounded.
    const auto outcode = [](int x, int y) {
        unsigned code = 0;
        if (x < 0) code |= 1u; else if (x >= Width) code |= 2u;
        if (y < 0) code |= 4u; else if (y >= Height) code |= 8u;
        return code;
    };
    unsigned c0 = outcode(x0, y0), c1 = outcode(x1, y1);
    while (true) {
        if (!(c0 | c1)) break;
        if (c0 & c1) return;
        const unsigned out = c0 ? c0 : c1;
        long long x = 0, y = 0;
        if (out & 8u) {
            if (y1 == y0) return;
            y = Height - 1; x = static_cast<long long>(x0) + (static_cast<long long>(x1) - x0) * (y - y0) /
                (static_cast<long long>(y1) - y0);
        } else if (out & 4u) {
            if (y1 == y0) return;
            y = 0; x = x0 + static_cast<long long>(x1 - x0) * (y - y0) / (y1 - y0);
        } else if (out & 2u) {
            if (x1 == x0) return;
            x = Width - 1; y = static_cast<long long>(y0) + (static_cast<long long>(y1) - y0) * (x - x0) /
                (static_cast<long long>(x1) - x0);
        } else {
            if (x1 == x0) return;
            x = 0; y = y0 + static_cast<long long>(y1 - y0) * (x - x0) / (x1 - x0);
        }
        if (out == c0) { x0 = static_cast<int>(x); y0 = static_cast<int>(y); c0 = outcode(x0, y0); }
        else { x1 = static_cast<int>(x); y1 = static_cast<int>(y); c1 = outcode(x1, y1); }
    }
    int dx = std::abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -std::abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int error = dx + dy;
    const auto color = static_cast<std::uint16_t>(rgb555 & 0x7FFFu);
    while (true) {
        pixels_[static_cast<std::size_t>(y0) * Width + x0] = color;
        if (x0 == x1 && y0 == y1) break;
        const int twice = 2 * error;
        if (twice >= dy) { error += dy; x0 += sx; }
        if (twice <= dx) { error += dx; y0 += sy; }
    }
}

void Vdp1Renderer::fill_triangle(int x0, int y0, int x1, int y1, int x2, int y2, std::uint16_t rgb555) {
    // Bound off-screen vertices before edge arithmetic to avoid overflow on extreme input.
    const auto clamp_x = [](int x) { return std::max(-static_cast<int>(Width) * 2,
                                                     std::min(static_cast<int>(Width) * 2, x)); };
    const auto clamp_y = [](int y) { return std::max(-static_cast<int>(Height) * 2,
                                                     std::min(static_cast<int>(Height) * 2, y)); };
    x0 = clamp_x(x0); x1 = clamp_x(x1); x2 = clamp_x(x2);
    y0 = clamp_y(y0); y1 = clamp_y(y1); y2 = clamp_y(y2);
    const int min_x = std::max(0, std::min({x0, x1, x2}));
    const int max_x = std::min(static_cast<int>(Width) - 1, std::max({x0, x1, x2}));
    const int min_y = std::max(0, std::min({y0, y1, y2}));
    const int max_y = std::min(static_cast<int>(Height) - 1, std::max({y0, y1, y2}));
    if (min_x > max_x || min_y > max_y) return;
    const auto edge = [](int ax, int ay, int bx, int by, int px, int py) -> std::int64_t {
        return static_cast<std::int64_t>(px - ax) * (by - ay) -
               static_cast<std::int64_t>(py - ay) * (bx - ax);
    };
    const std::int64_t area = edge(x0, y0, x1, y1, x2, y2);
    if (area == 0) return;
    const auto color = static_cast<std::uint16_t>(rgb555 & 0x7FFFu);
    for (int y = min_y; y <= max_y; ++y) {
        for (int x = min_x; x <= max_x; ++x) {
            const auto e0 = edge(x0, y0, x1, y1, x, y);
            const auto e1 = edge(x1, y1, x2, y2, x, y);
            const auto e2 = edge(x2, y2, x0, y0, x, y);
            if ((area > 0 && e0 >= 0 && e1 >= 0 && e2 >= 0) ||
                (area < 0 && e0 <= 0 && e1 <= 0 && e2 <= 0))
                pixels_[static_cast<std::size_t>(y) * Width + x] = color;
        }
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
