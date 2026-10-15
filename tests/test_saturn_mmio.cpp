#include "../src/saturn/saturn_mmio.hpp"
#include <cassert>
#include <cstdint>
#include <vector>

int main() {
    using shining_saturn::MmioRegisterBank;
    MmioRegisterBank bank({
        {0x1000, 4, 0x12, true},
        {0x1004, 2, 0xA5, false},
        {0x1005, 4, 0x00, true}, // overlaps; must be rejected
        {0xFFFFFFFF, 2, 0, true} // wraps; must be rejected
    });
    assert(bank.handles(0x1000));
    assert(bank.read8(0x1003) == 0x12);
    assert(bank.write8(0x1002, 0x77));
    assert(bank.read8(0x1002) == 0x77);
    assert(!bank.write8(0x1004, 0));
    assert(bank.read8(0x1004) == 0xA5);
    assert(bank.read8(0x1005) == 0xA5);
    assert(!bank.handles(0x1006));
    assert(bank.read8(0x2000) == 0xFF);
    assert(bank.write16(0x1000, 0xABCD));
    assert(bank.read16(0x1000) == 0xABCD);
    assert(bank.write32(0x1000, 0x12345678u));
    assert(bank.read32(0x1000) == 0x12345678u);
    assert(!bank.write16(0x1003, 0x9999));
    assert(!bank.write32(0x1002, 0xAABBCCDDu));
    assert(bank.read16(0x1002) == 0x5678);
    assert(bank.read32(0xFFFFFFFFu) == 0xFFFFFFFFu);
    bank.reset();
    assert(bank.read8(0x1002) == 0x12);
}
