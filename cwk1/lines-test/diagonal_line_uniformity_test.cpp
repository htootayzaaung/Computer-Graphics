#include <catch2/catch_amalgamated.hpp>

#include "helpers.hpp"
#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"
/*
    Purpose of the Test:

        - Uniformity Check: This test ensures that when a line is drawn diagonally across the surface, every expected pixel along the line's 
        theoretical path is indeed colored. This means that there should be no gaps (missing colored pixels) and no extra pixels (pixels outside 
        the line's path that are incorrectly colored).

        - Algorithm Precision and Accuracy: Diagonal lines can be particularly challenging due to the diagonal not aligning perfectly with the pixel 
        grid. This test confirms that the line drawing algorithm handles this scenario correctly, ensuring that the line remains straight and doesn't deviate.

    Reasons for Inclusion:

        - In graphical applications, the precision of diagonal lines directly affects the visual quality of the rendered image. A uniform 
        diagonal line is crucial for the perceived sharpness and clarity of the graphics.
*/

TEST_CASE("Diagonal Line Uniformity Test", "[lines]") {
    Surface surface(200, 200);
    surface.clear();
    ColorU8_sRGB white{255, 255, 255}; // Color for the lines

    Vec2f start{0.f, 0.f};
    Vec2f end{199.f, 199.f};

    // Draw the line
    draw_line_solid(surface, start, end, white);

    /*
        After drawing the line, the test iterates through each pixel along the expected diagonal path, checking to ensure each pixel 
        matches the expected color using the pixelMatchesColor function. If any pixel does not match, the variable uniform is 
        set to false, and the iteration breaks early to avoid unnecessary checks.
    */

    // Verify that the line is uniformly drawn across the diagonal
    bool uniform = true;
    for (float i = 0; i <= 199; ++i) {
        Vec2f point{i, i}; // Since it's a diagonal, x and y are the same
        if (!pixelMatchesColor(surface, point, white)) {
            uniform = false;
            break;
        }
    }

    // The line should be continuous and uniform across the entire diagonal
    REQUIRE(uniform);

    /*
        The assertion REQUIRE(uniform); ensures that if the variable uniform is false (meaning the line is not uniform), the test will fail, 
        indicating a problem with the line drawing algorithm
    */
}
