#pragma once

#include "sh2_core.hpp"
#include "saturn_address_decoder.hpp"
#include <cstdint>
#include <vector>

namespace shining_saturn {

// Minimal byte-addressable MMIO register bank. Device-specific side effects
// (DMA, IRQ generation, VDP commands, etc.) must be implemented by handlers.
class MmioRegisterBank {
public:
    struct Register {
        std::uint32_t base;
        std::uint32_t size;
        std::uint8_t reset_value;
        bool writable;
    };

    explicit MmioRegisterBank(std::vector<Register> registers);
    bool handles(std::uint32_t address) const;
    std::uint8_t read8(std::uint32_t address) const;
    std::uint16_t read16(std::uint32_t address) const;
    std::uint32_t read32(std::uint32_t address) const;
    bool write8(std::uint32_t address, std::uint8_t value);
    bool write16(std::uint32_t address, std::uint16_t value);
    bool write32(std::uint32_t address, std::uint32_t value);
    void reset();

private:
    struct Entry { Register spec; std::vector<std::uint8_t> bytes; };
    std::vector<Entry> entries_;
};

} // namespace shining_saturn
