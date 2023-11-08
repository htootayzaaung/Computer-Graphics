#include "spaceship.hpp"

#include <cstdio>

#include "../draw2d/shape.hpp"

/* Instructions - CUSTOM SPACESHIP DESIGNS
 *
 *  0. If you are OK with your space ship design being included in future
 *     computer graphics exercises and/or courseworks, please indicate this by
 *     placing a 'X' in the space between the [ ] on the following line:
 *     [ ] I give permission for my space ship shape to be used in future
 *         computer graphics exercises and/or courseworks.
 *
 *     If you do not wish for your shape to be used, leave the line as is.
 *     Your decision will not affect the marking of Task 2.7 in CW1.
 *
 *     If you wish to be credited (name + year when you took the computer
 *     graphics module) in a CREDITS.txt or similar file with future exercises
 *     and/or courseworks, please indicate this by ticking the following box
 *     [X] I wish to have my name + year listed with assignments that use my
 *         space ship shape.
 *
 *     If you do not wish to be credited, leave the [ ] empty.
 *
 *     Many thanks if you do permit the use of your cool space ship designs! It
 *     will help us to make future computer graphics exercises and courseworks
 *     more interesting.
 *
 *  1. Change the line
 *     #	define SPACESHIP SPACESHIP_DEFAULT
 *     to
 *     #	define SPACESHIP SPACESHIP_CUSTOM
 *
 *  2. Define your custom space ship design in the now active code section.
 *     (Look for the line that says '// TODO: YOUR DESIGN GOES HERE')
 */

#define SPACESHIP_DEFAULT 1
#define SPACESHIP_CUSTOM 2

// Ensure SPACESHIP is defined. Uncomment the one you need.
// #define SPACESHIP SPACESHIP_DEFAULT
#define SPACESHIP SPACESHIP_CUSTOM

LineStrip make_spaceship_shape()
{
#if SPACESHIP == SPACESHIP_CUSTOM
    static constexpr Vec2f customPoints[] = {
        // Custom spaceship design goes here
        { 0.0f, 0.0f }, // Nose of the spaceship
        { -20.0f, 40.0f }, // Start of the wing curve on one side
        { -100.0f, 100.0f }, // Tip of the wing on one side
        { -80.0f, 80.0f }, // Inner wing curve back towards the body
        { -50.0f, 30.0f }, // Body to rear wing edge
        { -120.0f, 0.0f }, // End of the wing on one side
        { -50.0f, -30.0f }, // Body to rear wing edge on the other side
        { -80.0f, -80.0f }, // Inner wing curve on the other side
        { -100.0f, -100.0f }, // Tip of the wing on the other side
        { -20.0f, -40.0f }, // Start of the wing curve back to the nose
        { 0.0f, 0.0f } // Back to the nose to close the shape
    };
    return LineStrip(customPoints);
#elif SPACESHIP == SPACESHIP_DEFAULT
    static constexpr float xs[] = { 250.f, 200.f, 150.f, 100.f, 000.f, 040.f, -50.f, -140.f, -170.f };
    static constexpr float ys[] = { 190.f, 180.f, 70.f, 50.f, 30.f, 20.f };
    static constexpr Vec2f defaultPoints[] = {
        { 0.2f * xs[0], 0.2f * +ys[5] },
        { 0.2f * xs[1], 0.2f * +ys[3] },
        { 0.2f * xs[2], 0.2f * +ys[3] },
        { 0.2f * xs[3], 0.2f * +ys[4] },
        { 0.2f * xs[4], 0.2f * +ys[4] },
        { 0.2f * xs[4], 0.2f * +ys[2] },
        { 0.2f * xs[5], 0.2f * +ys[1] },
        { 0.2f * xs[6], 0.2f * +ys[0] },
        { 0.2f * xs[8], 0.2f * +ys[2] },
        { 0.2f * xs[7], 0.2f * +ys[3] },
        { 0.2f * xs[7], 0.2f * -ys[3] },
        { 0.2f * xs[8], 0.2f * -ys[2] },
        { 0.2f * xs[6], 0.2f * -ys[0] },
        { 0.2f * xs[5], 0.2f * -ys[1] },
        { 0.2f * xs[4], 0.2f * -ys[2] },
        { 0.2f * xs[4], 0.2f * -ys[4] },
        { 0.2f * xs[3], 0.2f * -ys[4] },
        { 0.2f * xs[2], 0.2f * -ys[3] },
        { 0.2f * xs[1], 0.2f * -ys[3] },
        { 0.2f * xs[0], 0.2f * -ys[5] },
        { 0.2f * xs[0], 0.2f * +ys[5] }
    };
    return LineStrip(defaultPoints);
#endif
}


