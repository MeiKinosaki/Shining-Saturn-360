#include "../src/saturn/sh2_core.hpp"
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace shining_saturn;

class BenchBus final : public Bus {
public:
    explicit BenchBus(std::size_t size) : bytes(size, 0) {}
    std::uint8_t read8(std::uint32_t address) override { return bytes.at(address); }
    void write8(std::uint32_t address, std::uint8_t value) override { bytes.at(address) = value; }
    std::vector<std::uint8_t> bytes;
};

int main() {
    constexpr std::uint32_t Count = 100000;
    BenchBus bus(Count * 2u + 2u);
    for (std::uint32_t i = 0; i < Count; ++i) {
        bus.bytes[i * 2u] = 0x00;
        bus.bytes[i * 2u + 1u] = 0x09; // SH-2 NOP
    }
    Sh2Core cpu;
    cpu.reset(0);
    const auto start = std::chrono::steady_clock::now();
    for (std::uint32_t i = 0; i < Count; ++i) {
        if (cpu.step(bus) != StepResult::Executed) {
            std::cerr << "SH-2 step failed at " << i << "\n";
            return 1;
        }
    }
    const auto stop = std::chrono::steady_clock::now();
    const double elapsed = std::chrono::duration<double>(stop - start).count();
    std::cout << "benchmark=sh2_nop_stream\n"
              << "instructions=" << Count << "\n"
              << "elapsed_seconds=" << std::fixed << std::setprecision(6) << elapsed << "\n"
              << "instructions_per_second=" << std::fixed << std::setprecision(0)
              << (elapsed > 0.0 ? Count / elapsed : 0.0) << "\n";
    return cpu.pc == Count * 2u ? 0 : 2;
}
