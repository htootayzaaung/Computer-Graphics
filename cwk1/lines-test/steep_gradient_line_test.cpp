#include <catch2/catch_amalgamated.hpp>
#include "helpers.hpp"
#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"

/*
    This test is designed to verify that the line-drawing algorithm can accurately render lines that have a steep gradient. 
    Such lines can be challenging for some algorithms because small changes in the x-coordinate result in large changes in 
    the y-coordinate, which can lead to gaps or visual inconsistencies if not handled properly.

    Purpose:
        - Gradient Challenges: Steep lines test the algorithm's ability to handle large differences in the y-direction for 
        minimal changes in the x-direction.

        - Visual Continuity: Ensures the line is continuous and there are no gaps or missing pixels, which is critical for 
        steep lines where errors are more noticeable.
    
    Failure Analysis: 
        - Algorithm Limitation: If the test fails, it may indicate that the line-drawing algorithm does not handle the error 
        term correctly when the gradient is steep, leading to missed pixels.

        - Implementation Error: There could be an issue in the way the pixels are being plotted, or the condition for stepping 
        in the x or y direction may not be accurate for steep gradients.
*/

// Verify that a steep line is drawn without breaks
bool isSteepLineContinuous(const Surface& surface, Vec2f start, Vec2f end, const ColorU8_sRGB& color) {
    int dx = static_cast<int>(end.x - start.x);
    int dy = static_cast<int>(end.y - start.y);
    int absDx = std::abs(dx);
    int absDy = std::abs(dy);
    
    // We expect a steep line, so dy should be greater than dx
    if (absDy <= absDx) {
        return false;
    }

    for (int i = 0; i <= absDy; ++i) {
        int x = static_cast<int>(start.x) + dx * i / absDy;
        int y = static_cast<int>(start.y) + i;

        if (!pixelMatchesColor(surface, Vec2f{static_cast<float>(x), static_cast<float>(y)}, color)) {
            return false;
        }
    }

    return true;
}

TEST_CASE("Steep Gradient Line Test", "[lines][!mayfail]") {
    Surface surface(200, 200);
    surface.clear();
    ColorU8_sRGB white{255, 255, 255}; // White color for the line

    Vec2f start{100.f, 10.f};
    Vec2f end{101.f, 190.f};

    draw_line_solid(surface, start, end, white);

    REQUIRE(isSteepLineContinuous(surface, start, end, white));
}
