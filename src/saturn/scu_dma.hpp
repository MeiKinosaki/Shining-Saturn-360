#pragma once

#include "sh2_core.hpp"
#include <array>
#include <cstddef>
#include <cstdint>

namespace shining_saturn {

// Deterministic byte-copy DMA prototype for SCU integration tests.
// It is not yet a register-accurate Saturn SCU implementation.
class ScuDma {
public:
    static constexpr std::size_t ChannelCount = 3;

    struct Channel {
        std::uint32_t source = 0;
        std::uint32_t destination = 0;
        std::uint32_t remaining = 0;
        bool active = false;
        bool completed = false;
    };

    void reset();
    bool configure(std::size_t channel, std::uint32_t source,
                   std::uint32_t destination, std::uint32_t byte_count);
    bool start(std::size_t channel);
    std::size_t step(Bus& bus, std::size_t byte_budget);
    const Channel* channel(std::size_t index) const;
    bool busy() const;

private:
    std::array<Channel, ChannelCount> channels_{};
    std::size_t next_channel_ = 0;
};

} // namespace shining_saturn
