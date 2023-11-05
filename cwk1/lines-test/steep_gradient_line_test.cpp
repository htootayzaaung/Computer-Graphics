#include <catch2/catch_amalgamated.hpp>

#include <algorithm>

#include "helpers.hpp"

#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"

TEST_CASE("Steep Gradient Line Test", "[lines]")
{
    Surface surface(200, 200);
    surface.clear();
    ColorU8_sRGB color{255, 255, 255}; // Defined color
    Vec2f start{100.f, 10.f};
    Vec2f end{101.f, 190.f};
    
    draw_line_solid(surface, start, end, color);

    // Verify the line handles steep gradients correctly.
    // ... (assertions to be added)
}
