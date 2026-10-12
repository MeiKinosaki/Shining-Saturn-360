#pragma once

#include <array>
#include <cstdint>

namespace shining_saturn {

class Bus {
public:
    virtual ~Bus() = default;
    virtual std::uint8_t read8(std::uint32_t address) = 0;
    virtual void write8(std::uint32_t address, std::uint8_t value) = 0;
    std::uint16_t read16(std::uint32_t address);
    std::uint32_t read32(std::uint32_t address);
    void write16(std::uint32_t address, std::uint16_t value);
    void write32(std::uint32_t address, std::uint32_t value);
};

enum class StepResult { Executed, UnsupportedOpcode, Halted };

// Initial SH-2 interpreter foundation; not a complete Saturn CPU.
class Sh2Core {
public:
    static constexpr std::uint32_t kTBit = 1u;
    std::array<std::uint32_t, 16> r{};
    std::uint32_t pc = 0, pr = 0, gbr = 0, vbr = 0, mach = 0, macl = 0, sr = 0;
    bool halted = false;

    StepResult step(Bus& bus);
    void reset(std::uint32_t start_pc);
    std::uint16_t last_opcode() const { return last_opcode_; }
    std::uint32_t fault_pc() const { return fault_pc_; }

private:
    bool branch_pending_ = false;
    std::uint32_t branch_target_ = 0;
    std::uint16_t last_opcode_ = 0;
    std::uint32_t fault_pc_ = 0;
    void schedule_branch(std::uint32_t target);
};

} // namespace shining_saturn
