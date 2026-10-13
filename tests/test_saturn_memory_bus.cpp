#include "../src/saturn/saturn_memory_bus.hpp"
#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>

using namespace shining_saturn;

int main() {
    const std::vector<std::uint8_t> boot{0x12, 0x34, 0x56, 0x78};
    SaturnMemoryBus bus(boot);
    assert(bus.read8(0) == 0x12);
    assert(bus.read8(3) == 0x78);
    bus.write8(0, 0xff);
    assert(bus.read8(0) == 0x12); // supplied boot ROM remains read-only

    bus.write8(0x00200000u, 0xab);
    assert(bus.read8(0x00200000u) == 0xab);
    bus.write8(0x06000010u, 0xcd);
    assert(bus.read8(0x06000010u) == 0xcd);
    bus.write8(0x05c00000u, 0x55);
    assert(bus.read8(0x05c00000u) == 0x55);

    assert(bus.device_at(0x00200000u) == saturn_map::Device::WorkRamLow);
    assert(bus.device_at(0x05d00000u) == saturn_map::Device::Vdp1Registers);
    assert(!bus.mapped_ram_or_rom(0x05d00000u)); // MMIO not implemented yet
    assert(!bus.mapped_ram_or_rom(0x00400000u));
    assert(bus.read8(0x00400000u) == 0xff);
    std::cout << "Saturn memory adapter tests passed\n";
}
