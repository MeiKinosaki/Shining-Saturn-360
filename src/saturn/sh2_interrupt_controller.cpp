#include "sh2_interrupt_controller.hpp"
#include <algorithm>

namespace shining_saturn {
namespace {
constexpr std::uint32_t kFirst = Sh2InterruptController::IPRB;
constexpr std::uint32_t kLast = Sh2InterruptController::IPRA + 1u;
}

Sh2InterruptController::Sh2InterruptController() { reset(); }
void Sh2InterruptController::reset() {
    ipra_ = iprb_ = vcra_ = 0;
    icr_ = 0x8000u; // Saturn SH-2 NMI input is pulled high at reset.
    nmi_pending_ = false;
    nmi_line_high_ = true;
    for (unsigned i = 0; i < 9; ++i) { pending_[i] = false; requests_[i] = {}; }
}
bool Sh2InterruptController::handles(std::uint32_t a) const {
    return (a >= IPRB && a <= VCRA + 1u) || (a >= ICR && a <= IPRA + 1u);
}
std::uint8_t Sh2InterruptController::read8(std::uint32_t a) const {
    const std::uint16_t v = read16(a & ~1u);
    return (a & 1u) ? static_cast<std::uint8_t>(v) : static_cast<std::uint8_t>(v >> 8);
}
void Sh2InterruptController::write8(std::uint32_t a, std::uint8_t v) {
    const std::uint32_t aligned = a & ~1u;
    std::uint16_t old = read16(aligned);
    old = (a & 1u) ? static_cast<std::uint16_t>((old & 0xff00u) | v)
                   : static_cast<std::uint16_t>((old & 0x00ffu) | (std::uint16_t(v) << 8));
    write16(aligned, old);
}
std::uint16_t Sh2InterruptController::read16(std::uint32_t a) const {
    switch (a) {
    case IPRB: return iprb_;
    case VCRA: return vcra_;
    case ICR: return static_cast<std::uint16_t>((icr_ & 0x7f7fu) | (nmi_line_high_ ? 0x8000u : 0u));
    case IPRA: return ipra_;
    default: return 0xffffu;
    }
}
void Sh2InterruptController::write16(std::uint32_t a, std::uint16_t v) {
    switch (a) {
    case IPRB: iprb_ = v & 0xff00u; break;
    case VCRA: vcra_ = v & 0x7f7fu; break;
    case ICR: icr_ = static_cast<std::uint16_t>((icr_ & 0x8000u) | (v & 0x0101u)); break;
    case IPRA: ipra_ = v & 0xfff0u; break;
    default: break;
    }
}
void Sh2InterruptController::request(SaturnInterruptSource source, std::uint8_t level, std::uint8_t vector) {
    const unsigned i = static_cast<unsigned>(source);
    if (i >= 9 || level == 0 || level > 15) return;
    requests_[i] = {level, vector, false, source};
    pending_[i] = true;
}
void Sh2InterruptController::request_nmi(bool line_high) {
    if (!line_high && nmi_line_high_) nmi_pending_ = true;
    nmi_line_high_ = line_high;
}
bool Sh2InterruptController::pending(SaturnInterruptSource source) const {
    const unsigned i = static_cast<unsigned>(source);
    return i < 9 && pending_[i];
}
bool Sh2InterruptController::acknowledge(std::uint8_t sr_mask, InterruptRequest& out) {
    if (nmi_pending_) {
        nmi_pending_ = false;
        out = {16, 11, true, SaturnInterruptSource::Scu};
        return true;
    }
    int best = -1;
    for (unsigned i = 0; i < 9; ++i) {
        if (!pending_[i]) continue;
        if (requests_[i].level <= sr_mask) continue;
        if (best < 0 || requests_[i].level > requests_[best].level ||
            (requests_[i].level == requests_[best].level && i < static_cast<unsigned>(best)))
            best = static_cast<int>(i);
    }
    if (best < 0) return false;
    out = requests_[best];
    pending_[best] = false;
    return true;
}
} // namespace shining_saturn
