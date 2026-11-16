#include "../src/saturn/sh2_interrupt_controller.hpp"
#include <cassert>
#include <cstdint>
#include <iostream>
using namespace shining_saturn;

int main() {
    Sh2InterruptController intc;
    assert(intc.read16(Sh2InterruptController::ICR) == 0x8000u);
    intc.write16(Sh2InterruptController::IPRA, 0x123fu);
    intc.write16(Sh2InterruptController::IPRB, 0xabcd);
    intc.write16(Sh2InterruptController::VCRA, 0xffff);
    assert(intc.ipra() == 0x1230u);
    assert(intc.iprb() == 0xab00u);
    assert(intc.vcra() == 0x7f7fu);
    intc.write16(Sh2InterruptController::ICR, 0xffff);
    assert(intc.read16(Sh2InterruptController::ICR) == 0x8101u);

    intc.request(SaturnInterruptSource::Vdp2, 5, 0x45);
    intc.request(SaturnInterruptSource::Scu, 9, 0x40);
    InterruptRequest irq{};
    assert(!intc.acknowledge(9, irq));
    assert(intc.pending(SaturnInterruptSource::Scu));
    assert(intc.acknowledge(4, irq));
    assert(irq.level == 9 && irq.vector == 0x40 && !irq.nmi);
    assert(!intc.pending(SaturnInterruptSource::Scu));
    assert(intc.acknowledge(4, irq));
    assert(irq.level == 5 && irq.vector == 0x45);

    intc.request_nmi(false);
    assert(intc.read16(Sh2InterruptController::ICR) == 0x0101u);
    assert(intc.acknowledge(15, irq) && irq.nmi && irq.vector == 11);
    intc.request_nmi(false); // held low does not retrigger edge-sensitive NMI
    assert(!intc.acknowledge(0, irq));
    intc.request_nmi(true);
    intc.request_nmi(false);
    assert(intc.acknowledge(0, irq) && irq.nmi);

    intc.reset();
    assert(intc.read16(Sh2InterruptController::IPRA) == 0);
    assert(intc.read16(Sh2InterruptController::ICR) == 0x8000u);
    std::cout << "SH-2 interrupt controller tests passed\n";
}
