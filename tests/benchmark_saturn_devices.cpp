#include "../src/saturn/scu_dma.hpp"
#include "../src/saturn/vdp1_renderer.hpp"
#include "../src/saturn/vdp2_compositor.hpp"

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <vector>

using namespace shining_saturn;

class DeviceBenchBus final : public Bus {
public:
    explicit DeviceBenchBus(std::size_t size) : bytes(size, 0) {}
    std::uint8_t read8(std::uint32_t address) override {
        return address < bytes.size() ? bytes[address] : 0;
    }
    void write8(std::uint32_t address, std::uint8_t value) override {
        if (address < bytes.size()) bytes[address] = value;
    }
    std::vector<std::uint8_t> bytes;
};

template <typename Fn>
static double measure_seconds(Fn&& fn) {
    const auto start = std::chrono::steady_clock::now();
    fn();
    const auto stop = std::chrono::steady_clock::now();
    return std::chrono::duration<double>(stop - start).count();
}

static std::uint64_t checksum16(const std::vector<std::uint16_t>& values) {
    std::uint64_t hash = 1469598103934665603ull;
    for (const auto value : values) {
        hash ^= value;
        hash *= 1099511628211ull;
    }
    return hash;
}

int main() {
    constexpr std::uint32_t Source = 0x1000;
    constexpr std::uint32_t Destination = 0x20000;
    constexpr std::uint32_t TransferBytes = 64 * 1024;
    constexpr std::size_t Iterations = 20;

    DeviceBenchBus bus(Destination + TransferBytes + 16);
    for (std::uint32_t i = 0; i < TransferBytes; ++i)
        bus.bytes[Source + i] = static_cast<std::uint8_t>((i * 37u + 11u) & 0xffu);

    ScuDma dma;
    if (!dma.configure(0, Source, Destination, TransferBytes) || !dma.start(0)) {
        std::cerr << "DMA setup failed\n";
        return 1;
    }
    std::size_t moved = 0;
    const double dma_seconds = measure_seconds([&] {
        while (dma.busy()) moved += dma.step(bus, 4096);
    });
    const auto* channel = dma.channel(0);
    if (moved != TransferBytes || !channel || !channel->completed || channel->remaining != 0) {
        std::cerr << "DMA completion/count assertion failed\n";
        return 2;
    }
    std::uint64_t dma_checksum = 1469598103934665603ull;
    for (std::uint32_t i = 0; i < TransferBytes; ++i) {
        if (bus.bytes[Destination + i] != bus.bytes[Source + i]) {
            std::cerr << "DMA data mismatch at offset " << i << "\n";
            return 3;
        }
        dma_checksum ^= bus.bytes[Destination + i];
        dma_checksum *= 1099511628211ull;
    }
    std::cout << "benchmark=scu_dma_copy\nbytes=" << moved
              << "\nelapsed_seconds=" << dma_seconds
              << "\nchecksum=" << dma_checksum << "\n";

    Vdp1Renderer vdp1;
    std::uint64_t vdp1_nonzero = 0;
    const double vdp1_seconds = measure_seconds([&] {
        for (std::size_t i = 0; i < Iterations; ++i) {
            vdp1.clear(0);
            vdp1.fill_rect(8, 12, 240, 120, 0x4210);
            vdp1.draw_line(0, static_cast<int>(i), 319, 223 - static_cast<int>(i), 0x7fff);
            vdp1.fill_triangle(30, 20, 290, 45, 160, 210, 0x03e0);
            vdp1_nonzero = vdp1.nonzero_pixels();
        }
    });
    if (vdp1_nonzero == 0) {
        std::cerr << "VDP1 workload produced an empty framebuffer\n";
        return 4;
    }
    std::cout << "benchmark=vdp1_software_primitives\niterations=" << Iterations
              << "\nelapsed_seconds=" << vdp1_seconds
              << "\nnonzero_pixels=" << vdp1_nonzero
              << "\nchecksum=" << checksum16(vdp1.pixels()) << "\n";

    Vdp2Compositor vdp2;
    Vdp2Compositor::Layer layer0(Vdp2Compositor::Width * Vdp2Compositor::Height, 0);
    Vdp2Compositor::Layer layer1(Vdp2Compositor::Width * Vdp2Compositor::Height, 0);
    for (std::size_t i = 0; i < layer0.size(); ++i) {
        if ((i % 7u) == 0) layer0[i] = static_cast<std::uint16_t>((i % 0x7ffeu) + 1u);
        if ((i % 11u) == 0) layer1[i] = static_cast<std::uint16_t>(((i * 3u) % 0x7ffeu) + 1u);
    }
    if (!vdp2.set_layer(0, layer0, 1) || !vdp2.set_layer(1, layer1, 2) ||
        !vdp2.set_scroll(0, 3, 2) || !vdp2.set_scroll(1, -5, 4)) {
        std::cerr << "VDP2 workload setup failed\n";
        return 5;
    }
    const double vdp2_seconds = measure_seconds([&] {
        for (std::size_t i = 0; i < Iterations; ++i) vdp2.compose();
    });
    const auto vdp2_checksum = checksum16(vdp2.output());
    if (vdp2_checksum == 0) {
        std::cerr << "VDP2 checksum assertion failed\n";
        return 6;
    }
    std::cout << "benchmark=vdp2_software_composition\niterations=" << Iterations
              << "\nelapsed_seconds=" << vdp2_seconds
              << "\nchecksum=" << vdp2_checksum << "\n";
    std::cout << "note=host-side prototypes only; not Saturn accuracy or Xbox 360 performance\n";
    return 0;
}
