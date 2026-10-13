#pragma once

#include <cstdint>

namespace shining_saturn::saturn_map {

// Physical Saturn address map (cache-through aliases and device behavior are
// intentionally not implemented by these constants).
struct Region {
    std::uint32_t base;
    std::uint32_t size;
    constexpr std::uint64_t end() const { return std::uint64_t(base) + size; }
    constexpr bool contains(std::uint32_t address) const {
        return address >= base && std::uint64_t(address) < end();
    }
};

inline constexpr Region BootRom       {0x00000000u, 0x00080000u};
inline constexpr Region Smpc          {0x00100000u, 0x00000080u};
inline constexpr Region BackupRam     {0x00180000u, 0x00008000u};
inline constexpr Region WorkRamLow    {0x00200000u, 0x00100000u};
inline constexpr Region CartridgeBus  {0x02000000u, 0x02000000u};
inline constexpr Region CdBlock       {0x05890000u, 0x00010000u};
inline constexpr Region SoundRam      {0x05A00000u, 0x00080000u};
inline constexpr Region ScspRegisters {0x05B00000u, 0x00001000u};
inline constexpr Region Vdp1Vram      {0x05C00000u, 0x00080000u};
inline constexpr Region Vdp1FrameBuf  {0x05C80000u, 0x00080000u};
inline constexpr Region Vdp1Registers {0x05D00000u, 0x00000018u};
inline constexpr Region Vdp2Vram      {0x05E00000u, 0x00080000u};
inline constexpr Region Vdp2Cram      {0x05F00000u, 0x00008000u};
inline constexpr Region Vdp2Registers {0x05F80000u, 0x00000200u};
inline constexpr Region ScuRegisters  {0x05FE0000u, 0x00000100u};
inline constexpr Region WorkRamHigh   {0x06000000u, 0x00100000u};
inline constexpr Region Sh2OnChip     {0xFFFFFE00u, 0x00000200u};

constexpr bool valid(const Region& region) {
    return region.size != 0 && region.end() <= (std::uint64_t(1) << 32);
}

} // namespace shining_saturn::saturn_map
