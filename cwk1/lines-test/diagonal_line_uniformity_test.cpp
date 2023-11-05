#include <catch2/catch_amalgamated.hpp>

#include "helpers.hpp"
#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"

TEST_CASE("Diagonal Line Uniformity Test", "[lines]") {
    Surface surface(200, 200);
    surface.clear();
    ColorU8_sRGB white{255, 255, 255}; // Color for the lines

    Vec2f start{0.f, 0.f};
    Vec2f end{199.f, 199.f};

    // Draw the line
    draw_line_solid(surface, start, end, white);

    // Verify that the line is uniformly drawn across the diagonal
    bool uniform = true;
    for (float i = 0; i <= 199; ++i) {
        Vec2f point{i, i}; // Since it's a diagonal, x and y are the same
        if (!pixelMatchesColor(surface, point, white)) {
            uniform = false;
            break;
        }
    }

    // The line should be continuous and uniform across the entire diagonal
    REQUIRE(uniform);
}
