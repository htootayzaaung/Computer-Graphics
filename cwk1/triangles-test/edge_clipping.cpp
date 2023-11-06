#include <catch2/catch_amalgamated.hpp>

#include "helpers.hpp"
#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"

TEST_CASE("Edge Clipping - Triangle Clipping", "[triangle][clip]") {
    Surface surface(320, 240);
    surface.clear();

    ColorU8_sRGB triangleColor = {255, 0, 0}; // Red for visibility.
    ColorU8_sRGB clearColor = {0, 0, 0}; // Assuming clear color is black for the surface.

    // Triangle Partially Outside - Left Edge
    SECTION("Triangle Partially Outside - Left Edge") {
        Vec2f v1 = {-10.f, 10.f};
        Vec2f v2 = {160.f, 10.f};
        Vec2f v3 = {75.f, 220.f};

        draw_triangle_solid(surface, v1, v2, v3, triangleColor);

        // Check only the first column for the expected clear color.
        for (Surface::Index y = 0; y < surface.get_height(); ++y) {
            auto index = y * surface.get_width() * 4; // Index for the first pixel of each row
            ColorU8_sRGB pixelColor = {
                surface.get_surface_ptr()[index],    // Red
                surface.get_surface_ptr()[index+1],  // Green
                surface.get_surface_ptr()[index+2]   // Blue
            };
            REQUIRE(pixelColor == clearColor);
        }
    }

    // Triangle Partially Outside - Right Edge
    SECTION("Triangle Partially Outside - Right Edge") {
        Vec2f v1 = {320.f, 10.f};
        Vec2f v2 = {640.f, 100.f}; // well beyond the right edge
        Vec2f v3 = {320.f, 230.f};

        draw_triangle_solid(surface, v1, v2, v3, triangleColor);

        // Check only the last column for the expected clear color.
        for (Surface::Index y = 0; y < surface.get_height(); ++y) {
            auto index = y * surface.get_width() * 4 + (surface.get_width() - 1) * 4; // Index for the last pixel of each row
            ColorU8_sRGB pixelColor = {
                surface.get_surface_ptr()[index],    // Red
                surface.get_surface_ptr()[index+1],  // Green
                surface.get_surface_ptr()[index+2]   // Blue
            };
            REQUIRE(pixelColor == clearColor);
        }
    }

    // Triangle Partially Outside - Top Edge
    SECTION("Triangle Partially Outside - Top Edge") {
        Vec2f v1 = {10.f, -10.f};
        Vec2f v2 = {300.f, -10.f};
        Vec2f v3 = {160.f, 210.f};

        draw_triangle_solid(surface, v1, v2, v3, triangleColor);

        // Check only the first row for the expected clear color.
        for (Surface::Index x = 0; x < surface.get_width(); ++x) {
            auto index = x * 4; // Index for the first row
            ColorU8_sRGB pixelColor = {
                surface.get_surface_ptr()[index],    // Red
                surface.get_surface_ptr()[index+1],  // Green
                surface.get_surface_ptr()[index+2]   // Blue
            };
            REQUIRE(pixelColor == clearColor);
        }
    }

    // Triangle Partially Outside - Bottom Edge
    SECTION("Triangle Partially Outside - Bottom Edge") {
        Vec2f v1 = {10.f, 240.f};
        Vec2f v2 = {300.f, 240.f}; // just beyond the bottom edge
        Vec2f v3 = {160.f, 10.f};

        draw_triangle_solid(surface, v1, v2, v3, triangleColor);

        // Check only the last row for the expected clear color.
        for (Surface::Index x = 0; x < surface.get_width(); ++x) {
            auto index = (surface.get_height() - 1) * surface.get_width() * 4 + x * 4; // Index for the last row
            ColorU8_sRGB pixelColor = {
                surface.get_surface_ptr()[index],    // Red
                surface.get_surface_ptr()[index+1],  // Green
                surface.get_surface_ptr()[index+2]   // Blue
            };
            REQUIRE(pixelColor == clearColor);
        }
    }

    // Add additional tests for the triangle completely outside the boundaries
    // if required
