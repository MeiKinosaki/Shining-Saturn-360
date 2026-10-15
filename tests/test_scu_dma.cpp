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
    assert(dma.channel(ScuDma::ChannelCount) == nullptr);
    assert(dma.configure(1, 32, 96, 0));
    assert(dma.start(1));
    assert(dma.channel(1)->completed);
    assert(!dma.busy());

    dma.reset();
    assert(!dma.busy());
    assert(dma.channel(0)->remaining == 0);
    std::cout << "SCU DMA prototype tests passed\n";
}
