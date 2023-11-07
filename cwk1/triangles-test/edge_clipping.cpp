#include <catch2/catch_amalgamated.hpp>
#include "helpers.hpp"
#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"
#include <iostream>

TEST_CASE("Edge Clipping", "[triangle][clipping]") {
    std::cout << "Test Start: Edge Clipping" << std::endl;

    try {
        Surface surface(320, 240);
        std::cout << "Surface created with size 320x240" << std::endl;

        surface.clear();
        std::cout << "Surface cleared" << std::endl;

        ColorU8_sRGB testColor = {255, 0, 0}; // Red color for visibility.
        std::cout << "Test color defined" << std::endl;

        Vec2f v1 = {160.f, 120.f}; // Center of the surface.
        Vec2f v2 = {330.f, -10.f}; // Outside the top-right boundary.
        Vec2f v3 = {330.f, 250.f}; // Outside the bottom-right boundary.
        std::cout << "Triangle vertices defined" << std::endl;

        draw_triangle_solid(surface, v1, v2, v3, testColor);
        std::cout << "Triangle drawn" << std::endl;

        bool colorMismatchFound = false;
        for (Surface::Index x = 0; x < surface.get_width(); ++x) {
            for (Surface::Index y = 0; y < surface.get_height(); ++y) {
                Vec2f point = {static_cast<float>(x), static_cast<float>(y)};
                if (is_point_inside_triangle(point, v1, v2, v3)) {
                    const std::uint8_t* pixel = surface.get_surface_ptr() + (y * surface.get_width() + x) * 4;
                    ColorU8_sRGB pixelColor = {pixel[0], pixel[1], pixel[2]};
                    if (pixelColor != testColor) {
                        colorMismatchFound = true;
                        std::cout << "Color mismatch found at pixel: " << x << ", " << y << std::endl;
                        break;
                    }
                }
            }
            if (colorMismatchFound) {
                break;
            }
        }

        std::cout << "Checked for color mismatch within triangle" << std::endl;
        REQUIRE_FALSE(colorMismatchFound);
    } catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown exception caught. Possible segmentation fault or signal received." << std::endl;
    }

    std::cout << "Test End: Edge Clipping" << std::endl;
}
