#include "../src/saturn/vdp2_compositor.hpp"
#include <cassert>
#include <climits>

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
    Vdp2Compositor::Layer middle(Vdp2Compositor::Width * Vdp2Compositor::Height, 0);
    middle[0] = 0x1234;
    assert(compositor.set_layer(2, middle, 3));
    assert(compositor.set_layer(3, high, 4));
    assert(compositor.set_layer(4, low, 5));
    compositor.compose();
    assert(compositor.pixel(0, 0) == 0x001F); // highest-priority opaque plane wins
    assert(!compositor.set_layer(5, low, 0));
    assert(!compositor.set_layer(0, Vdp2Compositor::Layer(1, 0), 0));
    assert(compositor.pixel(320, 0) == 0);
    Vdp2Compositor scroll_test;
    Vdp2Compositor::Layer dot(Vdp2Compositor::Width * Vdp2Compositor::Height, 0);
    dot[0] = 0x55AA;
    assert(scroll_test.set_layer(0, dot, 1));
    assert(scroll_test.set_scroll(0, 1, 1));
    scroll_test.compose();
    assert(scroll_test.pixel(1, 1) == 0x55AA);
    assert(scroll_test.pixel(0, 0) == 0);
    // Zero horizontal scroll takes the non-wrapped scanline path unchanged.
    assert(scroll_test.set_scroll(0, 0, 0));
    scroll_test.compose();
    assert(scroll_test.pixel(0, 0) == 0x55AA);
    assert(scroll_test.pixel(1, 1) == 0);
    // A one-pixel positive scroll moves source x=0 to destination x=1.
    assert(scroll_test.set_scroll(0, 1, 0));
    scroll_test.compose();
    assert(scroll_test.pixel(1, 0) == 0x55AA);
    assert(scroll_test.pixel(0, 0) == 0);
    assert(scroll_test.set_scroll(0, -1, -1));
    scroll_test.compose();
    assert(scroll_test.pixel(319, 223) == 0x55AA);
    assert(!scroll_test.set_scroll(5, 0, 0));
    assert(scroll_test.set_scroll(0, INT_MIN, INT_MAX));
    scroll_test.compose();
    assert(scroll_test.output().size() == Vdp2Compositor::Width * Vdp2Compositor::Height);

    // Layer enable/disable and backdrop updates are reflected on the next compose.
    Vdp2Compositor toggle_test;
    Vdp2Compositor::Layer stripe(Vdp2Compositor::Width * Vdp2Compositor::Height, 0);
    stripe[2 * Vdp2Compositor::Width + 3] = 0x1357;
    assert(toggle_test.set_layer(0, stripe, 1, false));
    toggle_test.clear_backdrop(0x2468);
    toggle_test.compose();
    assert(toggle_test.pixel(3, 2) == 0x2468);
    assert(toggle_test.set_layer(0, stripe, 1, true));
    toggle_test.compose();
    assert(toggle_test.pixel(3, 2) == 0x1357);
    toggle_test.clear_backdrop(0x1111);
    toggle_test.disable_layer(0);
    toggle_test.compose();
    assert(toggle_test.pixel(3, 2) == 0x1111);

    // Transparent overlay pixels leave the opaque lower layer visible.
    Vdp2Compositor::Layer base(Vdp2Compositor::Width * Vdp2Compositor::Height, 0);
    Vdp2Compositor::Layer overlay(Vdp2Compositor::Width * Vdp2Compositor::Height, 0);
    base[9] = 0x0123;
    overlay[10] = 0x0456;
    Vdp2Compositor transparency_test;
    assert(transparency_test.set_layer(0, base, 1));
    assert(transparency_test.set_layer(1, overlay, 2));
    transparency_test.compose();
    assert(transparency_test.pixel(9, 0) == 0x0123);
    assert(transparency_test.pixel(10, 0) == 0x0456);

    // Equal priorities keep deterministic plane-index order: the later plane wins.
    Vdp2Compositor tie_test;
    Vdp2Compositor::Layer tie_a(Vdp2Compositor::Width * Vdp2Compositor::Height, 0);
    Vdp2Compositor::Layer tie_b(Vdp2Compositor::Width * Vdp2Compositor::Height, 0);
    tie_a[7] = 0xFFFF; // RGB555 output must discard bit 15.
    tie_b[7] = 0x2222;
    tie_test.clear_backdrop(0xFFFF);
    assert(tie_test.set_layer(0, tie_a, 4));
    assert(tie_test.set_layer(1, tie_b, 4));
    tie_test.compose();
    assert(tie_test.pixel(7, 0) == 0x2222);
    assert(tie_test.pixel(8, 0) == 0x7FFF); // backdrop is masked to RGB555.
    assert(tie_test.set_layer(1, tie_b, 4, false));
    tie_test.compose();
    assert(tie_test.pixel(7, 0) == 0x7FFF);
    // Extreme scroll offsets must remain deterministic and bounded.
    Vdp2Compositor extreme_scroll;
    Vdp2Compositor::Layer corner(Vdp2Compositor::Width * Vdp2Compositor::Height, 0);
    corner[(Vdp2Compositor::Height - 1) * Vdp2Compositor::Width + (Vdp2Compositor::Width - 1)] = 0x0311;
    assert(extreme_scroll.set_layer(0, corner, 1));
    assert(extreme_scroll.set_scroll(0, INT_MIN, INT_MAX));
    extreme_scroll.compose();
    assert(extreme_scroll.output().size() == Vdp2Compositor::Width * Vdp2Compositor::Height);
    const int scroll_x = ((INT_MIN % static_cast<int>(Vdp2Compositor::Width)) + static_cast<int>(Vdp2Compositor::Width)) % static_cast<int>(Vdp2Compositor::Width);
    const int scroll_y = ((INT_MAX % static_cast<int>(Vdp2Compositor::Height)) + static_cast<int>(Vdp2Compositor::Height)) % static_cast<int>(Vdp2Compositor::Height);
    const int expected_extreme_x = (static_cast<int>(Vdp2Compositor::Width) - 1 + scroll_x) % static_cast<int>(Vdp2Compositor::Width);
    const int expected_extreme_y = (static_cast<int>(Vdp2Compositor::Height) - 1 + scroll_y) % static_cast<int>(Vdp2Compositor::Height);
    assert(extreme_scroll.pixel(static_cast<std::size_t>(expected_extreme_x), static_cast<std::size_t>(expected_extreme_y)) == 0x0311);
    assert(extreme_scroll.pixel(static_cast<std::size_t>((expected_extreme_x + 1) % static_cast<int>(Vdp2Compositor::Width)), static_cast<std::size_t>(expected_extreme_y)) == 0);

}

