#include "draw.hpp"

#include <algorithm>

#include <cmath>

#include "color.hpp"
#include "surface.hpp"

/*
void draw_rectangle_solid( Surface& aSurface, Vec2f aMinCorner, Vec2f aMaxCorner, ColorU8_sRGB aColor )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	//TODO: remove the following when you start your implementation
	(void)aSurface; // Avoid warnings about unused arguments until the function
	(void)aMinCorner;   // is properly implemented.
	(void)aMaxCorner;
	(void)aColor;
}

void draw_rectangle_outline( Surface& aSurface, Vec2f aMinCorner, Vec2f aMaxCorner, ColorU8_sRGB aColor )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	//TODO: remove the following when you start your implementation
	(void)aSurface; // Avoid warnings about unused arguments
	(void)aMinCorner;
	(void)aMaxCorner;
	(void)aColor;
}
*/


void draw_rectangle_solid(Surface& aSurface, Vec2f aMinCorner, Vec2f aMaxCorner, ColorU8_sRGB aColor)
{
    float minX = std::min(aMinCorner.x, aMaxCorner.x);
    float maxX = std::max(aMinCorner.x, aMaxCorner.x);
    float minY = std::min(aMinCorner.y, aMaxCorner.y);
    float maxY = std::max(aMinCorner.y, aMaxCorner.y);

    for (float y = minY; y < maxY; ++y)
    {
        for (float x = minX; x < maxX; ++x)
        {
            aSurface.set_pixel_srgb(static_cast<Surface::Index>(x),
                                    static_cast<Surface::Index>(y), aColor);
        }
    }
}

void draw_rectangle_outline(Surface& aSurface, Vec2f aMinCorner, Vec2f aMaxCorner, ColorU8_sRGB aColor)
{
    float minX = std::min(aMinCorner.x, aMaxCorner.x);
    float maxX = std::max(aMinCorner.x, aMaxCorner.x);
    float minY = std::min(aMinCorner.y, aMaxCorner.y);
    float maxY = std::max(aMinCorner.y, aMaxCorner.y);

    for (float x = minX; x < maxX; ++x)
    {
        aSurface.set_pixel_srgb(static_cast<Surface::Index>(x),
                                static_cast<Surface::Index>(minY), aColor);
        aSurface.set_pixel_srgb(static_cast<Surface::Index>(x),
                                static_cast<Surface::Index>(maxY), aColor);
    }
    for (float y = minY; y < maxY; ++y)
    {
        aSurface.set_pixel_srgb(static_cast<Surface::Index>(minX),
                                static_cast<Surface::Index>(y), aColor);
        aSurface.set_pixel_srgb(static_cast<Surface::Index>(maxX),
                                static_cast<Surface::Index>(y), aColor);
    }
}