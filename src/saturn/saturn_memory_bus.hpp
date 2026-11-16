#pragma once

#include "memory_bus.hpp"
#include "saturn_address_decoder.hpp"
#include "saturn_mmio.hpp"
#include "sh2_interrupt_controller.hpp"
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
    Sh2InterruptController& interrupt_controller() { return interrupt_controller_; }
    const Sh2InterruptController& interrupt_controller() const { return interrupt_controller_; }
    void raise_device_interrupt(SaturnInterruptSource source, std::uint8_t level, std::uint8_t vector);
    void set_nmi_line(bool high);

private:
    MemoryBus storage_;
    MmioRegisterBank mmio_;
    Sh2InterruptController interrupt_controller_;
};

} // namespace shining_saturn
