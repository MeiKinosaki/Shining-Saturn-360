#include "sh2_core.hpp"

namespace shining_saturn {

std::uint16_t Bus::read16(std::uint32_t a) {
    return (std::uint16_t(read8(a)) << 8) | read8(a + 1);
}
std::uint32_t Bus::read32(std::uint32_t a) {
    return (std::uint32_t(read8(a)) << 24) | (std::uint32_t(read8(a + 1)) << 16) |
           (std::uint32_t(read8(a + 2)) << 8) | std::uint32_t(read8(a + 3));
}
void Bus::write16(std::uint32_t a, std::uint16_t v) {
    write8(a, std::uint8_t(v >> 8)); write8(a + 1, std::uint8_t(v));
}
void Bus::write32(std::uint32_t a, std::uint32_t v) {
    write8(a, std::uint8_t(v >> 24)); write8(a + 1, std::uint8_t(v >> 16));
    write8(a + 2, std::uint8_t(v >> 8)); write8(a + 3, std::uint8_t(v));
}
void Sh2Core::reset(std::uint32_t start_pc) {
    r.fill(0); pc = start_pc; pr = gbr = vbr = mach = macl = sr = 0;
    halted = false; branch_pending_ = false; branch_target_ = 0;
    last_opcode_ = 0; fault_pc_ = 0;
}
void Sh2Core::schedule_branch(std::uint32_t target) {
    branch_pending_ = true; branch_target_ = target;
}

StepResult Sh2Core::step(Bus& bus) {
    if (halted) return StepResult::Halted;
    const bool completing_delay_slot = branch_pending_;
    const std::uint32_t delayed_target = branch_target_;
    branch_pending_ = false;
    fault_pc_ = pc;
    last_opcode_ = bus.read16(pc);
    const std::uint16_t op = last_opcode_;
    pc += 2;
    const unsigned n = (op >> 8) & 0x0f, m = (op >> 4) & 0x0f;
    const std::int32_t imm8 = static_cast<std::int8_t>(op & 0xff);
    const std::int32_t disp12 = (op & 0x0800) ? (std::int32_t(op & 0x0fff) - 0x1000)
                                                : std::int32_t(op & 0x0fff);

    if (op == 0x0009) { // NOP
    } else if (op == 0x000b) { // RTS, delay slot
        schedule_branch(pr);
    } else if ((op & 0xf000) == 0xe000) { // MOV #imm,Rn
        r[n] = static_cast<std::uint32_t>(imm8);
    } else if ((op & 0xf000) == 0x7000) { // ADD #imm,Rn
        r[n] += static_cast<std::uint32_t>(imm8);
    } else if ((op & 0xf00f) == 0x6003) { // MOV Rm,Rn
        r[n] = r[m];
    } else if ((op & 0xf00f) == 0x300c) { // ADD Rm,Rn
        r[n] += r[m];
    } else if ((op & 0xf00f) == 0x3008) { // SUB Rm,Rn
        r[n] -= r[m];
    } else if ((op & 0xf00f) == 0x3000) { // CMP/EQ Rm,Rn
        sr = (sr & ~kTBit) | (r[n] == r[m] ? kTBit : 0u);
    } else if ((op & 0xf00f) == 0x2009) { // AND Rm,Rn
        r[n] &= r[m];
    } else if ((op & 0xf00f) == 0x200a) { // XOR Rm,Rn
        r[n] ^= r[m];
    } else if ((op & 0xf00f) == 0x200b) { // OR Rm,Rn
        r[n] |= r[m];
    } else if ((op & 0xf00f) == 0x6000) { // MOV.B @Rm,Rn
        r[n] = static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int8_t>(bus.read8(r[m]))));
    } else if ((op & 0xf00f) == 0x6001) { // MOV.W @Rm,Rn
        r[n] = static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(bus.read16(r[m]))));
    } else if ((op & 0xf00f) == 0x6002) { // MOV.L @Rm,Rn
        r[n] = bus.read32(r[m]);
    } else if ((op & 0xf00f) == 0x2000) { // MOV.B Rm,@Rn
        bus.write8(r[n], static_cast<std::uint8_t>(r[m]));
    } else if ((op & 0xf00f) == 0x2001) { // MOV.W Rm,@Rn
        bus.write16(r[n], static_cast<std::uint16_t>(r[m]));
    } else if ((op & 0xf00f) == 0x2002) { // MOV.L Rm,@Rn
        bus.write32(r[n], r[m]);
    } else if ((op & 0xf000) == 0xa000) { // BRA disp12, delay slot
        schedule_branch(pc + 2 + static_cast<std::uint32_t>(disp12 * 2));
    } else if ((op & 0xf000) == 0xb000) { // BSR disp12, delay slot
        pr = pc + 2; schedule_branch(pc + 2 + static_cast<std::uint32_t>(disp12 * 2));
    } else if ((op & 0xff00) == 0x8900) { // BT disp8, no delay slot
        if (sr & kTBit) pc = pc + 2 + static_cast<std::uint32_t>(imm8 * 2);
    } else if ((op & 0xff00) == 0x8b00) { // BF disp8, no delay slot
        if (!(sr & kTBit)) pc = pc + 2 + static_cast<std::uint32_t>(imm8 * 2);
    } else if ((op & 0xf0ff) == 0x402b) { // JMP @Rn, delay slot
        schedule_branch(r[n]);
    } else if ((op & 0xf0ff) == 0x400b) { // JSR @Rn, delay slot
        pr = pc + 2; schedule_branch(r[n]);
    } else {
        pc = fault_pc_;
        if (completing_delay_slot) halted = true;
        return StepResult::UnsupportedOpcode;
    }
    if (completing_delay_slot) {
        if (branch_pending_) { halted = true; return StepResult::UnsupportedOpcode; }
        pc = delayed_target;
    }
    return StepResult::Executed;
}

} // namespace shining_saturn
