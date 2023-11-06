#include <catch2/catch_amalgamated.hpp>
#include "helpers.hpp"
#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"

/*
    This method, carries out the verification process. It iterates over each pixel in the horizontal span of two lines and checks the
    following conditions.

    - The pixel on the line should match the expected color.
    - The pixel immediately above and below the line should not match the line's color, ensuring there's no vertical overlap with adjacent lines.

    If these conditions hold true for both lines across their entire length, the test passes, indicating that the line-drawing algorithm maintains 
    precision as expected. If any of these checks fail, the test will reveal a potential flaw in the algorithm's implementation.
*/

// Function to check if two horizontal lines are drawn correctly without overlapping.
bool checkLinesPrecision(const Surface& surface, Vec2f start1, Vec2f end1, Vec2f start2, Vec2f end2, const ColorU8_sRGB& color) {
    // Check first line for expected color and that it's clear above and below.
    for (int x = static_cast<int>(start1.x); x <= static_cast<int>(end1.x); ++x) {
        if (!pixelMatchesColor(surface, {static_cast<float>(x), start1.y}, color) ||
            pixelMatchesColor(surface, {static_cast<float>(x), start1.y - 1}, color) ||
            pixelMatchesColor(surface, {static_cast<float>(x), start1.y + 1}, color)) {
            return false;
        }
    }
    // Check second line for expected color and that it's clear above and below.
    for (int x = static_cast<int>(start2.x); x <= static_cast<int>(end2.x); ++x) {
        if (!pixelMatchesColor(surface, {static_cast<float>(x), start2.y}, color) ||
            pixelMatchesColor(surface, {static_cast<float>(x), start2.y - 1}, color) ||
            pixelMatchesColor(surface, {static_cast<float>(x), start2.y + 1}, color)) {
            return false;
        }
    }
    return true;
}

/*
    The "Line Drawing Precision Test" is an essential test designed to ensure the accuracy of a line-drawing algorithm when rendering 
    closely positioned parallel lines. The test specifically checks that two horizontal lines, placed one pixel apart in the vertical 
    plane, are drawn with precision—that is, without any overlap or unintended gaps.

    Purpose of the Test:
        - Precision: The test validates that the line-drawing algorithm can handle closely spaced lines without causing any overlap, 
        which could occur due to rounding errors or incorrect pixel calculation.

        - Spacing: It ensures that the algorithm respects the one-pixel vertical spacing between the two lines, a condition that's 
        important for drawing fine details and avoiding visual artifacts.

        - Continuity: By drawing the lines from start1 to end1 and start2 to end2, the test ensures that each line is continuous over 
        its expected span.

        - Boundary Checking: The lines are verified to make sure they do not bleed into adjacent rows, which would indicate an error in 
        the line-drawing algorithm's incremental step or error adjustment process.

    Reasons for including the Test:
        - Algorithm Robustness: This test challenges the algorithm with a scenario that requires high precision, which is indicative of 
        the overall robustness and reliability of the line-drawing routine.

        - Graphics Quality: In graphical applications, especially those that require rendering of fine details like fonts or grid lines, 
        the precision of line drawing is critical to the quality of the output.

        - Error Handling: The test acts as a safeguard against common errors in rasterization algorithms, such as those that might occur 
        in Bresenham's line algorithm when dealing with special cases of slope and orientation.
*/

TEST_CASE("Line Drawing Precision Test", "[lines][!mayfail]") {
    Surface surface(200, 200);
    surface.clear();
    ColorU8_sRGB white{255, 255, 255}; // White color for the lines

    Vec2f start1{50.f, 50.f};
    Vec2f end1{150.f, 50.f};
    Vec2f start2{50.f, 51.f};
    Vec2f end2{150.f, 51.f};
    
    // Draw two horizontal lines, one pixel apart
    draw_line_solid(surface, start1, end1, white);
    draw_line_solid(surface, start2, end2, white);

    // Verify that the lines are drawn precisely without overlap
    REQUIRE(checkLinesPrecision(surface, start1, end1, start2, end2, white));
}
