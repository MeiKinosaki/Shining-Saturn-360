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

void MmioRegisterBank::reset() {
    for (Entry& entry : entries_) for (std::uint8_t& byte : entry.bytes) byte = entry.spec.reset_value;
}

} // namespace shining_saturn
