#include <catch2/catch_amalgamated.hpp>
#include "helpers.hpp" // Include this if helpers.hpp contains relevant utility functions or declarations
#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"
#include "../draw2d/color.hpp" // Include this if it contains relevant color definitions
/*
// Helper function to verify that edges of the triangle are correctly clipped
bool checkEdgeClipping(const Surface& surface, Vec2f p0, Vec2f p1, Vec2f p2) {
    // Check the four corners of the surface to ensure they are not colored
    // Top-left corner
    if (surface.get_pixel_color(0, 0) != ColorU8_sRGB{0, 0, 0}) return false;
    // Top-right corner
    if (surface.get_pixel_color(surface.get_width() - 1, 0) != ColorU8_sRGB{0, 0, 0}) return false;
    // Bottom-left corner
    if (surface.get_pixel_color(0, surface.get_height() - 1) != ColorU8_sRGB{0, 0, 0}) return false;
    // Bottom-right corner
    if (surface.get_pixel_color(surface.get_width() - 1, surface.get_height() - 1) != ColorU8_sRGB{0, 0, 0}) return false;

    // Optionally, more thorough checks can be added to verify the absence of colored pixels outside the triangle's bounding box

    return true;
}

// Test case to verify that triangle edges are clipped correctly
TEST_CASE("Edge Clipping Test", "[triangles][solid][interp]") {
    Surface surface(200, 200);
    surface.clear();
    ColorU8_sRGB fillColor{255, 0, 0}; // Red color for the solid triangle
    ColorF c0{1.f, 0.f, 0.f}, c1{0.f, 1.f, 0.f}, c2{0.f, 0.f, 1.f}; // RGB colors

    Vec2f p0{-50.f, -50.f}, p1{250.f, -50.f}, p2{100.f, 250.f};

    // Draw solid triangle
    draw_triangle_solid(surface, p0, p1, p2, fillColor);

    // Draw interpolated triangle
    draw_triangle_interp(surface, p0, p1, p2, c0, c1, c2);

    // Verify edge clipping
    REQUIRE(checkEdgeClipping(surface, p0, p1, p2));
}
*/