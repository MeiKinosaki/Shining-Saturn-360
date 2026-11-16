#include "../src/saturn/saturn_memory_bus.hpp"
#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>

using namespace shining_saturn;

int main() {
    const std::vector<std::uint8_t> boot{0x12, 0x34, 0x56, 0x78};
    SaturnMemoryBus bus(boot);
    assert(bus.read8(0) == 0x12);
    assert(bus.read8(3) == 0x78);
    bus.write8(0, 0xff);
    assert(bus.read8(0) == 0x12); // supplied boot ROM remains read-only

    bus.write8(0x00200000u, 0xab);
    assert(bus.read8(0x00200000u) == 0xab);
    bus.write8(0x06000010u, 0xcd);
    assert(bus.read8(0x06000010u) == 0xcd);
    bus.write8(0x05c00000u, 0x55);
    assert(bus.read8(0x05c00000u) == 0x55);

    assert(bus.device_at(0x00200000u) == saturn_map::Device::WorkRamLow);
    assert(bus.device_at(0x05d00000u) == saturn_map::Device::Vdp1Registers);
    assert(!bus.mapped_ram_or_rom(0x05d00000u)); // MMIO is separate from RAM/ROM storage
    bus.write8(0x05d00000u, 0x42);
    assert(bus.read8(0x05d00000u) == 0x42); // generic register backing is connected

    // Bus helpers must preserve Saturn's big-endian byte order across MMIO.
    bus.write16(0x05d00010u, 0x1234);
    assert(bus.read8(0x05d00010u) == 0x12);
    assert(bus.read8(0x05d00011u) == 0x34);
    assert(bus.read16(0x05d00010u) == 0x1234);

    // VDP1 register space is only 0x18 bytes; use the wider VDP2 register bank here.
    bus.write32(0x05f80020u, 0x89abcdefu);
    assert(bus.read8(0x05f80020u) == 0x89);
    assert(bus.read8(0x05f80021u) == 0xab);
    assert(bus.read8(0x05f80022u) == 0xcd);
    assert(bus.read8(0x05f80023u) == 0xef);
    assert(bus.read32(0x05f80020u) == 0x89abcdefu);
    bus.write8(0x00100000u, 0x19);
    assert(bus.read8(0x00100000u) == 0x19);
    assert(!bus.mapped_ram_or_rom(0x00400000u));
    assert(bus.read8(0x00400000u) == 0xff);
    // Work RAM multi-width access uses big-endian ordering consistently.
    bus.write16(0x00200020u, 0xa1b2u);
    assert(bus.read8(0x00200020u) == 0xa1);
    assert(bus.read8(0x00200021u) == 0xb2);
    assert(bus.read16(0x00200020u) == 0xa1b2u);

    bus.write32(0x06000020u, 0x10203040u);
    assert(bus.read8(0x06000020u) == 0x10);
    assert(bus.read8(0x06000021u) == 0x20);
    assert(bus.read8(0x06000022u) == 0x30);
    assert(bus.read8(0x06000023u) == 0x40);
    assert(bus.read32(0x06000020u) == 0x10203040u);

    // Unmapped addresses stay open-bus and do not alias valid RAM.
    bus.write8(0x00400000u, 0x66);
    assert(bus.read8(0x00400000u) == 0xff);
    assert(bus.read8(0x00200000u) == 0xab);
    assert(bus.read8(0x06000010u) == 0xcd);

    // Adjacent RAM bytes do not bleed across a 16-bit write.
    bus.write8(0x00200030u, 0x5a);
    bus.write8(0x00200031u, 0xc3);
    bus.write16(0x00200032u, 0x1234u);
    assert(bus.read16(0x00200030u) == 0x5ac3u);
    assert(bus.read16(0x00200032u) == 0x1234u);

    // SH-2 on-chip INTC register window is mapped by the Saturn bus.
    bus.write16(Sh2InterruptController::IPRA, 0xabcdu);
    bus.write16(Sh2InterruptController::IPRB, 0x9876u);
    bus.write16(Sh2InterruptController::VCRA, 0xffffu);
    assert(bus.read16(Sh2InterruptController::IPRA) == 0xabc0u);
    assert(bus.read16(Sh2InterruptController::IPRB) == 0x9800u);
    assert(bus.read16(Sh2InterruptController::VCRA) == 0x7f7fu);
    bus.raise_device_interrupt(SaturnInterruptSource::Vdp1, 6, 0x48);
    InterruptRequest irq{};
    assert(bus.interrupt_controller().acknowledge(5, irq));
    assert(irq.source == SaturnInterruptSource::Vdp1 && irq.level == 6 && irq.vector == 0x48);
    bus.set_nmi_line(false);
    assert(bus.interrupt_controller().acknowledge(15, irq) && irq.nmi);
    bus.set_nmi_line(true);

    // CPU step consumes a device IRQ through Bus::acknowledge_interrupt.
    Sh2Core irq_cpu;
    irq_cpu.reset(0x00200080u);
    irq_cpu.vbr = 0x00201000u;
    irq_cpu.r[15] = 0x0020f000u;
    irq_cpu.sr = 0x10u;
    bus.write32(0x00201120u, 0x00200200u); // vector 0x48
    bus.raise_device_interrupt(SaturnInterruptSource::Vdp1, 6, 0x48);
    assert(irq_cpu.step(bus) == StepResult::Executed);
    assert(irq_cpu.pc == 0x00200200u);
    assert(bus.read32(0x0020eff8u) == 0x00200080u);
    assert(bus.read32(0x0020effcu) == 0x10u);
    assert(((irq_cpu.sr >> 4) & 0xfu) == 6u);

    std::cout << "Saturn memory adapter tests passed\n";
}
