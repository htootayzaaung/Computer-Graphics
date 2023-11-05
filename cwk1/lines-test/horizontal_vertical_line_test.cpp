#include <catch2/catch_amalgamated.hpp>
#include "helpers.hpp"
#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"
#include "../draw2d/color.hpp"

/*
    The purpose of this test is to ensure the correctness and integrity of the line drawing functionality provided by the draw_line_solid function in your 
    graphics application. Here's a breakdown of the reasoning behind this test and its importance:

    1.) Horizontal Line Test:
        - This test checks whether a horizontal line drawn from point start to point end using draw_line_solid results in the expected number of pixels 
        being set to the desired color (white in this case).
        - The line is expected to be continuous without any gaps or unexpected colored pixels.
        - The checkLineIntegrity function verifies that the exact number of pixels along the line's path are colored and that this number matches the 
        expected pixel count (the difference in x-coordinates plus one).

    2.) Verical Line Test:
        - Similar to the horizontal line test, but for a vertical line.
        - It checks that a vertical line from start to end results in the correct number of pixels being set to white.
        - This test is crucial because vertical and horizontal lines are often edge cases for line drawing algorithms due to their alignment with the pixel 
        grid.

    Reasoning behind the Test:

    - Algorithm Validation: 
        The Bresenham algorithm, which is typically used for line drawing in raster graphics, is particularly optimized for drawing straight lines efficiently. 
        It's important to ensure that the implementation correctly handles edge cases like perfectly horizontal or vertical lines.

    - Edge Case Handling:
        Horizontal and vertical lines may not require the same conditional logic as diagonal lines in some implementations. This test ensures that such cases 
        are handled correctly.

    - Visual Continuity:
        For lines, visual continuity is essential. Any gaps or extra pixels would be easily noticeable and undesirable in a graphical application.
*/

// Helper function to count colored pixels in a row or column
int countColoredPixels(const Surface& surface, unsigned int x, unsigned int y, bool isHorizontal, const ColorU8_sRGB& expectedColor) {
    int count = 0;
    const auto* ptr = surface.get_surface_ptr();
    auto width = surface.get_width();
    auto height = surface.get_height();

    if (isHorizontal) {
        for (unsigned int i = 0; i < width; ++i) {
            auto index = surface.get_linear_index(i, y);
            if (ptr[index] == expectedColor.r && ptr[index + 1] == expectedColor.g && ptr[index + 2] == expectedColor.b) {
                ++count;
            }
        }
    } else {
        for (unsigned int i = 0; i < height; ++i) {
            auto index = surface.get_linear_index(x, i);
            if (ptr[index] == expectedColor.r && ptr[index + 1] == expectedColor.g && ptr[index + 2] == expectedColor.b) {
                ++count;
            }
        }
    }
    return count;
}

// Additional helper function to check for gaps or additional pixels
bool checkLineIntegrity(const Surface& surface, unsigned int x, unsigned int y, bool isHorizontal, const ColorU8_sRGB& expectedColor, int expectedPixelCount) {
    int pixelCount = countColoredPixels(surface, x, y, isHorizontal, expectedColor);
    return pixelCount == expectedPixelCount;
}

TEST_CASE("Horizontal and Vertical Line Test", "[lines]") {
    Surface surface(200, 200);
    surface.clear();
    ColorU8_sRGB white{255, 255, 255};

    SECTION("Horizontal Line") {
        Vec2f start{10.f, 10.f};
        Vec2f end{190.f, 10.f};
        draw_line_solid(surface, start, end, white);

        // Verify that the horizontal line is drawn correctly
        REQUIRE(checkLineIntegrity(surface, start.x, start.y, true, white, end.x - start.x + 1));
    }

    SECTION("Vertical Line") {
        Vec2f start{10.f, 10.f};
        Vec2f end{10.f, 190.f};
        draw_line_solid(surface, start, end, white);

        // Verify that the vertical line is drawn correctly
        REQUIRE(checkLineIntegrity(surface, start.x, start.y, false, white, end.y - start.y + 1));
    }
}
