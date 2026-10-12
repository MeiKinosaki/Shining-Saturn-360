#pragma once

#include "sh2_core.hpp"
#include <cstddef>
#include <cstdint>
#include <vector>

namespace shining_saturn {

// Explicitly mapped byte-addressable bus. Unmapped reads return 0xFF;
// writes to unmapped or read-only regions are ignored. Device registers and
// Saturn-specific address decoding can be added as separate mapped regions.
class MemoryBus final : public Bus {
public:
    bool map_ram(std::uint32_t base, std::size_t size, std::uint8_t initial = 0);
    bool map_rom(std::uint32_t base, const std::vector<std::uint8_t>& bytes);

    std::uint8_t read8(std::uint32_t address) override;
    void write8(std::uint32_t address, std::uint8_t value) override;
    bool mapped(std::uint32_t address) const;

private:
    struct Region {
        std::uint32_t base;
        std::vector<std::uint8_t> bytes;
        bool writable;
    };
    std::vector<Region> regions_;
    bool overlaps(std::uint32_t base, std::size_t size) const;
};

} // namespace shining_saturn
