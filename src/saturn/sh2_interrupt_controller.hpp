#pragma once
#include <cstdint>

namespace shining_saturn {

enum class SaturnInterruptSource : std::uint8_t {
    Scu, Vdp1, Vdp2, CdBlock, Scsp, Smpc, Dma, ExternalIrq0, ExternalIrq1
};

struct InterruptRequest {
    std::uint8_t level = 0;
    std::uint8_t vector = 0;
    bool nmi = false;
    SaturnInterruptSource source = SaturnInterruptSource::Scu;
};

class Sh2InterruptController {
public:
    static constexpr std::uint32_t IPRB = 0xFFFFFE60u;
    static constexpr std::uint32_t VCRA = 0xFFFFFE62u;
    static constexpr std::uint32_t ICR  = 0xFFFFFEE0u;
    static constexpr std::uint32_t IPRA = 0xFFFFFEE2u;

    Sh2InterruptController();
    void reset();
    std::uint8_t read8(std::uint32_t address) const;
    void write8(std::uint32_t address, std::uint8_t value);
    std::uint16_t read16(std::uint32_t address) const;
    void write16(std::uint32_t address, std::uint16_t value);
    bool handles(std::uint32_t address) const;
    void request(SaturnInterruptSource source, std::uint8_t level, std::uint8_t vector);
    void request_nmi(bool line_high = true);
    bool acknowledge(std::uint8_t sr_mask, InterruptRequest& out);
    std::uint16_t ipra() const { return ipra_; }
    std::uint16_t iprb() const { return iprb_; }
    std::uint16_t icr() const { return icr_; }
    std::uint16_t vcra() const { return vcra_; }

private:
    std::uint16_t ipra_ = 0, iprb_ = 0, icr_ = 0x8000, vcra_ = 0;
    bool nmi_pending_ = false;
    bool nmi_line_high_ = true;
    bool pending_[9]{};
    InterruptRequest requests_[9]{};
};

} // namespace shining_saturn
