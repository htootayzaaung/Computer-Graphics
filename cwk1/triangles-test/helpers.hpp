#ifndef HELPERS_HPP_DD37133A_D9CE_4998_AA48_41DA09E1517C
#define HELPERS_HPP_DD37133A_D9CE_4998_AA48_41DA09E1517C

#include "../draw2d/forward.hpp"
#include "../draw2d/../vmlib/vec2.hpp" // Include Vec2f definition from the correct location.

struct BarycentricCoordinates {
    float w1, w2, w3;
};

bool is_point_inside_triangle(Vec2f p, Vec2f a, Vec2f b, Vec2f c);
BarycentricCoordinates compute_barycentric_coordinates(Vec2f p, Vec2f a, Vec2f b, Vec2f c);

ColorU8_sRGB find_most_red_pixel( Surface const& );
ColorU8_sRGB find_least_red_nonzero_pixel( Surface const& );

#endif // HELPERS_HPP_DD37133A_D9CE_4998_AA48_41DA09E1517C
