#include "draw.hpp"

#include <algorithm>

#include <cmath>

//I have included this!
#include "color.hpp"

#include "surface.hpp"

/*
void draw_line_solid( Surface& aSurface, Vec2f aBegin, Vec2f aEnd, ColorU8_sRGB aColor )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	//TODO: remove the following when you start your implementation
	(void)aSurface; // Avoid warnings about unused arguments until the function
	(void)aBegin;   // is properly implemented.
	(void)aEnd;
	(void)aColor;
}
*/

void draw_line_solid( Surface& surface, Vec2f begin, Vec2f end, ColorU8_sRGB color ) {
    // Step 1: Liang-Barsky Line Clipping
    float x0 = begin.x, y0 = begin.y;
    float x1 = end.x, y1 = end.y;

    float p[4], q[4];
    p[0] = -(x1 - x0);
    p[1] = -p[0];
    p[2] = -(y1 - y0);
    p[3] = -p[2];
    
    q[0] = x0;
    q[1] = surface.get_width() - x0;
    q[2] = y0;
    q[3] = surface.get_height() - y0;

    float u1 = 0.0, u2 = 1.0;
    for (int i = 0; i < 4; i++) {
        if (p[i] == 0) {
            if (q[i] < 0) return;
        } else {
            float t = q[i] / p[i];
            if (p[i] < 0) u1 = std::max(u1, t);
            else u2 = std::min(u2, t);
        }
    }
    if (u1 > u2) return;

    x0 = x0 + u1 * (x1 - x0);
    y0 = y0 + u1 * (y1 - y0);
    x1 = x0 + u2 * (x1 - x0);
    y1 = y0 + u2 * (y1 - y0);

    // Ensuring that the clipped coordinates are within the surface dimensions
    x0 = std::max(0.0f, std::min(x0, static_cast<float>(surface.get_width() - 1)));
    y0 = std::max(0.0f, std::min(y0, static_cast<float>(surface.get_height() - 1)));
    x1 = std::max(0.0f, std::min(x1, static_cast<float>(surface.get_width() - 1)));
    y1 = std::max(0.0f, std::min(y1, static_cast<float>(surface.get_height() - 1)));

    // Step 2: Bresenham's Line Drawing
    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        surface.set_pixel_srgb((int)x0, (int)y0, color);
        if ((x0 == x1) && (y0 == y1)) break;
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}


void draw_triangle_wireframe( Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorU8_sRGB aColor )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	//TODO: remove the following when you start your implementation
	(void)aSurface; // Avoid warnings about unused arguments until the function
	(void)aP0;   // is properly implemented.
	(void)aP1;
	(void)aP2;
	(void)aColor;
}

/*
void draw_triangle_solid( Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorU8_sRGB aColor )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	//TODO: remove the following when you start your implementation
	(void)aSurface; // Avoid warnings about unused arguments until the function
	(void)aP0;   // is properly implemented.
	(void)aP1;
	(void)aP2;
	(void)aColor;
}
*/

void draw_triangle_solid(Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorU8_sRGB aColor) {
    // Calculate bounding box of the triangle
    float minX = std::max(0.0f, std::min({aP0.x, aP1.x, aP2.x}));
    float minY = std::max(0.0f, std::min({aP0.y, aP1.y, aP2.y}));

    float maxX = std::min(static_cast<float>(aSurface.get_width() - 1), std::max({aP0.x, aP1.x, aP2.x}));
    float maxY = std::min(static_cast<float>(aSurface.get_height() - 1), std::max({aP0.y, aP1.y, aP2.y}));

    // Precompute values for the barycentric coordinates
    float denom = (aP1.y - aP2.y) * (aP0.x - aP2.x) + (aP2.x - aP1.x) * (aP0.y - aP2.y);

    for (float y = minY; y <= maxY; y += 1.0f) {
        for (float x = minX; x <= maxX; x += 1.0f) {
            float w0 = ((aP1.y - aP2.y) * (x - aP2.x) + (aP2.x - aP1.x) * (y - aP2.y)) / denom;
            float w1 = ((aP2.y - aP0.y) * (x - aP2.x) + (aP0.x - aP2.x) * (y - aP2.y)) / denom;
            float w2 = 1.0f - w0 - w1;

            if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                // Inside the triangle, set the pixel color
                aSurface.set_pixel_srgb(static_cast<Surface::Index>(x), static_cast<Surface::Index>(y), aColor);
            }
        }
    }
}

/*
void draw_triangle_interp( Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorF aC0, ColorF aC1, ColorF aC2 )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here

	//TODO: Remove the following when you start your implementation!
	//TODO: This draws a solid triangle until you implement the function
	//TODO: properly. (This is such that you can see the asteroids with solid
	//TODO: shading until that point.)
	draw_triangle_solid( aSurface, aP0, aP1, aP2, linear_to_srgb( aC0 ) );
	(void)aC1;
	(void)aC2;
}
*/

void draw_triangle_interp(Surface& surface, Vec2f v0, Vec2f v1, Vec2f v2, ColorF c0, ColorF c1, ColorF c2)
{
    // Compute triangle bounding box
    float minX = std::min({v0.x, v1.x, v2.x});
    float minY = std::min({v0.y, v1.y, v2.y});
    float maxX = std::max({v0.x, v1.x, v2.x});
    float maxY = std::max({v0.y, v1.y, v2.y});

    // Clip against screen bounds
    minX = std::max(minX, 0.0f);
    minY = std::max(minY, 0.0f);
    maxX = std::min(maxX, (float)surface.width - 1);
    maxY = std::min(maxY, (float)surface.height - 1);

    // Rasterize
    for (float y = minY; y <= maxY; y += 1.0f)
    {
        for (float x = minX; x <= maxX; x += 1.0f)
        {
            // Using brace initialization for Vec2f
            Vec2f p = {x, y};
            
            // Compute barycentric coordinates
            float w0 = edgeFunction(v1, v2, p);
            float w1 = edgeFunction(v2, v0, p);
            float w2 = edgeFunction(v0, v1, p);
            
            if (w0 >= 0 && w1 >= 0 && w2 >= 0)
            {
                // Interpolate vertex colors
                float sum = w0 + w1 + w2;
                w0 /= sum;
                w1 /= sum;
                w2 /= sum;
                ColorF interpolatedColor = {
                    w0 * c0.r + w1 * c1.r + w2 * c2.r,
                    w0 * c0.g + w1 * c1.g + w2 * c2.g,
                    w0 * c0.b + w1 * c1.b + w2 * c2.b,
                };

                // Convert to 8-bit color
                Color8Bit intColor8Bit = {
                    static_cast<uint8_t>(255 * interpolatedColor.r),
                    static_cast<uint8_t>(255 * interpolatedColor.g),
                    static_cast<uint8_t>(255 * interpolatedColor.b)
                };

                // Drawing pixel
                // Please ensure you have the correct function to draw pixel in the Surface class.
                // Here, I'm using a hypothetical function as I don't have the Surface class details.
                surface.drawPixel(p, intColor8Bit);
            }
        }
    }
}







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
