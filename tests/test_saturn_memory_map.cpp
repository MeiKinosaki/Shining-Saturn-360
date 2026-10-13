#include "../src/saturn/saturn_memory_map.hpp"
#include <cassert>
#include <iostream>

using namespace shining_saturn::saturn_map;

int main() {
    static_assert(valid(BootRom));
    static_assert(valid(WorkRamLow));
    static_assert(valid(Sh2OnChip));
    static_assert(BootRom.end() == 0x00080000ull);
    static_assert(WorkRamLow.end() == 0x00300000ull);
    static_assert(Sh2OnChip.end() == 0x100000000ull);

    assert(BootRom.contains(0x00000000u));
    assert(BootRom.contains(0x0007ffffu));
    assert(!BootRom.contains(0x00080000u));
    assert(WorkRamLow.contains(0x002fffffu));
    assert(!WorkRamLow.contains(0x00300000u));
    assert(Vdp1Registers.contains(0x05d00010u));
    assert(!Vdp1Registers.contains(0x05d00018u));
    assert(Sh2OnChip.contains(0xffffffffu));

    std::cout << "Saturn physical address-map constants passed\n";
}
