#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace shining_saturn {

// Early software compositor for test framebuffers; not a hardware-accurate VDP2.
class Vdp2Compositor {
public:
    static constexpr std::size_t Width = 320;
    static constexpr std::size_t Height = 224;
    using Layer = std::vector<std::uint16_t>;

    Vdp2Compositor();
    void clear_backdrop(std::uint16_t rgb555);
    bool set_layer(std::size_t index, const Layer& pixels, int priority, bool enabled = true);
    void disable_layer(std::size_t index);
    void compose();
    std::uint16_t pixel(std::size_t x, std::size_t y) const;
    const Layer& output() const { return output_; }

private:
    struct LayerState {
        Layer pixels;
        int priority = 0;
        bool enabled = false;
    };
    std::uint16_t backdrop_ = 0;
    std::array<LayerState, 5> layers_{};
    Layer output_;
};

} // namespace shining_saturn
