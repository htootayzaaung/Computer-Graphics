#include <catch2/catch_amalgamated.hpp>

#include "helpers.hpp"
#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"

TEST_CASE("Interpolation Across Triangle", "[triangle][interp]") {
    // Initialize a graphics surface.
    Surface surface(320, 240);
    surface.clear();

    // Define the triangle vertices and their corresponding colors.
    Vec2f v1 = {50.f, 50.f};
    Vec2f v2 = {270.f, 50.f};
    Vec2f v3 = {160.f, 190.f};
    ColorF c1 = {1.0f, 0.0f, 0.0f}; // Red
    ColorF c2 = {0.0f, 1.0f, 0.0f}; // Green
    ColorF c3 = {0.0f, 0.0f, 1.0f}; // Blue

    // Draw the triangle with interpolated colors.
    draw_triangle_interp(surface, v1, v2, v3, c1, c2, c3);

    // Check the colors at the vertices and at several points within the triangle.
    // We have to directly access the surface buffer for this since we're not adding new methods.
    SECTION("Color Interpolation") {
        // We'll check the color at the center of the triangle to see if the interpolation is correct.
        Vec2f center = (v1 + v2 + v3) / 3.0f;
        BarycentricCoordinates bary = compute_barycentric_coordinates(center, v1, v2, v3);
        ColorF expectedColor = c1 * bary.w1 + c2 * bary.w2 + c3 * bary.w3;
        ColorU8_sRGB expectedColorSrgb = linear_to_srgb(expectedColor);

        // Since we don't have a get_pixel_color method, we'll access the buffer directly.
        auto buffer = surface.get_surface_ptr();
        auto stride = surface.get_width() * 4;
        auto index = static_cast<std::size_t>(center.y) * stride + static_cast<std::size_t>(center.x) * 4;

        ColorU8_sRGB centerColor = {
            buffer[index],    // Red
            buffer[index+1],  // Green
            buffer[index+2]   // Blue
        };

        // Check if the color at the center is close to what we expect.
        REQUIRE(centerColor.r == Catch::Approx(expectedColorSrgb.r).margin(1));
        REQUIRE(centerColor.g == Catch::Approx(expectedColorSrgb.g).margin(1));
        REQUIRE(centerColor.b == Catch::Approx(expectedColorSrgb.b).margin(1));
    }
}
