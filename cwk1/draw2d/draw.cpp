#include "draw.hpp"

#include <algorithm>

#include <cmath>

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
