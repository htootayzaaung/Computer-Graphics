#include <catch2/catch_amalgamated.hpp>
#include "helpers.hpp"
#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"

/*
// Helper function to verify the continuity at a point
bool verifyContinuity(const Surface& surface, const Vec2f& point, const ColorU8_sRGB& color) {
    // Check the pixel at the point itself
    if (!pixelMatchesColor(surface, point, color)) {
        return false;
    }

    // Check the pixels immediately around the point for continuity
    std::vector<Vec2f> pointsToCheck = {
        {point.x - 1, point.y}, // left
        {point.x + 1, point.y}, // right
        {point.x, point.y - 1}, // top
        {point.x, point.y + 1}  // bottom
    };

    for (const auto& p : pointsToCheck) {
        if (!pixelMatchesColor(surface, p, color)) {
            return false;
        }
    }

    return true;
}

TEST_CASE("Continuous Line Drawing Test", "[lines]") {
    Surface surface(200, 200);
    surface.clear();
    ColorU8_sRGB white{255, 255, 255}; // Color for the lines

    Vec2f p0{10.f, 100.f};
    Vec2f p1{100.f, 100.f}; // Connection point
    Vec2f p2{200.f, 100.f};

    // Draw the lines
    draw_line_solid(surface, p0, p1, white);
    draw_line_solid(surface, p1, p2, white);

    // Verify continuity at connection point
    REQUIRE(verifyContinuity(surface, p1, white));
}
*/

TEST_CASE("Continuous Line Drawing Test", "[lines]") {
    Surface surface(200, 200);
    surface.clear();
    ColorU8_sRGB white{255, 255, 255}; // Color for the lines

    Vec2f p0{10.f, 100.f};
    Vec2f p1{100.f, 100.f}; // Connection point
    Vec2f p2{200.f, 100.f};

    // Draw the lines
    draw_line_solid(surface, p0, p1, white);
    draw_line_solid(surface, p1, p2, white);

    // Check the number of pixels that have 2 neighbors
    auto const counts = count_pixel_neighbours(surface);
    std::size_t expectedCount = static_cast<std::size_t>(p2.x) - static_cast<std::size_t>(p0.x) - 1;

    // Verify the number of pixels with exactly 2 neighbors (should match the expected count)
    REQUIRE(counts[2] == expectedCount);
}

