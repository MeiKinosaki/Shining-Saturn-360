#include "saturn_mmio.hpp"

namespace shining_saturn {

MmioRegisterBank::MmioRegisterBank(std::vector<Register> registers) {
    for (const Register& reg : registers) {
        if (reg.size == 0 || reg.base > UINT32_MAX - (reg.size - 1)) continue;
        bool overlap = false;
        const std::uint64_t end = static_cast<std::uint64_t>(reg.base) + reg.size;
        for (const Entry& entry : entries_) {
            const std::uint64_t other_end =
                static_cast<std::uint64_t>(entry.spec.base) + entry.spec.size;
            if (reg.base < other_end && entry.spec.base < end) { overlap = true; break; }
        }
        if (!overlap) entries_.push_back(Entry{reg, std::vector<std::uint8_t>(reg.size, reg.reset_value)});
    }
}

bool MmioRegisterBank::handles(std::uint32_t address) const {
    for (const Entry& entry : entries_)
        if (address >= entry.spec.base &&
            static_cast<std::uint64_t>(address) < static_cast<std::uint64_t>(entry.spec.base) + entry.spec.size)
            return true;
    return false;
}

std::uint8_t MmioRegisterBank::read8(std::uint32_t address) const {
    for (const Entry& entry : entries_) {
        if (address >= entry.spec.base &&
            static_cast<std::uint64_t>(address) < static_cast<std::uint64_t>(entry.spec.base) + entry.spec.size)
            return entry.bytes[address - entry.spec.base];
    }
    return 0xFF;
}

std::uint16_t MmioRegisterBank::read16(std::uint32_t address) const {
    if (address > UINT32_MAX - 1u) return 0xFFFFu;
    return static_cast<std::uint16_t>((std::uint16_t(read8(address)) << 8) | read8(address + 1u));
}

std::uint32_t MmioRegisterBank::read32(std::uint32_t address) const {
    if (address > UINT32_MAX - 3u) return 0xFFFFFFFFu;
    return (std::uint32_t(read8(address)) << 24) | (std::uint32_t(read8(address + 1u)) << 16) |
           (std::uint32_t(read8(address + 2u)) << 8) | std::uint32_t(read8(address + 3u));
}

bool MmioRegisterBank::write8(std::uint32_t address, std::uint8_t value) {
    for (Entry& entry : entries_) {
        if (address >= entry.spec.base &&
            static_cast<std::uint64_t>(address) < static_cast<std::uint64_t>(entry.spec.base) + entry.spec.size) {
            if (!entry.spec.writable) return false;
            entry.bytes[address - entry.spec.base] = value;
            return true;
        }
    }
    return false;
}

bool MmioRegisterBank::write16(std::uint32_t address, std::uint16_t value) {
    if (address > UINT32_MAX - 1u || !handles(address) || !handles(address + 1u)) return false;
    auto writable_at = [this](std::uint32_t a) {
        for (const Entry& entry : entries_)
            if (a >= entry.spec.base && static_cast<std::uint64_t>(a) <
                static_cast<std::uint64_t>(entry.spec.base) + entry.spec.size) return entry.spec.writable;
        return false;
    };
    if (!writable_at(address) || !writable_at(address + 1u)) return false;
    write8(address, static_cast<std::uint8_t>(value >> 8));
    write8(address + 1u, static_cast<std::uint8_t>(value));
    return true;
}

bool MmioRegisterBank::write32(std::uint32_t address, std::uint32_t value) {
    if (address > UINT32_MAX - 3u) return false;
    for (std::uint32_t i = 0; i < 4; ++i) {
        const std::uint32_t a = address + i;
        if (!handles(a)) return false;
        bool writable = false;
        for (const Entry& entry : entries_)
            if (a >= entry.spec.base && static_cast<std::uint64_t>(a) <
                static_cast<std::uint64_t>(entry.spec.base) + entry.spec.size) {
                writable = entry.spec.writable; break;
            }
        if (!writable) return false;
    }
    for (std::uint32_t i = 0; i < 4; ++i)
        write8(address + i, static_cast<std::uint8_t>(value >> (24u - i * 8u)));
    return true;
}

void MmioRegisterBank::reset() {
    for (Entry& entry : entries_) for (std::uint8_t& byte : entry.bytes) byte = entry.spec.reset_value;
}

} // namespace shining_saturn
