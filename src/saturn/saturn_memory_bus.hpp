#pragma once

#include "memory_bus.hpp"
#include "saturn_address_decoder.hpp"
#include <cstdint>
#include <vector>

namespace shining_saturn {

// Initial Saturn memory adapter. RAM-like storage is mapped; device registers,
// cartridge and CD block are deliberately left unmapped until their handlers exist.
class SaturnMemoryBus final : public Bus {
public:
    explicit SaturnMemoryBus(const std::vector<std::uint8_t>& boot_rom = {});

    std::uint8_t read8(std::uint32_t address) override;
    void write8(std::uint32_t address, std::uint8_t value) override;
    bool mapped_ram_or_rom(std::uint32_t address) const;
    saturn_map::Device device_at(std::uint32_t address) const;

private:
    MemoryBus storage_;
};

} // namespace shining_saturn
