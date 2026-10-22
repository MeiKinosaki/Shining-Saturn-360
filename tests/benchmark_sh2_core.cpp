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

static bool run_workload(const char* name, BenchBus& bus, Sh2Core& cpu,
                         std::uint32_t count, std::uint32_t data_address,
                         std::uint16_t opcode, bool uses_memory) {
    for (std::uint32_t i = 0; i < count; ++i) {
        bus.bytes[i * 2u] = static_cast<std::uint8_t>(opcode >> 8);
        bus.bytes[i * 2u + 1u] = static_cast<std::uint8_t>(opcode);
    }
    cpu.reset(0);
    if (uses_memory) cpu.r[1] = data_address;
    const auto start = std::chrono::steady_clock::now();
    for (std::uint32_t i = 0; i < count; ++i) {
        if (cpu.step(bus) != StepResult::Executed) {
            std::cerr << "SH-2 workload " << name << " failed at instruction " << i << "\n";
            return false;
        }
    }
    const auto stop = std::chrono::steady_clock::now();
    const double elapsed = std::chrono::duration<double>(stop - start).count();
    std::cout << "benchmark=" << name << "\n"
              << "instructions=" << count << "\n"
              << "elapsed_seconds=" << std::fixed << std::setprecision(6) << elapsed << "\n"
              << "instructions_per_second=" << std::fixed << std::setprecision(0)
              << (elapsed > 0.0 ? count / elapsed : 0.0) << "\n";
    if (cpu.pc != count * 2u) {
        std::cerr << "Unexpected PC after workload " << name << ": " << cpu.pc << "\n";
        return false;
    }
    return true;
}

int main() {
    constexpr std::uint32_t Count = 100000;
    constexpr std::uint32_t DataAddress = 0x40000;
    BenchBus bus(DataAddress + 16u);
    bus.bytes[DataAddress] = 0x12;
    bus.bytes[DataAddress + 1u] = 0x34;
    bus.bytes[DataAddress + 2u] = 0x56;
    bus.bytes[DataAddress + 3u] = 0x78;
    Sh2Core cpu;

    // NOP and MOV.L @R1,R0 (0x6012) provide separate decode-only and memory paths.
    if (!run_workload("sh2_nop_stream", bus, cpu, Count, DataAddress, 0x0009, false))
        return 1;
    if (!run_workload("sh2_movl_indirect_load", bus, cpu, Count, DataAddress, 0x6012, true))
        return 1;
    if (cpu.r[0] != 0x12345678u) {
        std::cerr << "MOV.L benchmark returned unexpected data: " << cpu.r[0] << "\n";
        return 2;
    }
    return 0;
}
