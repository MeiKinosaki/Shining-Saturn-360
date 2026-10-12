#include "../src/saturn/memory_bus.hpp"
#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>

using namespace shining_saturn;

int main() {
    MemoryBus bus;
    assert(bus.map_ram(0x00200000, 0x1000));
    assert(!bus.map_ram(0x00200800, 0x1000)); // overlapping range rejected
    assert(!bus.map_ram(0x00300000, 0)); // empty range rejected
    assert(bus.map_rom(0x00000000, {0x12, 0x34, 0x56, 0x78}));
    assert(!bus.map_ram(0x00000002, 4)); // cannot overlap ROM

    bus.write32(0x00200000, 0x1234abcd);
    assert(bus.read8(0x00200000) == 0x12);
    assert(bus.read16(0x00200000) == 0x1234);
    assert(bus.read32(0x00200000) == 0x1234abcd);

    assert(bus.read32(0x00000000) == 0x12345678);
    bus.write8(0x00000000, 0xff); // ROM write ignored
    assert(bus.read8(0x00000000) == 0x12);
    assert(bus.read8(0x00400000) == 0xff); // unmapped open-bus policy
    bus.write8(0x00400000, 0x99);
    assert(!bus.mapped(0x00400000));
    assert(bus.mapped(0x00200fff));
    assert(!bus.mapped(0x00201000));
    std::cout << "MemoryBus tests passed\n";
}
