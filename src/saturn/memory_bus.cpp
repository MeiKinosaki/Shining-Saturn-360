#include "memory_bus.hpp"
#include <limits>

namespace shining_saturn {

bool MemoryBus::overlaps(std::uint32_t base, std::size_t size) const {
    if (size == 0 || size > std::numeric_limits<std::uint32_t>::max() ||
        std::uint64_t(base) + size > (std::uint64_t(1) << 32)) return true;
    const std::uint64_t end = std::uint64_t(base) + size;
    for (const auto& region : regions_) {
        const std::uint64_t other_end = std::uint64_t(region.base) + region.bytes.size();
        if (std::uint64_t(base) < other_end && std::uint64_t(region.base) < end) return true;
    }
    return false;
}

bool MemoryBus::map_ram(std::uint32_t base, std::size_t size, std::uint8_t initial) {
    if (overlaps(base, size)) return false;
    regions_.push_back(Region{base, std::vector<std::uint8_t>(size, initial), true});
    return true;
}

bool MemoryBus::map_rom(std::uint32_t base, const std::vector<std::uint8_t>& bytes) {
    if (overlaps(base, bytes.size())) return false;
    regions_.push_back(Region{base, bytes, false});
    return true;
}

bool MemoryBus::mapped(std::uint32_t address) const {
    for (const auto& region : regions_) {
        if (address >= region.base &&
            std::uint64_t(address) < std::uint64_t(region.base) + region.bytes.size()) return true;
    }
    return false;
}

std::uint8_t MemoryBus::read8(std::uint32_t address) {
    for (const auto& region : regions_) {
        if (address >= region.base &&
            std::uint64_t(address) < std::uint64_t(region.base) + region.bytes.size())
            return region.bytes[address - region.base];
    }
    return 0xff;
}

void MemoryBus::write8(std::uint32_t address, std::uint8_t value) {
    for (auto& region : regions_) {
        if (region.writable && address >= region.base &&
            std::uint64_t(address) < std::uint64_t(region.base) + region.bytes.size()) {
            region.bytes[address - region.base] = value;
            return;
        }
    }
}

} // namespace shining_saturn
