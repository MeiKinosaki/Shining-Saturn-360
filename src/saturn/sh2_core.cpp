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
    } else if ((op & 0xf000) == 0x9000) { // MOV.W @(disp,PC),Rn
        const std::uint32_t address = pc + 2 + static_cast<std::uint32_t>(op & 0xff) * 2;
        r[n] = static_cast<std::uint32_t>(static_cast<std::int32_t>(
            static_cast<std::int16_t>(bus.read16(address))));
    } else if ((op & 0xf000) == 0xd000) { // MOV.L @(disp,PC),Rn
        const std::uint32_t address = (pc & ~std::uint32_t(3)) +
                                      static_cast<std::uint32_t>(op & 0xff) * 4;
        r[n] = bus.read32(address);
    } else if ((op & 0xf000) == 0x1000) { // MOV.L Rm,@(disp,Rn)
        const std::uint32_t address = r[n] + static_cast<std::uint32_t>(op & 0x0f) * 4;
        bus.write32(address, r[m]);
    } else if ((op & 0xf000) == 0x5000) { // MOV.L @(disp,Rm),Rn
        const std::uint32_t address = r[m] + static_cast<std::uint32_t>(op & 0x0f) * 4;
        r[n] = bus.read32(address);
    } else if ((op & 0xf00f) == 0x6003) { // MOV Rm,Rn
        r[n] = r[m];
    } else if ((op & 0xf00f) == 0x300c) { // ADD Rm,Rn
        r[n] += r[m];
    } else if ((op & 0xf00f) == 0x3008) { // SUB Rm,Rn
        r[n] -= r[m];
    } else if ((op & 0xf00f) == 0x3000) { // CMP/EQ Rm,Rn
        sr = (sr & ~kTBit) | (r[n] == r[m] ? kTBit : 0u);
    } else if ((op & 0xf00f) == 0x3002) { // CMP/HS Rm,Rn (unsigned)
        sr = (sr & ~kTBit) | (r[n] >= r[m] ? kTBit : 0u);
    } else if ((op & 0xf00f) == 0x3006) { // CMP/HI Rm,Rn (unsigned)
        sr = (sr & ~kTBit) | (r[n] > r[m] ? kTBit : 0u);
    } else if ((op & 0xf00f) == 0x3003) { // CMP/GE Rm,Rn (signed)
        sr = (sr & ~kTBit) | (static_cast<std::int32_t>(r[n]) >= static_cast<std::int32_t>(r[m]) ? kTBit : 0u);
    } else if ((op & 0xf00f) == 0x3007) { // CMP/GT Rm,Rn (signed)
        sr = (sr & ~kTBit) | (static_cast<std::int32_t>(r[n]) > static_cast<std::int32_t>(r[m]) ? kTBit : 0u);
    } else if ((op & 0xf00f) == 0x200c) { // CMP/STR Rm,Rn
        const std::uint32_t x = r[n] ^ r[m];
        const bool equal_byte = ((x & 0x000000ffu) == 0) || ((x & 0x0000ff00u) == 0) ||
                                ((x & 0x00ff0000u) == 0) || ((x & 0xff000000u) == 0);
        sr = (sr & ~kTBit) | (equal_byte ? kTBit : 0u);
    } else if ((op & 0xf00f) == 0x300e) { // ADDC Rm,Rn
        const std::uint32_t lhs = r[n], rhs = r[m], carry_in = sr & kTBit;
        const std::uint32_t partial = lhs + rhs;
        const bool carry1 = partial < lhs;
        const std::uint32_t result = partial + carry_in;
        const bool carry2 = result < partial;
        r[n] = result;
        sr = (sr & ~kTBit) | ((carry1 || carry2) ? kTBit : 0u);
    } else if ((op & 0xf00f) == 0x300a) { // SUBC Rm,Rn
        const std::uint32_t lhs = r[n], rhs = r[m], borrow_in = sr & kTBit;
        const std::uint32_t partial = lhs - rhs;
        const bool borrow1 = lhs < rhs;
        const std::uint32_t result = partial - borrow_in;
        const bool borrow2 = partial < borrow_in;
        r[n] = result;
        sr = (sr & ~kTBit) | ((borrow1 || borrow2) ? kTBit : 0u);
    } else if ((op & 0xf00f) == 0x300f) { // ADDV Rm,Rn
        const std::uint32_t lhs = r[n], rhs = r[m], result = lhs + rhs;
        r[n] = result;
        sr = (sr & ~kTBit) | (((~(lhs ^ rhs) & (lhs ^ result)) >> 31) ? kTBit : 0u);
    } else if ((op & 0xf00f) == 0x300b) { // SUBV Rm,Rn
        const std::uint32_t lhs = r[n], rhs = r[m], result = lhs - rhs;
        r[n] = result;
        sr = (sr & ~kTBit) | ((((lhs ^ rhs) & (lhs ^ result)) >> 31) ? kTBit : 0u);
    } else if ((op & 0xf00f) == 0x2009) { // AND Rm,Rn
        r[n] &= r[m];
    } else if ((op & 0xf00f) == 0x200a) { // XOR Rm,Rn
        r[n] ^= r[m];
    } else if ((op & 0xf00f) == 0x200b) { // OR Rm,Rn
        r[n] |= r[m];
    } else if (op == 0x0008) { // CLRT
        sr &= ~kTBit;
    } else if (op == 0x0018) { // SETT
        sr |= kTBit;
    } else if (op == 0x0028) { // CLRMAC
        mach = macl = 0;
    } else if ((op & 0xf00f) == 0x0007) { // MUL.L Rm,Rn
        macl = r[n] * r[m];
    } else if ((op & 0xf00f) == 0x200e) { // MULU.W Rm,Rn
        macl = (r[n] & 0xffffu) * (r[m] & 0xffffu);
    } else if ((op & 0xf00f) == 0x200f) { // MULS.W Rm,Rn
        const std::int32_t lhs = static_cast<std::int16_t>(r[n]);
        const std::int32_t rhs = static_cast<std::int16_t>(r[m]);
        macl = static_cast<std::uint32_t>(lhs * rhs);
    } else if ((op & 0xf00f) == 0x3005) { // DMULU.L Rm,Rn
        const std::uint64_t product = std::uint64_t(r[n]) * std::uint64_t(r[m]);
        mach = static_cast<std::uint32_t>(product >> 32);
        macl = static_cast<std::uint32_t>(product);
    } else if ((op & 0xf00f) == 0x300d) { // DMULS.L Rm,Rn
        const std::int64_t product = std::int64_t(static_cast<std::int32_t>(r[n])) *
                                     std::int64_t(static_cast<std::int32_t>(r[m]));
        const std::uint64_t bits = static_cast<std::uint64_t>(product);
        mach = static_cast<std::uint32_t>(bits >> 32);
        macl = static_cast<std::uint32_t>(bits);
    } else if (op == 0x0029) { // MOVT Rn
        r[n] = sr & kTBit;
    } else if ((op & 0xf0ff) == 0x4000) { // SHLL Rn
        sr = (sr & ~kTBit) | ((r[n] >> 31) & kTBit);
        r[n] <<= 1;
    } else if ((op & 0xf0ff) == 0x4001) { // SHLR Rn
        sr = (sr & ~kTBit) | (r[n] & kTBit);
        r[n] >>= 1;
    } else if ((op & 0xf0ff) == 0x4020) { // SHAL Rn
        sr = (sr & ~kTBit) | ((r[n] >> 31) & kTBit);
        r[n] <<= 1;
    } else if ((op & 0xf0ff) == 0x4021) { // SHAR Rn
        sr = (sr & ~kTBit) | (r[n] & kTBit);
        r[n] = static_cast<std::uint32_t>(static_cast<std::int32_t>(r[n]) >> 1);
    } else if ((op & 0xf0ff) == 0x4024) { // ROTCL Rn
        const std::uint32_t old_t = sr & kTBit;
        sr = (sr & ~kTBit) | ((r[n] >> 31) & kTBit);
        r[n] = (r[n] << 1) | old_t;
    } else if ((op & 0xf0ff) == 0x4025) { // ROTCR Rn
        const std::uint32_t old_t = sr & kTBit;
        sr = (sr & ~kTBit) | (r[n] & kTBit);
        r[n] = (r[n] >> 1) | (old_t << 31);
    } else if ((op & 0xf0ff) == 0x4028) { // ROTL Rn
        sr = (sr & ~kTBit) | ((r[n] >> 31) & kTBit);
        r[n] = (r[n] << 1) | (r[n] >> 31);
    } else if ((op & 0xf0ff) == 0x4029) { // ROTR Rn
        sr = (sr & ~kTBit) | (r[n] & kTBit);
        r[n] = (r[n] >> 1) | (r[n] << 31);
    } else if ((op & 0xf0ff) == 0x4010) { // DT Rn
        --r[n];
        sr = (sr & ~kTBit) | (r[n] == 0 ? kTBit : 0u);
    } else if ((op & 0xf0ff) == 0x4011) { // CMP/PZ Rn
        sr = (sr & ~kTBit) | (static_cast<std::int32_t>(r[n]) >= 0 ? kTBit : 0u);
    } else if ((op & 0xf0ff) == 0x4015) { // CMP/PL Rn
        sr = (sr & ~kTBit) | (static_cast<std::int32_t>(r[n]) > 0 ? kTBit : 0u);
    } else if ((op & 0xf00f) == 0x6007) { // NOT Rm,Rn
        r[n] = ~r[m];
    } else if ((op & 0xf00f) == 0x600b) { // NEG Rm,Rn
        r[n] = 0u - r[m];
    } else if ((op & 0xf00f) == 0x200d) { // XTRCT Rm,Rn
        r[n] = (r[n] >> 16) | (r[m] << 16);
    } else if ((op & 0xf00f) == 0x6008) { // SWAP.B Rm,Rn
        r[n] = (r[m] & 0xffff0000u) | ((r[m] & 0x000000ffu) << 8) |
               ((r[m] & 0x0000ff00u) >> 8);
    } else if ((op & 0xf00f) == 0x6009) { // SWAP.W Rm,Rn
        r[n] = (r[m] << 16) | (r[m] >> 16);
    } else if ((op & 0xf00f) == 0x600c) { // EXTU.B Rm,Rn
        r[n] = r[m] & 0xffu;
    } else if ((op & 0xf00f) == 0x600d) { // EXTU.W Rm,Rn
        r[n] = r[m] & 0xffffu;
    } else if ((op & 0xf00f) == 0x600e) { // EXTS.B Rm,Rn
        r[n] = static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int8_t>(r[m])));
    } else if ((op & 0xf00f) == 0x600f) { // EXTS.W Rm,Rn
        r[n] = static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(r[m])));
    } else if ((op & 0xff00) == 0xc800) { // TST #imm,R0
        sr = (sr & ~kTBit) | ((r[0] & (op & 0xff)) == 0 ? kTBit : 0u);
    } else if ((op & 0xff00) == 0xc900) { // AND #imm,R0
        r[0] &= op & 0xff;
    } else if ((op & 0xff00) == 0xca00) { // XOR #imm,R0
        r[0] ^= op & 0xff;
    } else if ((op & 0xff00) == 0xcb00) { // OR #imm,R0
        r[0] |= op & 0xff;
    } else if ((op & 0xf00f) == 0x6000) { // MOV.B @Rm,Rn
        r[n] = static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int8_t>(bus.read8(r[m]))));
    } else if ((op & 0xf00f) == 0x6001) { // MOV.W @Rm,Rn
        r[n] = static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(bus.read16(r[m]))));
    } else if ((op & 0xf00f) == 0x6002) { // MOV.L @Rm,Rn
        r[n] = bus.read32(r[m]);
    } else if ((op & 0xf00f) == 0x6004) { // MOV.B @Rm+,Rn
        r[n] = static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int8_t>(bus.read8(r[m]))));
        if (n != m) ++r[m];
    } else if ((op & 0xf00f) == 0x6005) { // MOV.W @Rm+,Rn
        r[n] = static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<std::int16_t>(bus.read16(r[m]))));
        if (n != m) r[m] += 2;
    } else if ((op & 0xf00f) == 0x6006) { // MOV.L @Rm+,Rn
        r[n] = bus.read32(r[m]);
        if (n != m) r[m] += 4;
    } else if ((op & 0xf00f) == 0x2004) { // MOV.B Rm,@-Rn
        --r[n]; bus.write8(r[n], static_cast<std::uint8_t>(r[m]));
    } else if ((op & 0xf00f) == 0x2005) { // MOV.W Rm,@-Rn
        r[n] -= 2; bus.write16(r[n], static_cast<std::uint16_t>(r[m]));
    } else if ((op & 0xf00f) == 0x2006) { // MOV.L Rm,@-Rn
        r[n] -= 4; bus.write32(r[n], r[m]);
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
