#include <catch2/catch_amalgamated.hpp>
#include "helpers.hpp"
#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"

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
