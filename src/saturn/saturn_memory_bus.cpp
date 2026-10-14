#include "saturn_memory_bus.hpp"

namespace shining_saturn {
namespace {
std::vector<MmioRegisterBank::Register> make_device_registers() {
    using namespace saturn_map;
    // Initial storage-backed MMIO ranges. These are not yet hardware-accurate
    // registers and do not implement device-specific side effects.
    return {
        {Smpc.base, static_cast<std::uint32_t>(Smpc.size), 0x00, true},
        {ScspRegisters.base, static_cast<std::uint32_t>(ScspRegisters.size), 0x00, true},
        {Vdp1Registers.base, static_cast<std::uint32_t>(Vdp1Registers.size), 0x00, true},
        {Vdp2Registers.base, static_cast<std::uint32_t>(Vdp2Registers.size), 0x00, true},
        {ScuRegisters.base, static_cast<std::uint32_t>(ScuRegisters.size), 0x00, true},
        {CdBlock.base, static_cast<std::uint32_t>(CdBlock.size), 0x00, true}
    };
}
} // namespace

SaturnMemoryBus::SaturnMemoryBus(const std::vector<std::uint8_t>& boot_rom)
    : mmio_(make_device_registers()) {
    using namespace saturn_map;
    if (!boot_rom.empty() && boot_rom.size() <= BootRom.size)
        storage_.map_rom(BootRom.base, boot_rom);

    storage_.map_ram(BackupRam.base, BackupRam.size);
    storage_.map_ram(WorkRamLow.base, WorkRamLow.size);
    storage_.map_ram(SoundRam.base, SoundRam.size);
    storage_.map_ram(Vdp1Vram.base, Vdp1Vram.size);
    storage_.map_ram(Vdp1FrameBuf.base, Vdp1FrameBuf.size);
    storage_.map_ram(Vdp2Vram.base, Vdp2Vram.size);
    storage_.map_ram(Vdp2Cram.base, Vdp2Cram.size);
    storage_.map_ram(WorkRamHigh.base, WorkRamHigh.size);
}

std::uint8_t SaturnMemoryBus::read8(std::uint32_t address) {
    if (mmio_.handles(address)) return mmio_.read8(address);
    return storage_.read8(address);
}

void SaturnMemoryBus::write8(std::uint32_t address, std::uint8_t value) {
    if (mmio_.handles(address)) {
        mmio_.write8(address, value);
        return;
    }
    storage_.write8(address, value);
}

bool SaturnMemoryBus::mapped_ram_or_rom(std::uint32_t address) const {
    return storage_.mapped(address);
}

saturn_map::Device SaturnMemoryBus::device_at(std::uint32_t address) const {
    return saturn_map::decode_physical(address);
}

} // namespace shining_saturn
