#include "../src/saturn/saturn_address_decoder.hpp"
#include <cassert>
#include <iostream>

using namespace shining_saturn::saturn_map;

int main() {
    assert(decode_physical(0x00000000u) == Device::BootRom);
    assert(decode_physical(0x00100020u) == Device::Smpc);
    assert(decode_physical(0x00212345u) == Device::WorkRamLow);
    assert(decode_physical(0x05c00010u) == Device::Vdp1Vram);
    assert(decode_physical(0x05d00004u) == Device::Vdp1Registers);
    assert(decode_physical(0x05e00010u) == Device::Vdp2Vram);
    assert(decode_physical(0x05fe0000u) == Device::ScuRegisters);
    assert(decode_physical(0x06000010u) == Device::WorkRamHigh);
    assert(decode_physical(0x00400000u) == Device::Unmapped);
    assert(decode_physical(0xffffffffu) == Device::Sh2OnChip);
    std::cout << "Saturn physical address decoder tests passed\n";
}
