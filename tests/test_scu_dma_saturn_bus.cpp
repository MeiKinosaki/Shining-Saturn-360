#include "../src/saturn/saturn_memory_bus.hpp"
#include "../src/saturn/scu_dma.hpp"
#include <cassert>
#include <cstdint>
#include <iostream>

using namespace shining_saturn;

int main() {
    SaturnMemoryBus bus;
    ScuDma dma;

    // Exercise the DMA against the actual Saturn address map, not a mock bus.
    for (std::uint32_t i = 0; i < 8; ++i)
        bus.write8(0x00200000u + i, static_cast<std::uint8_t>(0xA0u + i));

    assert(dma.configure(0, 0x00200000u, 0x00200020u, 8));
    assert(dma.start(0));
    assert(dma.step(bus, 3) == 3);
    assert(dma.channel(0)->remaining == 5);
    assert(bus.read8(0x00200020u) == 0xA0u);
    assert(bus.read8(0x00200022u) == 0xA2u);
    assert(bus.read8(0x00200023u) == 0x00u);
    assert(bus.read8(0x00200024u) == 0x00u);

    assert(dma.step(bus, 16) == 5);
    assert(!dma.busy());
    assert(dma.channel(0)->completed);
    for (std::uint32_t i = 0; i < 8; ++i)
        assert(bus.read8(0x00200020u + i) == static_cast<std::uint8_t>(0xA0u + i));

    // DMA can also write the storage-backed VDP2 register window, but this
    // still tests byte storage only; it does not model hardware side effects.
    bus.write8(0x00200040u, 0x12u);
    bus.write8(0x00200041u, 0x34u);
    assert(dma.configure(1, 0x00200040u, 0x05F80020u, 2));
    assert(dma.start(1));
    assert(dma.step(bus, 2) == 2);
    assert(bus.read16(0x05F80020u) == 0x1234u);
    assert(dma.channel(1)->completed);

    // Verify round-robin fairness using the real Saturn memory adapter.
    dma.reset();
    for (std::uint32_t i = 0; i < 3; ++i) {
        bus.write8(0x00200100u + i, static_cast<std::uint8_t>(0x30u + i));
        bus.write8(0x00200120u + i, static_cast<std::uint8_t>(0x60u + i));
    }
    assert(dma.configure(0, 0x00200100u, 0x00200200u, 3));
    assert(dma.configure(1, 0x00200120u, 0x00200220u, 3));
    assert(dma.start(0) && dma.start(1));
    assert(dma.step(bus, 1) == 1);
    assert(dma.channel(0)->remaining == 2);
    assert(dma.channel(1)->remaining == 3);
    assert(dma.step(bus, 1) == 1);
    assert(dma.channel(0)->remaining == 2);
    assert(dma.channel(1)->remaining == 2);
    assert(bus.read8(0x00200200u) == 0x30u);
    assert(bus.read8(0x00200220u) == 0x60u);
    assert(dma.step(bus, 4) == 4);
    assert(!dma.busy());
    for (std::uint32_t i = 0; i < 3; ++i) {
        assert(bus.read8(0x00200200u + i) == static_cast<std::uint8_t>(0x30u + i));
        assert(bus.read8(0x00200220u + i) == static_cast<std::uint8_t>(0x60u + i));
    }

    // Validate channel bounds, overflow rejection, and lifecycle transitions.
    assert(dma.configure(ScuDma::ChannelCount, 0x100u, 0x200u, 1) == false);
    assert(dma.configure(0, 0xFFFFFFFFu, 0x200u, 2) == false);
    assert(dma.configure(0, 0x100u, 0x200u, 0));
    assert(dma.start(0));
    assert(dma.channel(0)->completed);
    assert(!dma.channel(0)->active);
    assert(!dma.start(0)); // completed transfers must be reconfigured first

    assert(dma.configure(2, 0x00200100u, 0x00200300u, 2));
    assert(dma.start(2));
    assert(!dma.configure(2, 0x00200100u, 0x00200300u, 1)); // active channel
    assert(dma.step(bus, 2) == 2);
    assert(dma.channel(2)->completed);
    assert(dma.configure(2, 0x00200100u, 0x00200300u, 1)); // completed can be reconfigured
    assert(dma.start(2));
    assert(dma.step(bus, 1) == 1);
    assert(dma.channel(2)->completed);

    std::cout << "SCU DMA Saturn memory integration tests passed\\n";
}
