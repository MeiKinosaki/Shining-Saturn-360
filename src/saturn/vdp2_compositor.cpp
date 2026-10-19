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
    layers_[index].scroll_x = ((x % static_cast<int>(Width)) + static_cast<int>(Width)) % static_cast<int>(Width);
    layers_[index].scroll_y = ((y % static_cast<int>(Height)) + static_cast<int>(Height)) % static_cast<int>(Height);
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
    // Stable insertion sort is allocation-free and bounded for this fixed five-layer set.
    for (std::size_t i = 1; i < order.size(); ++i) {
        const std::size_t key = order[i];
        std::size_t j = i;
        while (j > 0 && layers_[order[j - 1]].priority > layers_[key].priority) {
            order[j] = order[j - 1];
            --j;
        }
        order[j] = key;
    }
    // Scroll is normalized by set_scroll; use bounded wrap branches in the hot pixel loop.
    for (std::size_t n = 0; n < order.size(); ++n) {
        const auto& layer = layers_[order[n]];
        if (!layer.enabled) continue;
        const int scroll_x = layer.scroll_x;
        const int scroll_y = layer.scroll_y;
        for (std::size_t y = 0; y < Height; ++y) {
            const std::size_t source_y = y >= static_cast<std::size_t>(scroll_y)
                ? y - static_cast<std::size_t>(scroll_y)
                : y + Height - static_cast<std::size_t>(scroll_y);
            const std::size_t source_row = source_y * Width;
            const std::size_t destination_row = y * Width;
            for (std::size_t x = 0; x < Width; ++x) {
                const std::size_t source_x = x >= static_cast<std::size_t>(scroll_x)
                    ? x - static_cast<std::size_t>(scroll_x)
                    : x + Width - static_cast<std::size_t>(scroll_x);
                const std::size_t source = source_row + source_x;
                const std::size_t destination = destination_row + x;
                // Prototype transparency; Saturn color math/priority rules remain unmodeled.
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
