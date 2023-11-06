
#include <catch2/catch_amalgamated.hpp>
#include "helpers.hpp" // Include this if helpers.hpp contains relevant utility functions or declarations
#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"
#include "../draw2d/color.hpp" // Include this if it contains relevant color definitions
/*
// Helper function to check color interpolation at the centroid of the triangle
bool checkInterpolation(const Surface& surface, Vec2f p0, Vec2f p1, Vec2f p2, ColorF c0, ColorF c1, ColorF c2) {
    // Calculate the centroid of the triangle
    Vec2f centroid = (p0 + p1 + p2) / 3.0f;

    // Calculate the expected interpolated color at the centroid
    ColorF expectedColor = (c0 + c1 + c2) / 3.0f;

    // Convert the expected color from floating point to sRGB
    ColorU8_sRGB expectedColorSRGB = linear_to_srgb(expectedColor);

    // Get the actual color at the centroid from the surface
    ColorU8_sRGB actualColor = surface.get_pixel_color(static_cast<int>(centroid.x), static_cast<int>(centroid.y));

    // Check if the actual color matches the expected color within a small threshold to account for rounding errors
    const float threshold = 0.01f; // Define a suitable threshold value
    bool redMatch = std::abs(actualColor.r - expectedColorSRGB.r) <= threshold;
    bool greenMatch = std::abs(actualColor.g - expectedColorSRGB.g) <= threshold;
    bool blueMatch = std::abs(actualColor.b - expectedColorSRGB.b) <= threshold;

    // If the colors match within the threshold, the interpolation is correct
    return redMatch && greenMatch && blueMatch;
}

// Test case to verify color interpolation accuracy
TEST_CASE("Color Interpolation Accuracy Test", "[triangles][interp]") {
    Surface surface(200, 200);
    surface.clear();

    Vec2f p0{50.f, 50.f}, p1{150.f, 50.f}, p2{100.f, 150.f};
    ColorF c0{1.f, 0.f, 0.f}, c1{0.f, 1.f, 0.f}, c2{0.f, 0.f, 1.f}; // RGB colors

    // Draw the triangle with interpolated colors
    draw_triangle_interp(surface, p0, p1, p2, c0, c1, c2);

    // Verify color interpolation along centroid lines
    REQUIRE(checkInterpolation(surface, p0, p1, p2, c0, c1, c2));
}
*/