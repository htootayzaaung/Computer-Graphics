#include "helpers.hpp"

#include "../draw2d/color.hpp"
#include "../draw2d/surface.hpp"


ColorU8_sRGB find_most_red_pixel( Surface const& aSurface )
{
	ColorU8_sRGB ret{ 0, 0, 0 };

	auto const stride = aSurface.get_width() << 2;
	for( std::uint32_t x = 0; x < aSurface.get_width(); ++x )
	{
		for( std::uint32_t y = 0; y < aSurface.get_height(); ++y )
		{
			auto const idx = y*stride + (x<<2);
			auto const ptr = aSurface.get_surface_ptr() + idx;

			// Not really needed.
			//if( 0 == ptr[0] && 0 == ptr[1] && 0 == ptr[2] )
			//	continue;

			if( ptr[0] >= ret.r )
			{
				ret.r = ptr[0];
				ret.g = ptr[1];
				ret.b = ptr[2];
			}
		}
	}

	return ret;
}
ColorU8_sRGB find_least_red_nonzero_pixel( Surface const& aSurface )
{
	ColorU8_sRGB ret{ 255, 255, 255 };

	auto const stride = aSurface.get_width() << 2;
	for( std::uint32_t x = 0; x < aSurface.get_width(); ++x )
	{
		for( std::uint32_t y = 0; y < aSurface.get_height(); ++y )
		{
			auto const idx = y*stride + (x<<2);
			auto const ptr = aSurface.get_surface_ptr() + idx;

			if( 0 == ptr[0] && 0 == ptr[1] && 0 == ptr[2] )
				continue;

			if( ptr[0] < ret.r )
			{
				ret.r = ptr[0];
				ret.g = ptr[1];
				ret.b = ptr[2];
			}
		}
	}

	return ret;

}

// Helper function to determine if a point is inside a triangle using barycentric coordinates.
bool is_point_inside_triangle(Vec2f p, Vec2f a, Vec2f b, Vec2f c) {
    float alpha = ((b.y - c.y)*(p.x - c.x) + (c.x - b.x)*(p.y - c.y)) /
                  ((b.y - c.y)*(a.x - c.x) + (c.x - b.x)*(a.y - c.y));
    float beta = ((c.y - a.y)*(p.x - c.x) + (a.x - c.x)*(p.y - c.y)) /
                 ((b.y - c.y)*(a.x - c.x) + (c.x - b.x)*(a.y - c.y));
    float gamma = 1.0f - alpha - beta;

    return alpha >= 0 && beta >= 0 && gamma >= 0;
}

// Function to compute barycentric coordinates for a point within a triangle
BarycentricCoordinates compute_barycentric_coordinates(Vec2f p, Vec2f a, Vec2f b, Vec2f c) {
    Vec2f v0 = {b.x - a.x, b.y - a.y};
    Vec2f v1 = {c.x - a.x, c.y - a.y};
    Vec2f v2 = {p.x - a.x, p.y - a.y};

    float d00 = v0.x * v0.x + v0.y * v0.y;
    float d01 = v0.x * v1.x + v0.y * v1.y;
    float d11 = v1.x * v1.x + v1.y * v1.y;
    float d20 = v2.x * v0.x + v2.y * v0.y;
    float d21 = v2.x * v1.x + v2.y * v1.y;
    float denom = d00 * d11 - d01 * d01;

    BarycentricCoordinates bary;
    bary.w1 = (d11 * d20 - d01 * d21) / denom;
    bary.w2 = (d00 * d21 - d01 * d20) / denom;
    bary.w3 = 1.0f - bary.w1 - bary.w2;

    return bary;
}