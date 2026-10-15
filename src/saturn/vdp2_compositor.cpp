#include "vdp2_compositor.hpp"
#include <algorithm>

namespace shining_saturn {

Vdp2Compositor::Vdp2Compositor()
    : output_(Width * Height, 0) {
    for (auto& layer : layers_) layer.pixels.assign(Width * Height, 0);
}

void Vdp2Compositor::clear_backdrop(std::uint16_t rgb555) {
    backdrop_ = static_cast<std::uint16_t>(rgb555 & 0x7FFFu);
}

bool Vdp2Compositor::set_layer(std::size_t index, const Layer& pixels, int priority, bool enabled) {
    if (index >= layers_.size() || pixels.size() != Width * Height) return false;
    layers_[index].pixels = pixels;
    layers_[index].priority = priority;
    layers_[index].enabled = enabled;
    return true;
}

bool Vdp2Compositor::set_scroll(std::size_t index, int x, int y) {
    if (index >= layers_.size()) return false;
    layers_[index].scroll_x = x % static_cast<int>(Width);
    layers_[index].scroll_y = y % static_cast<int>(Height);
    return true;
}

void Vdp2Compositor::disable_layer(std::size_t index) {
    if (index < layers_.size()) layers_[index].enabled = false;
}

void Vdp2Compositor::compose() {
    std::fill(output_.begin(), output_.end(), backdrop_);
    // Draw all five prototype background planes from lower to higher priority.
    // Stable ordering preserves plane-index order for equal priorities.
    std::array<std::size_t, 5> order{{0, 1, 2, 3, 4}};
    std::stable_sort(order.begin(), order.end(), [this](std::size_t a, std::size_t b) {
        return layers_[a].priority < layers_[b].priority;
    });
    for (std::size_t n = 0; n < order.size(); ++n) {
        const auto& layer = layers_[order[n]];
        if (!layer.enabled) continue;
        for (std::size_t y = 0; y < Height; ++y) {
            for (std::size_t x = 0; x < Width; ++x) {
                // Positive scroll moves the image right/down; coordinates wrap at the framebuffer edge.
                const auto wrap = [](int value, int extent) {
                    value %= extent;
                    return value < 0 ? value + extent : value;
                };
                const int source_x = wrap(static_cast<int>(x) - layer.scroll_x, static_cast<int>(Width));
                const int source_y = wrap(static_cast<int>(y) - layer.scroll_y, static_cast<int>(Height));
                const std::size_t source = static_cast<std::size_t>(source_y) * Width + source_x;
                const std::size_t destination = y * Width + x;
                // In this prototype, zero is transparent; Saturn color math/priority rules are not modeled.
                if (layer.pixels[source] != 0)
                    output_[destination] = static_cast<std::uint16_t>(layer.pixels[source] & 0x7FFFu);
            }
        }
    }
}

std::uint16_t Vdp2Compositor::pixel(std::size_t x, std::size_t y) const {
    if (x >= Width || y >= Height) return 0;
    return output_[y * Width + x];
}

} // namespace shining_saturn
