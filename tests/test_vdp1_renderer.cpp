#include "../src/saturn/vdp1_renderer.hpp"
#include <cassert>

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
    vdp.clear(0);\n    vdp.draw_line(0, 0, 4, 4, 0x001F);\n    assert(vdp.pixel(0, 0) == 0x001F);\n    assert(vdp.pixel(2, 2) == 0x001F);\n    assert(vdp.pixel(4, 4) == 0x001F);\n    vdp.clear(0);\n    vdp.fill_triangle(10, 10, 14, 10, 10, 14, 0x03E0);\n    assert(vdp.pixel(10, 10) == 0x03E0);\n    assert(vdp.pixel(11, 11) == 0x03E0);\n    assert(vdp.pixel(14, 14) == 0);\n    vdp.clear(0xFFFF);
    assert(vdp.pixel(0, 0) == 0x7FFF);
    assert(vdp.nonzero_pixels() == 320u * 224u);
}
