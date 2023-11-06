#include <catch2/catch_amalgamated.hpp>
#include "helpers.hpp"
#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"

/*
    This test aims to verify that when two lines are drawn end-to-end, there is no gap between them. This continuity check is essential 
    for ensuring that lines appear as expected on the screen without any breaks, which can be visually jarring and technically incorrect.

    This test draws two lines : one from p0 to p1 and another from p1 to p2. Since p1 is the connecting point, the expectation is that the 
    two lines together should look like a single continuous line on the surface.

*/
TEST_CASE("Continuous Line Drawing Test", "[lines][!mayfail]") {
    Surface surface(200, 200);
    surface.clear();
    ColorU8_sRGB white{255, 255, 255}; // Color for the lines

    Vec2f p0{10.f, 100.f};          
    Vec2f p1{100.f, 100.f}; 
    Vec2f p2{200.f, 100.f};

    // Two calls to draw_line_solid draw two continuous horizontal lines that should connect seamlessly at p1.
    draw_line_solid(surface, p0, p1, white);
    draw_line_solid(surface, p1, p2, white);

    /*
        - The count_pixel_neighbours function is called to analyze the drawn lines. It counts how many neighbors each pixel has. For a perfectly 
        drawn continuous line, each pixel except the start and end should have exactly two neighbors.

        - counts[2] should equal the length of the line minus the two end pixels. However, since p1 is shared between both lines, you subtract 
        one less to account for the shared pixel, hence -1

    */
    auto const counts = count_pixel_neighbours(surface);
    std::size_t expectedCount = static_cast<std::size_t>(p2.x) - static_cast<std::size_t>(p0.x) - 1;

    // Verify the number of pixels with exactly 2 neighbors (should match the expected count)
    REQUIRE(counts[2] == expectedCount);
}

