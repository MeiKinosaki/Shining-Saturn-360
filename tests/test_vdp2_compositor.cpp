#include "../src/saturn/vdp2_compositor.hpp"
#include <cassert>

int main() {
    using namespace shining_saturn;
    Vdp2Compositor compositor;
    Vdp2Compositor::Layer low(Vdp2Compositor::Width * Vdp2Compositor::Height, 0);
    Vdp2Compositor::Layer high(Vdp2Compositor::Width * Vdp2Compositor::Height, 0);
    low[0] = 0x001F;
    low[1] = 0x03E0;
    high[1] = 0x7C00;
    compositor.clear_backdrop(0x4210);
    assert(compositor.set_layer(0, low, 1));
    assert(compositor.set_layer(1, high, 2));
    compositor.compose();
    assert(compositor.pixel(0, 0) == 0x001F);
    assert(compositor.pixel(1, 0) == 0x7C00);
    assert(compositor.pixel(2, 0) == 0x4210);
    compositor.disable_layer(1);
    compositor.compose();
    assert(compositor.pixel(1, 0) == 0x03E0);
    assert(!compositor.set_layer(2, low, 0));
    assert(!compositor.set_layer(0, Vdp2Compositor::Layer(1, 0), 0));
    assert(compositor.pixel(320, 0) == 0);
}
