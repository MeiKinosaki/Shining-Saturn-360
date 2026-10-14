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
    if (index >= 2 || pixels.size() != Width * Height) return false;
    layers_[index].pixels = pixels;
    layers_[index].priority = priority;
    layers_[index].enabled = enabled;
    return true;
}

void Vdp2Compositor::disable_layer(std::size_t index) {
    if (index < 2) layers_[index].enabled = false;
}

void Vdp2Compositor::compose() {
    std::fill(output_.begin(), output_.end(), backdrop_);
    int first = 0, second = 1;
    // Draw lower priority first so the higher-priority opaque pixel wins.
    if (layers_[1].priority < layers_[0].priority) { first = 1; second = 0; }
    const int order[2] = {first, second};
    for (int n = 0; n < 2; ++n) {
        const auto& layer = layers_[order[n]];
        if (!layer.enabled) continue;
        for (std::size_t i = 0; i < output_.size(); ++i) {
            // In this prototype, zero is transparent; Saturn color math/priority rules are not modeled.
            if (layer.pixels[i] != 0) output_[i] = static_cast<std::uint16_t>(layer.pixels[i] & 0x7FFFu);
        }
    }
}

std::uint16_t Vdp2Compositor::pixel(std::size_t x, std::size_t y) const {
    if (x >= Width || y >= Height) return 0;
    return output_[y * Width + x];
}

} // namespace shining_saturn
