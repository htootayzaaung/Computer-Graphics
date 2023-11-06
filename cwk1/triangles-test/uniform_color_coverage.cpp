#include <catch2/catch_amalgamated.hpp>
#include "helpers.hpp"
#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"

// Helper function to determine if a point is inside a triangle using barycentric coordinates.
bool is_point_inside_triangle(Vec2f p, Vec2f a, Vec2f b, Vec2f c) {
    float alpha = ((b.y - c.y)*(p.x - c.x) + (c.x - b.x)*(p.y - c.y)) /
                  ((b.y - c.y)*(a.x - c.x) + (c.x - b.x)*(a.y - c.y));
    float beta = ((c.y - a.y)*(p.x - c.x) + (a.x - c.x)*(p.y - c.y)) /
                 ((b.y - c.y)*(a.x - c.x) + (c.x - b.x)*(a.y - c.y));
    float gamma = 1.0f - alpha - beta;

    return alpha >= 0 && beta >= 0 && gamma >= 0;
}

TEST_CASE("Uniform Color Coverage", "[triangle][solid]") {
    Surface surface(320, 240);
    surface.clear();

    // Define the color to be used for the test.
    ColorU8_sRGB testColor = {255, 0, 0}; // Red color for visibility.

    // Define the vertices of the triangle.
    Vec2f v1 = {10.f, 10.f};
    Vec2f v2 = {310.f, 10.f};
    Vec2f v3 = {160.f, 230.f};

    // Draw the triangle.
    draw_triangle_solid(surface, v1, v2, v3, testColor);

    bool colorMismatchFound = false; // Flag to indicate a color mismatch.

    // Check every pixel within the bounding box of the triangle.
    for (Surface::Index x = 0; x < surface.get_width(); ++x) {
        for (Surface::Index y = 0; y < surface.get_height(); ++y) {
            Vec2f point = {static_cast<float>(x), static_cast<float>(y)};
            if (is_point_inside_triangle(point, v1, v2, v3)) {
                const std::uint8_t* pixel = surface.get_surface_ptr() + (y * surface.get_width() + x) * 4;
                ColorU8_sRGB pixelColor = {pixel[0], pixel[1], pixel[2]};
                
                // If the color does not match, set the flag and break out of the loop.
                if (pixelColor.r != testColor.r ||
                    pixelColor.g != testColor.g ||
                    pixelColor.b != testColor.b) {
                    colorMismatchFound = true;
                    break;
                }
            }
        }
        if (colorMismatchFound) {
            break; // Break out of the outer loop as well if a mismatch is found.
        }
    }

    // Assert that no color mismatch was found.
    REQUIRE_FALSE(colorMismatchFound);
}
