#include "../src/saturn/vdp1_renderer.hpp"
#include <cassert>
#include <climits>

int main() {
    shining_saturn::Vdp1Renderer vdp;
    assert(vdp.pixels().size() == 320u * 224u);
    assert(vdp.nonzero_pixels() == 0);
    vdp.fill_rect(10, 20, 4, 3, 0x7FFF);
    assert(vdp.pixel(10, 20) == 0x7FFF);
    assert(vdp.pixel(13, 22) == 0x7FFF);
    assert(vdp.pixel(14, 22) == 0);
    assert(vdp.nonzero_pixels() == 12);
    vdp.fill_rect(-2, -1, 4, 3, 0x1234);
    assert(vdp.pixel(0, 0) == 0x1234);
    assert(vdp.nonzero_pixels() == 16);
    vdp.fill_rect(318, 222, 100, 100, 0x4567);
    assert(vdp.pixel(319, 223) == 0x4567);
    vdp.clear(0);
    vdp.draw_line(0, 0, 4, 4, 0x001F);
    assert(vdp.pixel(0, 0) == 0x001F);
    assert(vdp.pixel(2, 2) == 0x001F);
    assert(vdp.pixel(4, 4) == 0x001F);
    vdp.clear(0);
    vdp.fill_triangle(10, 10, 14, 10, 10, 14, 0x03E0);
    assert(vdp.pixel(10, 10) == 0x03E0);
    assert(vdp.pixel(11, 11) == 0x03E0);
    assert(vdp.pixel(14, 14) == 0);
    // Extreme rectangle coordinates are clipped without signed integer overflow.
    vdp.clear(0);
    vdp.fill_rect(INT_MAX, INT_MAX, INT_MAX, INT_MAX, 0x1234);
    assert(vdp.nonzero_pixels() == 0);
    vdp.fill_rect(INT_MIN, 10, INT_MAX, 2, 0x2345);
    assert(vdp.pixel(0, 10) == 0);
    vdp.draw_line(INT_MIN, 0, INT_MAX, 223, 0x3456);
    assert(vdp.nonzero_pixels() > 0);
    vdp.clear(0);
    vdp.draw_line(INT_MIN, INT_MIN, INT_MAX, INT_MAX, 0x3567);
    assert(vdp.nonzero_pixels() > 0);
    vdp.clear(0);
    vdp.draw_line(INT_MIN, 100, INT_MAX, 100, 0x3678);
    assert(vdp.nonzero_pixels() == 320);
    vdp.clear(0);
    vdp.fill_triangle(INT_MIN, INT_MIN, INT_MAX, 100, 100, INT_MAX, 0x4567);
    assert(vdp.nonzero_pixels() > 0);
    vdp.clear(0xFFFF);
    assert(vdp.pixel(0, 0) == 0x7FFF);
    assert(vdp.nonzero_pixels() == 320u * 224u);
}
