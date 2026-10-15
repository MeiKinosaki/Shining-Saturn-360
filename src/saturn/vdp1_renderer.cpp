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
    // Clip in floating-point before integer rasterization. This avoids overflow
    // in differences/products for extreme caller-supplied coordinates.
    double ax = x0, ay = y0, bx = x1, by = y1;
    const double dx = bx - ax, dy = by - ay;
    double t0 = 0.0, t1 = 1.0;
    const auto clip = [&t0, &t1](double p, double q) {
        if (p == 0.0) return q >= 0.0;
        const double r = q / p;
        if (p < 0.0) { if (r > t1) return false; if (r > t0) t0 = r; }
        else { if (r < t0) return false; if (r < t1) t1 = r; }
        return true;
    };
    if (!clip(-dx, ax) || !clip(dx, (Width - 1) - ax) ||
        !clip(-dy, ay) || !clip(dy, (Height - 1) - ay)) return;
    const int sx0 = static_cast<int>(ax + t0 * dx + 0.5);
    const int sy0 = static_cast<int>(ay + t0 * dy + 0.5);
    const int sx1 = static_cast<int>(ax + t1 * dx + 0.5);
    const int sy1 = static_cast<int>(ay + t1 * dy + 0.5);
    int x = sx0, y = sy0;
    const int delta_x = std::abs(sx1 - sx0), step_x = sx0 < sx1 ? 1 : -1;
    const int delta_y = -std::abs(sy1 - sy0), step_y = sy0 < sy1 ? 1 : -1;
    int error = delta_x + delta_y;
    const auto color = static_cast<std::uint16_t>(rgb555 & 0x7FFFu);
    while (true) {
        pixels_[static_cast<std::size_t>(y) * Width + x] = color;
        if (x == sx1 && y == sy1) break;
        const int twice = 2 * error;
        if (twice >= delta_y) { error += delta_y; x += step_x; }
        if (twice <= delta_x) { error += delta_x; y += step_y; }
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
        return (static_cast<std::int64_t>(px) - ax) * (static_cast<std::int64_t>(by) - ay) -
               (static_cast<std::int64_t>(py) - ay) * (static_cast<std::int64_t>(bx) - ax);
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
