#include "draw.hpp"

#include <algorithm>

#include <cmath>

//I have included this!
#include "color.hpp"

#include "surface.hpp"

/*
    Clipping:
        Clipping in Computer Graphics refers to the process of confining a graphical object within a predefined boundary, known as the viewport. Only what's
        inside the boundary, known as the viewport. Only what's inside the boundary is visible and rendered, while everything outside is "clipped".

    Liang-Barsky Line Clipping:
        Concept: 
            The Liang-Barsky algorithm is an efficient method to determine which parts of a line segment are inside a rectangular view-port[1].

        Key Aspects:
            - Parametric Line Representation: 
                - Instead of representing a line as y = mx + c, it's represented in a parametric form: P(t) = P0 + t(P1 - P0).
                    - Where X(t) = x1 + t(x2 - x1) and Y(t) = y1 + t(y2 - y1)
                - Here P0 and P1 are the end-points of the line, and t varies between 0 and 1. 

                Understanding t:
                    - "t" is a parameter that represents the relative position along the segment from P0 to P1.
                    - When "t = 0", P(t) is at the starting point P0.
                    - When "t = 1", P(t) is at the end point P1.
                    - For values of "t" between 0 and 1, P(t) gives a point on the line segment between P0 and P1.
                    - In essence, you can think of 't' as representing the 'percentage' or fraction of the journey from P0 to P1. For instance, if "t = 0.5",
                    P(t) would be precisely halfway between P0 and P1.
            
            - Edge Checks: 
                The algorithm uses the above presentation to check intersections with each edge of the viewport. By comparing the t-values for each intersection, 
                it determines if the line is:
                    - Completely outside (no drawing needed)
                    - Completely inside (draw the entire line)
                    - Partially inside (draw the segment that lies inside the view-port).
            
            - Efficiency in Clipping:
                The algorithm effectively computes the values of "t" where the line segment enters and exits the viewport. If th computed range for "t" lies entirely
                outside [0, 1], the segment is outside the viewport. If the range lies within [0, 1], then the segment (or a part of it) is inside the viewport. This 
                approach makes the algorithm particularly efficient because it can quickly rule out line segments that don't interact with the viewpoint at all.


    Bresenham's Line Drawing:

        The Brsenhem's line algorithm is an efficient way to draw a line between two points. It determines which pixels in a raster grid should be plotted to
        form a close approximation to a straight line. The algorithm minimizes the use of floating-point calculations, making it faster and suitable for hardware
        implementations.


*/


void draw_line_solid(Surface& surface, Vec2f begin, Vec2f end, ColorU8_sRGB color) {
    
    /*
        The purpose of this function is to draw a solid line on the given 'surface' between points 'begin' and 'end' with the specified 'color'. The function uses 
        Liang-Barsky Line Clipping algorithm to determine the portion of the line that is within the view window. Post clipping, Bresenham's Line Drawing algorithm 
        is used to render the visible segment of the line.
    */

    // Liang-Barsky Line Clipping

    /*
        To understand Liang-Barsky, consider the line as a parametric equation. These arrays 'p' and 'q' will help 
        us determine which portion of the line lies within the clipping window.
    */

    // Variables for storing the starting and ending coordinates of the line.
    float x0 = begin.x, y0 = begin.y;                       // Starting coordinates (x0, y0) of the line.
    float x1 = end.x, y1 = end.y;                           // Ending coordinates (x1, y1) of the line.

    float p[4], q[4];                                       // These arrays will hold values needed for the clipping calculation [2].
    p[0] = -(x1 - x0);                                      // Delta X, change in x from start to end (going left). Left-edge check!
    p[1] = (x1 - x0) ;                                      // Delta X, but in the opposite direction (going right). Right-edge check!
    p[2] = -(y1 - y0);                                      // Delta Y, change in y from start to end (going upwards). Bottom-edge check!
    p[3] = (y1 - y0);                                       // Delta Y, but in the opposite direction (going downwards). Top-edge check!
    
    // The q array computes the distances from the start point of the line to the borders of the rectangle.
    q[0] = x0;                                              // Distance from starting x to the left border.                                              
    q[1] = surface.get_width() - x0;                        // Distance from starting x to the right border.                        
    q[2] = y0;                                              // Distance from starting y to the top border.    
    q[3] = surface.get_height() - y0;                       // Distance from starting y to the bottom border.

    // Initialize parameters u1 and u2 used for determining the visible portion of the line segment.
    float u1 = 0.0, u2 = 1.0;

    /*
        Possible cases in the Liang-Barsky Line Clipping Algorithm [2]:

        Case 1: When the line is parallel to a view window boundary (p[i] == 0):
            - If q[i] < 0, then the line is outside the view window and entirely invisible.
            - If q[i] > 0, then the line is inside the corresponding window boundary.

        Case 2: When p[i] < 0, it indicates the line is entering the view window as t increases.

        Case 3: If p[i] = 0 and q[i] < 0, the line is trivially invisible because it lies outside the view window.

        Case 4: If p[i] = 0 and q[i] > 0, the line is inside the corresponding window boundary.

    */
    
    // Loop through each of the 4 borders (left, right, top, bottom).
    for (int i = 0; i < 4; i++) 
    {
    // Case 1: When the line is parallel to a view window boundary.
        if (p[i] == 0) 
        {
            // Case 1A: If starting point lies outside the border, the line is entirely outside the view window.
            if (q[i] < 0) return;  
        } 
        else 
        {
            // Compute the value at which the line intersects the border [3].
            float t = q[i] / p[i];
        
            // Case 2: If the line is moving towards the inside of the rectangular area.
            if (p[i] < 0) 
            {
                // Update u1 only if t is greater. This represents the maximum t at which the line enters the view window.
                u1 = std::max(u1, t);                             
            } 
            // Case 3: If the line is moving towards the outside.
            else 
            {
                // Update u2 only if t is smaller. This represents the minimum t at which the line exits the view window.
                u2 = std::min(u2, t);                          
            }
        }
    }
    /*
        Case 4: This is a trivially visible and does not require any clipping action. In the given code implementation, the loop just proceeds to the next
        iteration for this case without taking any action.
    */ 
    

    /*
        - If u1 is greater than u2, it means the visible portion of the line segment is non-existent.
        - In this case, there's no portion of the line that lies within our clipping rectangle, which means that the line does not intersect the viewport.
    */ 

    if (u1 > u2) return;

    x0 = x0 + u1 * (x1 - x0);
    y0 = y0 + u1 * (y1 - y0);
    x1 = x0 + u2 * (x1 - x0);
    y1 = y0 + u2 * (y1 - y0);

    /*
        Ensuring that the clipped coordinates are within the surface dimensions

        For example:

        x0 = std::clamp(x0, 0.0f, static_cast<float>(surface.get_width() - 1)); ensures that x0 is kept between 0 and surface.get_width() - 1. If x0 was outside 
        this range, it would be set to the closest boundary value.
    */
  
    x0 = std::clamp(x0, 0.0f, static_cast<float>(surface.get_width() - 1));
    y0 = std::clamp(y0, 0.0f, static_cast<float>(surface.get_height() - 1));
    x1 = std::clamp(x1, 0.0f, static_cast<float>(surface.get_width() - 1));
    y1 = std::clamp(y1, 0.0f, static_cast<float>(surface.get_height() - 1));

    // Bresenham's Line Drawing

    /*
        Rasterization is the process of converting geometric shapes into a set of pixels. Since a pixel grid consists of discrete integer positions (you 
        can't have half a pixel), it's essential to convert float endpoints to integer values to decide precisely which pixels to illuminate on the grid.
    */

    // Convert the float endpoints to integers for rasterization in a pixel grid.
    int ix0 = static_cast<int>(x0);
    int iy0 = static_cast<int>(y0);
    int ix1 = static_cast<int>(x1);
    int iy1 = static_cast<int>(y1);

    /*
        The endpoints of the line are the pixels at (x0, y0) and (x1, y1), where the first coordinate of the pair is the column and the second is the row.
    */

    // Calculate the differences delta-x and delta-y between the start and end points.
    int dx = std::abs(ix1 - ix0);
    int dy = std::abs(iy1 - iy0);

    // Determine the direction to move in for x and y. This results in either 1 (positive direction) or -1 (negative direction).
    int sx = (ix0 < ix1) ? 1 : -1;
    int sy = (iy0 < iy1) ? 1 : -1;

    // Initial error term. This error term will help determine when to increment the y-coordinate

    /*
        Significance of "err -= dy" and " err += dx":
            In the Bresenham's Line Drawing Algorithm, the variable err represents the decision parameter that determines the next pixel to be plotted. 
            When err is modified by subtracting dy or adding dx, it's essentially deciding if the next pixel will be in the x direction, y direction, or both. 
            The conditional checks using the doubled error (e2) make these decisions. The -= and += adjustments help in maintaining the right slope and error 
            value as you move along the line.
    */
    int err = dx - dy;

    // A safety counter to avoid infinite loops in case of any anomalies.
    int safetyCounter = 0; 

    // We assume the maximum steps to be roughly twice the diagonal of the surface.
    int maxSteps = std::max(surface.get_width(), surface.get_height()) * 2;

    // Begin the Bresenham's loop.
    while (safetyCounter < maxSteps) 
    {
        // Ensure the pixel is within the valid range of the surface dimensions before plotting.
        if (ix0 >= 0 && ix0 < static_cast<int>(surface.get_width()) && iy0 >= 0 && iy0 < static_cast<int>(surface.get_height())) 
        {
            // This function call sets the color of the pixel at (ix0, iy0) position on the surface.
            surface.set_pixel_srgb(ix0, iy0, color);
        }

        // If we've reached the destination pixel, we stop the algorithm.
        if (ix0 == ix1 && iy0 == iy1) break;

        /*
            In Bresenham's algorithm, determining whether to "step" or move in the x or y direction (or both) is crucial for drawing a line that 
            closely represents the ideal line between two points.
        */

        // Calculate twice the current error. This helps determine whether to step in x or y direction (or both).
        int e2 = 2 * err;

        /*
            Calculating twice the error and using it for decisions helps to avoid the need for floating-point arithmetic. Bresenham's algorithm is 
            designed to work with integers to make it more efficient, especially in the contexts where using floating-point arithmetic would be costly, 
            such as older hardware or specific graphics hardware.
        */
        
        // If e2 is greater than negative dy, then we'll move in the x direction.
        if (e2 > -dy) 
        {
            err -= dy;      // Adjust the error term for the next iteration.
            ix0 += sx;      // Move one pixel in the x direction (left or right). 
            
            //If "sx = 1", that means it moves right, and if "sx = -1" it means to move left.
        }

        // If e2 is less than dx, then we'll move in the y direction.
        if (e2 < dx) 
        {
            err += dx;      // Adjust the error term for the next iteration
            iy0 += sy;      // Move one pixel in the y direction (up or down).

            //If "sy = 1", that means it moves up, and if "sx = -1" it means to move down.
        }
        // The safety counter ensures that in the unlikely event of an algorithm hiccup, we don't end up in an infinite loop to prevent from crashing.
        safetyCounter++;    
    }
}


/*
    Questions:
        - Does the function produces correct results with all inputs, including cases where the line extends off-screen?
            Yes, the function performs Liang-Barsky line clipping which should handle cases where the line extends off-screen. 
            The clipped segment is then rendered.

        - Does the drawing method scale with O(N) with respect to the number of drawn pixels (N)?
            The function scales approximately with O(N) with respect to the number of drawn pixels (N), where N is the number of 
            pixels in the longest dimension of the surface or twice the diagonal.

            The Bresenham's algorithm inherently scales with O(N), where N is the number of pixels drawn. If it didn't scale this way, you might 
            find it takes significantly longer to draw longer lines than shorter ones, even if the number of pixels in the line only increases 
            modestly. This would make it unsuitable for real-time graphics.

        - Does it makes any dynamic allocations or system calls in the line drawing method?
            No dynamic allocations or system calls are made within the line drawing method. The function operates in a consistent 
            time based on the input length of the line and the dimensions of the surface.
        
    Note: Document the handling of lines extending off-screen.
*/


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


void draw_triangle_solid(Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorU8_sRGB aColor) {
    
    /*
        This function aims to draw a solid
        Calculating the bounding box of the triangle:
            - Bounding box is the smallest triangle that entirely encloses the triangle.
            - The purpose of calculating it is to minimize the number of pixels we need to check. 
            - Instead of checking every pixel on the screen, we'll only consider those within a box.

        Barycentric coordinates are used for blending three colors over a traingular region evenly in computer 
        graphics.

        Relevance of calculating the bounding box:

        Calculating the bounding box of the triangle is an optimization step. Instead of checking every pixel on 
        the drawing surface to see if it lies inside the triangle, you only check the pixels within the bounding 
        box. This significantly reduces the number of checks you need to perform and thus speeds up the drawing process.
    */

    // Find the left-most x-coordinate of the triangle, but ensure it's not less than 0.
    float minX = std::max(0.0f, std::min({aP0.x, aP1.x, aP2.x}));

    // Find the top-most y-coordinate of the triangle, but ensure it's not less than 0.
    float minY = std::max(0.0f, std::min({aP0.y, aP1.y, aP2.y}));

    // Find the right-most x-coordinate of the triangle, but ensure it's not outside the surface's width.
    float maxX = std::min(static_cast<float>(aSurface.get_width() - 1), std::max({aP0.x, aP1.x, aP2.x}));

    // Find the bottom-most y-coordinate of the triangle, but ensure it's not outside the surface's height.
    float maxY = std::min(static_cast<float>(aSurface.get_height() - 1), std::max({aP0.y, aP1.y, aP2.y}));

    /*
        Precompute values for the barycentric coordinates

        - Barycentric coordinates are a method to express any point inside a triangle as a combination of its vertices.
        - We'll use these coordinates to determine if a point (pixel) is inside the triangle.

        - Please refer to the report for the mathematical representation and the conditions that needs to satisfy
        Barycentric coordinates.

        The reason for using the barycentric coordinates approach in this context is due to its simplicity, efficiency, and 
        versatility. For a 2D drawing function like this, the barycentric method provides an elegant solution. It's easy to 
        compute, easy to understand, and it offers an efficient way to check if a point is inside a triangle. 
    */ 
    float denom = (aP1.y - aP2.y) * (aP0.x - aP2.x) + (aP2.x - aP1.x) * (aP0.y - aP2.y);

    /*
        Loop through all the pixels in the bounding box.

        += 1.0f ensures there are only integer increments. While x and y are floating point values, incrementing by 1.0f ensures
        that you move pixel by pixel.
    */
    for (float y = minY; y <= maxY; y += 1.0f) {
        for (float x = minX; x <= maxX; x += 1.0f) {
            // Compute barycentric coordinates for current pixel (x,y).
            // If all three values are between 0 and 1, then the point lies inside the triangle.
            float w0 = ((aP1.y - aP2.y) * (x - aP2.x) + (aP2.x - aP1.x) * (y - aP2.y)) / denom;
            float w1 = ((aP2.y - aP0.y) * (x - aP2.x) + (aP0.x - aP2.x) * (y - aP2.y)) / denom;
            
            // w2 is calculated from w0 and w1 since the sum of all three coordinates is always 1.
            float w2 = 1.0f - w0 - w1;

            // Check if the point (pixel) is inside the triangle.
            if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                // If the point is inside the triangle, set its color to the triangle's color.
                aSurface.set_pixel_srgb(static_cast<Surface::Index>(x), static_cast<Surface::Index>(y), aColor);
                /*
                    The static_cast<Surface::Index>(x) and static_cast<Surface::Index>(y) are casting the float values of x and y 
                    to an index type defined by the Surface class. Since pixel indices are usually integers (you can't have half a 
                    pixel or 0.5th pixel), this casting ensures the float coordinates are converted to appropriate integer indices.
                */
            }
        }
    }
}

void draw_triangle_interp(Surface& aSurface, Vec2f aP0, Vec2f aP1, Vec2f aP2, ColorF aC0, ColorF aC1, ColorF aC2)
{
    /* 
        The majority of the code is the same as the above method!
    */
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
                // Interpolate colors using barycentric coordinates

                /*
                    This is where the function differs from the solid triangle drawing. Instead of setting all pixels inside the triangle 
                    to a single color, here each pixel gets a unique color that's an interpolation of the triangle's vertex colors.

                    The interpolation uses the barycentric coordinates (w0, w1, w2) as weights. The calculated weights determine how much of 
                    each vertex's color is mixed into the final color for the pixel.

                    ColorF interpolatedColor = aC0 * w0 + aC1 * w1 + aC2 * w2;

                    This equation calculates the interpolated color. Each vertex color (aC0, aC1, aC2) is multiplied by its respective weight 
                    and then summed up to give the color of the pixel [7].

                    Think of each vertex color aC0, aC1, aC2 as a position in a 3D RGB space.

                    For any point P inside the triangle, its color will be some blend of these three colors. The weights w0, w1, w2 determine how 
                    much of each color contributes to point P.

                    The weighted average takes each vertex's color and multiplies it by its associated weight, then adds these results together to 
                    get the interpolated color at point P. This is a standard way to do color interpolation across the surface of a triangle in 
                    computer graphics, ensuring that the resulting color changes smoothly from one vertex to the other, creating a gradient effect 
                    across the triangle.

                    Additionally, empirical evidence in the form of visual results confirms its correctness. When you visualize the interpolated colors 
                    in a triangle, you should see a smooth gradient that intuitively makes sense based on the vertex colors.
                    
                    Main Difference: The key difference between this function and the one from Section 1.4 is the method of coloring the triangle. 
                    In 1.4, the triangle was filled with a solid color, whereas in 1.5, the colors are interpolated across the triangle based on 
                    the vertex colors and their barycentric coordinates.

                    The reason for using barycentric interpolation in this manner is to smoothly blend the colors across the triangle, which is 
                    crucial for various graphical applications like texture mapping, shading, etc. This gives a gradient effect based on the colors 
                    of the vertices, rather than a uniform color fill.
                */

                //In the color.hpp, I have created the multiplication and the addition operator!
                ColorF interpolatedColor = aC0 * w0 + aC1 * w1 + aC2 * w2;
                aSurface.set_pixel_srgb(static_cast<Surface::Index>(x), static_cast<Surface::Index>(y), linear_to_srgb(interpolatedColor));
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

/*
void draw_rectangle_solid( Surface& aSurface, Vec2f aMinCorner, Vec2f aMaxCorner, ColorU8_sRGB aColor )
{
    for (int y = static_cast<int>(aMinCorner.y); y <= static_cast<int>(aMaxCorner.y); ++y)
    {
        for (int x = static_cast<int>(aMinCorner.x); x <= static_cast<int>(aMaxCorner.x); ++x)
        {
            aSurface.set_pixel_srgb(x, y, aColor);
        }
    }
}

void draw_rectangle_outline( Surface& aSurface, Vec2f aMinCorner, Vec2f aMaxCorner, ColorU8_sRGB aColor )
{
    // Top and bottom lines
    for (int x = static_cast<int>(aMinCorner.x); x <= static_cast<int>(aMaxCorner.x); ++x)
    {
        aSurface.set_pixel_srgb(x, static_cast<int>(aMinCorner.y), aColor);
        aSurface.set_pixel_srgb(x, static_cast<int>(aMaxCorner.y), aColor);
    }

    // Left and right lines
    for (int y = static_cast<int>(aMinCorner.y); y <= static_cast<int>(aMaxCorner.y); ++y)
    {
        aSurface.set_pixel_srgb(static_cast<int>(aMinCorner.x), y, aColor);
        aSurface.set_pixel_srgb(static_cast<int>(aMaxCorner.x), y, aColor);
    }
}
*/

