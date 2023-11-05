#ifndef HELPERS_HPP_D7CD1385_1BA6_4B6B_9F31_72B9B8A05AA7
#define HELPERS_HPP_D7CD1385_1BA6_4B6B_9F31_72B9B8A05AA7

#include <array>

#include <cstddef>

#include "../draw2d/forward.hpp"

// I have imported these header files!
#include "../draw2d/surface.hpp"
#include "../draw2d/draw.hpp"

std::size_t max_row_pixel_count( Surface const& );
std::size_t max_col_pixel_count( Surface const& );

std::array<std::size_t,9> count_pixel_neighbours( Surface const& );

// I have implemented this header file!
bool pixelMatchesColor(const Surface& surface, const Vec2f& point, const ColorU8_sRGB& expectedColor);

#endif // HELPERS_HPP_D7CD1385_1BA6_4B6B_9F31_72B9B8A05AA7
