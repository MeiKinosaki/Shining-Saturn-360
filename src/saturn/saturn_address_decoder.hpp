#pragma once

#include "saturn_memory_map.hpp"
#include <cstdint>

namespace shining_saturn::saturn_map {

enum class Device : std::uint8_t {
    Unmapped,
    BootRom,
    Smpc,
    BackupRam,
    WorkRamLow,
    CartridgeBus,
    CdBlock,
    SoundRam,
    ScspRegisters,
    Vdp1Vram,
    Vdp1FrameBuffer,
    Vdp1Registers,
    Vdp2Vram,
    Vdp2ColorRam,
    Vdp2Registers,
    ScuRegisters,
    WorkRamHigh,
    Sh2OnChip
};

// Classifies the documented physical range only. This is not an MMIO handler:
// register side effects, mirrors, access permissions and arbitration are future work.
constexpr Device decode_physical(std::uint32_t address) {
    if (BootRom.contains(address)) return Device::BootRom;
    if (Smpc.contains(address)) return Device::Smpc;
    if (BackupRam.contains(address)) return Device::BackupRam;
    if (WorkRamLow.contains(address)) return Device::WorkRamLow;
    if (CartridgeBus.contains(address)) return Device::CartridgeBus;
    if (CdBlock.contains(address)) return Device::CdBlock;
    if (SoundRam.contains(address)) return Device::SoundRam;
    if (ScspRegisters.contains(address)) return Device::ScspRegisters;
    if (Vdp1Vram.contains(address)) return Device::Vdp1Vram;
    if (Vdp1FrameBuf.contains(address)) return Device::Vdp1FrameBuffer;
    if (Vdp1Registers.contains(address)) return Device::Vdp1Registers;
    if (Vdp2Vram.contains(address)) return Device::Vdp2Vram;
    if (Vdp2Cram.contains(address)) return Device::Vdp2ColorRam;
    if (Vdp2Registers.contains(address)) return Device::Vdp2Registers;
    if (ScuRegisters.contains(address)) return Device::ScuRegisters;
    if (WorkRamHigh.contains(address)) return Device::WorkRamHigh;
    if (Sh2OnChip.contains(address)) return Device::Sh2OnChip;
    return Device::Unmapped;
}

} // namespace shining_saturn::saturn_map
