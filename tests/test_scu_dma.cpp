#include "../src/saturn/scu_dma.hpp"
#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>

using namespace shining_saturn;

class DmaTestBus final : public Bus {
public:
    std::array<std::uint8_t, 256> bytes{};
    std::uint8_t read8(std::uint32_t address) override { return bytes.at(address); }
    void write8(std::uint32_t address, std::uint8_t value) override { bytes.at(address) = value; }
};

int main() {
    DmaTestBus bus;
    ScuDma dma;
    for (std::uint32_t i = 0; i < 16; ++i) bus.bytes[16 + i] = static_cast<std::uint8_t>(i + 0x40);

    assert(dma.configure(0, 16, 80, 8));
    assert(dma.start(0));
    assert(dma.busy());
    assert(dma.step(bus, 3) == 3);
    assert(dma.channel(0)->remaining == 5);
    assert(dma.channel(0)->active);
    assert(dma.step(bus, 16) == 5);
    assert(!dma.busy());
    assert(dma.channel(0)->completed);
    assert(dma.channel(0)->remaining == 0);
    for (std::uint32_t i = 0; i < 8; ++i) assert(bus.bytes[80 + i] == i + 0x40);

    assert(!dma.configure(ScuDma::ChannelCount, 0, 0, 1));
    assert(!dma.configure(2, UINT32_MAX - 1u, 0x40, 4));
    assert(!dma.configure(2, 0x20, UINT32_MAX - 1u, 4));
    assert(dma.channel(ScuDma::ChannelCount) == nullptr);
    assert(dma.configure(1, 32, 96, 0));
    assert(dma.start(1));
    assert(dma.channel(1)->completed);
    assert(!dma.busy());

    // One-byte budgets rotate among active channels instead of starving channel 1.
    dma.reset();
    assert(dma.configure(0, 16, 120, 2));
    assert(dma.configure(1, 32, 130, 2));
    assert(dma.start(0) && dma.start(1));
    assert(dma.step(bus, 1) == 1);
    assert(dma.channel(0)->remaining == 1);
    assert(dma.channel(1)->remaining == 2);
    assert(dma.step(bus, 1) == 1);
    assert(dma.channel(1)->remaining == 1);
    assert(dma.step(bus, 0) == 0);

    dma.reset();
    assert(!dma.busy());
    assert(dma.channel(0)->remaining == 0);
    // A short channel must complete while a longer transfer remains active.
    dma.reset();
    assert(dma.configure(0, 16, 140, 4));
    assert(dma.configure(1, 32, 150, 1));
    assert(dma.start(0) && dma.start(1));
    assert(dma.step(bus, 2) == 2);
    assert(dma.channel(1)->completed);
    assert(dma.channel(0)->remaining == 3);
    assert(dma.busy());
    assert(dma.step(bus, 8) == 3);
    assert(!dma.busy());

    // Three active channels each receive one byte in a three-byte budget.
    dma.reset();
    assert(dma.configure(0, 16, 160, 3));
    assert(dma.configure(1, 32, 170, 3));
    assert(dma.configure(2, 48, 180, 3));
    assert(dma.start(0) && dma.start(1) && dma.start(2));
    assert(dma.step(bus, 3) == 3);
    assert(dma.channel(0)->remaining == 2);
    assert(dma.channel(1)->remaining == 2);
    assert(dma.channel(2)->remaining == 2);
    assert(dma.step(bus, 6) == 6);
    assert(!dma.busy());
    for (std::uint32_t i = 0; i < 3; ++i) {
        assert(bus.bytes[160 + i] == bus.bytes[16 + i]);
        assert(bus.bytes[170 + i] == bus.bytes[32 + i]);
        assert(bus.bytes[180 + i] == bus.bytes[48 + i]);
    }

    std::cout << "SCU DMA prototype tests passed\n";
}
