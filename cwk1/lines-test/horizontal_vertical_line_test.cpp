#include <catch2/catch_amalgamated.hpp>
#include "helpers.hpp"
#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"
#include "../draw2d/color.hpp"

/*
    The purpose of this test is to ensure the correctness and integrity of the line drawing functionality provided by the draw_line_solid function in 
    draw.cpp Here's a breakdown of the reasoning behind this test and its importance:

    1.) Horizontal Line Test:
        This section ensures that the algorithm can draw a straight horizontal line correctly. The test draws a line using the draw_line_solid function 
        and then verifies the integrity of this line by counting the colored pixels along the x-axis. It expects the number of colored pixels to equal 
        the length of the line (which is the difference between the x-coordinates of the end and start points, plus one to account for the inclusive 
        nature of pixel coordinates).

    2.) Verical Line Test:
        This test performs a similar check for a straight vertical line. It validates that the algorithm can correctly draw a line along the y-axis 
        without any breaks or irregularities. The integrity check here counts the colored pixels along the y-axis, expecting a count equal to the vertical 
        length of the line.

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

/*
    Helper function to count colored pixels in a row or column

    - Counts the number of pixels of a specified color along a horizontal or vertical line.
    - It iterates over each pixel in the specified row (if isHorizontal is true) or column (if isHorizontal is false), checking if it matches the 
    expected color.

    Parameters:
        - `surface`: The Surface object on which the line is drawn.
        - `x`: The x-coordinate of the starting point of the line (used for vertical lines).
        - `y`: The y-coordinate of the starting point of the line (used for horizontal lines).
        - `isHorizontal`: A flag indicating whether the line is horizontal (true) or vertical (false).
        - `expectedColor`: The color that the line is expected to be.

    Returns:
        - The count of pixels matching the expected color along the specified line.
*/

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


/*
    Additional helper function to check for gaps or additional pixels

    - Validates that the number of colored pixels along a specified line matches the expected count.
    - This serves as an integrity check to ensure there are no missing or extra pixels in the line, which could indicate gaps or errors in the line 
    drawing algorithm.

    Returns:
        - True if the actual number of colored pixels matches the expected count, indicating correct line integrity.
        - False otherwise.
*/
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
