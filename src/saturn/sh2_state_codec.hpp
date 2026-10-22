#pragma once

#include "sh2_core.hpp"
#include <cstddef>
#include <cstdint>
#include <vector>

namespace shining_saturn {

// Versioned, endian-independent codec for CPU checkpoints only. This is not yet
// a full-machine save state: RAM and peripheral state need their own chunks.
class Sh2StateCodec {
public:
    static constexpr std::uint32_t FormatVersion = 1;

    static std::vector<std::uint8_t> encode(const Sh2Snapshot& s) {
        std::vector<std::uint8_t> out{'S','S','2','S'};
        put32(out, FormatVersion);
        for (auto v : s.r) put32(out, v);
        put32(out, s.pc); put32(out, s.pr); put32(out, s.gbr); put32(out, s.vbr);
        put32(out, s.mach); put32(out, s.macl); put32(out, s.sr);
        out.push_back(s.halted ? 1u : 0u);
        out.push_back(s.branch_pending ? 1u : 0u);
        put32(out, s.branch_target);
        put16(out, s.last_opcode);
        put32(out, s.fault_pc);
        put32(out, checksum(out.data(), out.size()));
        return out;
    }

    static bool decode(const std::vector<std::uint8_t>& bytes, Sh2Snapshot& out) {
        // 4-byte magic + version + 16 GPRs + 7 registers + 2 flags +
        // branch target + opcode + fault PC + checksum.
        constexpr std::size_t PayloadSize = 4 + 4 + 16*4 + 7*4 + 2 + 4 + 2 + 4 + 4;
        if (bytes.size() != PayloadSize) return false;
        if (bytes[0]!='S' || bytes[1]!='S' || bytes[2]!='2' || bytes[3]!='S') return false;
        if (get32(bytes, 4) != FormatVersion) return false;
        const std::uint32_t expected = get32(bytes, bytes.size()-4);
        if (checksum(bytes.data(), bytes.size()-4) != expected) return false;
        std::size_t p=8;
        Sh2Snapshot s;
        for (auto& v : s.r) { v=get32(bytes,p); p+=4; }
        s.pc=get32(bytes,p); p+=4; s.pr=get32(bytes,p); p+=4;
        s.gbr=get32(bytes,p); p+=4; s.vbr=get32(bytes,p); p+=4;
        s.mach=get32(bytes,p); p+=4; s.macl=get32(bytes,p); p+=4; s.sr=get32(bytes,p); p+=4;
        if (bytes[p]>1 || bytes[p+1]>1) return false;
        s.halted=bytes[p++]!=0; s.branch_pending=bytes[p++]!=0;
        s.branch_target=get32(bytes,p); p+=4;
        s.last_opcode=get16(bytes,p); p+=2;
        s.fault_pc=get32(bytes,p);
        out=s;
        return true;
    }

private:
    static void put16(std::vector<std::uint8_t>& b,std::uint16_t v) {
        b.push_back(static_cast<std::uint8_t>(v)); b.push_back(static_cast<std::uint8_t>(v>>8));
    }
    static void put32(std::vector<std::uint8_t>& b,std::uint32_t v) {
        for(unsigned i=0;i<4;++i)b.push_back(static_cast<std::uint8_t>(v>>(i*8)));
    }
    static std::uint16_t get16(const std::vector<std::uint8_t>& b,std::size_t p) {
        return static_cast<std::uint16_t>(b[p]) |
               static_cast<std::uint16_t>(static_cast<std::uint16_t>(b[p+1])<<8);
    }
    static std::uint32_t get32(const std::vector<std::uint8_t>& b,std::size_t p) {
        return std::uint32_t(b[p]) | (std::uint32_t(b[p+1])<<8) |
               (std::uint32_t(b[p+2])<<16) | (std::uint32_t(b[p+3])<<24);
    }
    static std::uint32_t checksum(const std::uint8_t* data,std::size_t size) {
        std::uint32_t h=2166136261u;
        for(std::size_t i=0;i<size;++i){h^=data[i];h*=16777619u;}
        return h;
    }
};

} // namespace shining_saturn
