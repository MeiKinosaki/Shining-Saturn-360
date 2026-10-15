#include "scu_dma.hpp"

namespace shining_saturn {

void ScuDma::reset() {
    channels_ = {};
}

bool ScuDma::configure(std::size_t index, std::uint32_t source,
                       std::uint32_t destination, std::uint32_t byte_count) {
    if (index >= channels_.size() || channels_[index].active) return false;
    if (byte_count != 0 &&
        (source > UINT32_MAX - (byte_count - 1u) ||
         destination > UINT32_MAX - (byte_count - 1u))) return false;
    channels_[index] = Channel{source, destination, byte_count, false, false};
    return true;
}

bool ScuDma::start(std::size_t index) {
    if (index >= channels_.size()) return false;
    Channel& ch = channels_[index];
    if (ch.active || ch.completed) return false;
    ch.active = ch.remaining != 0;
    ch.completed = ch.remaining == 0;
    return true;
}

std::size_t ScuDma::step(Bus& bus, std::size_t byte_budget) {
    std::size_t moved = 0;
    for (Channel& ch : channels_) {
        while (ch.active && ch.remaining != 0 && moved < byte_budget) {
            const std::uint8_t value = bus.read8(ch.source);
            bus.write8(ch.destination, value);
            ++ch.source;
            ++ch.destination;
            --ch.remaining;
            ++moved;
        }
        if (ch.active && ch.remaining == 0) {
            ch.active = false;
            ch.completed = true;
        }
        if (moved == byte_budget) break;
    }
    return moved;
}

const ScuDma::Channel* ScuDma::channel(std::size_t index) const {
    return index < channels_.size() ? &channels_[index] : nullptr;
}

bool ScuDma::busy() const {
    for (const Channel& ch : channels_) if (ch.active) return true;
    return false;
}

} // namespace shining_saturn
