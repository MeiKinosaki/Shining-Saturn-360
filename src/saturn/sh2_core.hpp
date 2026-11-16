#pragma once

#include <array>
#include <cstdint>
#include "sh2_interrupt_controller.hpp"

namespace shining_saturn {

class Bus {
public:
    virtual ~Bus() = default;
    virtual std::uint8_t read8(std::uint32_t address) = 0;
    virtual void write8(std::uint32_t address, std::uint8_t value) = 0;
    virtual bool acknowledge_interrupt(std::uint8_t sr_mask, InterruptRequest& out) { (void)sr_mask; (void)out; return false; }
    virtual void raise_device_interrupt(SaturnInterruptSource source, std::uint8_t level, std::uint8_t vector) { (void)source; (void)level; (void)vector; }
    std::uint16_t read16(std::uint32_t address);
    std::uint32_t read32(std::uint32_t address);
    void write16(std::uint32_t address, std::uint16_t value);
    void write32(std::uint32_t address, std::uint32_t value);
    bool address_error() const { return address_error_; }
    void clear_address_error() { address_error_ = false; }
};

enum class StepResult { Executed, UnsupportedOpcode, Halted };

// Portable CPU-only checkpoint. Memory and device state must be captured separately.
struct Sh2Snapshot {
    std::array<std::uint32_t, 16> r{};
    std::uint32_t pc = 0, pr = 0, gbr = 0, vbr = 0, mach = 0, macl = 0, sr = 0;
    bool halted = false;
    bool branch_pending = false;
    std::uint32_t branch_target = 0;
    bool rte_pending = false;
    std::uint32_t rte_sr = 0;
    std::uint8_t interrupt_level = 0;
    std::uint8_t interrupt_vector = 0;
    std::uint16_t last_opcode = 0;
    std::uint32_t fault_pc = 0;
};

// Initial SH-2 interpreter foundation; not a complete Saturn CPU.
class Sh2Core {
public:
    static constexpr std::uint32_t kTBit = 1u;
    std::array<std::uint32_t, 16> r{};
    std::uint32_t pc = 0, pr = 0, gbr = 0, vbr = 0, mach = 0, macl = 0, sr = 0;
    bool halted = false;

    StepResult step(Bus& bus);
    void reset(std::uint32_t start_pc);
    void request_interrupt(std::uint8_t level, std::uint8_t vector);
    bool raise_exception(Bus& bus, std::uint8_t vector, std::uint32_t saved_pc);
    std::uint16_t last_opcode() const { return last_opcode_; }
    std::uint32_t fault_pc() const { return fault_pc_; }
    Sh2Snapshot snapshot() const;
    void restore(const Sh2Snapshot& state);

private:
    bool branch_pending_ = false;
    std::uint32_t branch_target_ = 0;
    bool rte_pending_ = false;
    std::uint32_t rte_sr_ = 0;
    std::uint8_t interrupt_level_ = 0;
    std::uint8_t interrupt_vector_ = 0;
    std::uint16_t last_opcode_ = 0;
    std::uint32_t fault_pc_ = 0;
    bool address_error_ = false;
    void schedule_branch(std::uint32_t target);
    bool enter_exception(Bus& bus, std::uint8_t vector, std::uint32_t saved_pc, std::uint8_t level);
};

} // namespace shining_saturn
