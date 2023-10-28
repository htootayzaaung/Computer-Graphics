/* Inline functions need to have a definition that is visible to the compiler
 * whenever the function is used. THey could be define in the header that
 * declares the function. However, to keep the definitions and declarations
 * somewhat apart, it is a common practice to move them to an "inline" file
 * such as this one (*.inl extension). This file is then #include:ed at the end
 * of the header, to ensure that whoever includes the header also automatically
 * includes the inline file.
 *
 * Inlining allows us to avoid any overheads related to call (when building in
 * "release mode" / with optimizations enabled). This makes functions like
 * set_pixel_srgb() zero overhead abstractions.
 *
 */

inline 
auto Surface::get_width() const noexcept -> Index
{
	return mWidth;
}

inline
auto Surface::get_height() const noexcept -> Index
{
	return mHeight;
}

/*
    Every pixel on a screen or image is usually represented in a two-dimensional fashion, meaning
    they have an x-coordinate(horizontal position) and a y-coordinate(vertical position). However,
    in computer memory, images are typically stored as a one-dimensional array. This means we need
    a way to convert the 2D coordinates (x, y) of a pixel into a 1D postion or "index" in that array.
*/

inline
Surface::Index Surface::get_linear_index( Index aX, Index aY ) const noexcept
{
    /*
        Parameters:
            - aX: This is the x-coordinate (horizontal position of the pixel).
            - aY: This is the y-coordinate (vertical position of the pixel).

        Other variables:
            - mWidth: This is a member-variable of "Surface" class that likely to represent the width of 
                      the image.

        Return value: The function returns the corresponding 1D index for the given 2D (x, y) coordinates. 
    */

    // Ensure that the aX and aY coordinates provided are within the valid range.
    assert(aX < mWidth && aY < mHeight); 

    /*
        Convert the 2D coordinates (x, y) to a 1D index:

        Imagine you have a grid (or 2D array) of width "mWidth":
    
        [0][0] [0][1] [0][2] ... [0][mWidth - 1]
        [1][0] [1][1] [1][2] ...
        [2][0] [2][1] [2][2] ...
        ...

        If you want to flatten this into a 1D array, it would look something like: 

        [0][0], [0][1], [0][2], ... [0][mWidth - 1], [1][0], [1][1], [1][2], ... [1][mWidth - 1], [2][0], [2][1], [2][2], ... [2][mWidth - 1], ...

        1.) You first count how many complete rows (from the top) there are until you get to the last row that would be "aY" rows.
            Since each row has "mWidth" columns, you've passed by "aY * mWidth" elements to get to the start of the row.

        2.) Then, within your row, you move to the "aX" column. Combining the steps: 
            the 1D index = "aY * mWidth + aX"

        3.) Since each pixel is represented by 4 values (RGBx), you multiply the result by 4 to get the exact position in the 1D array that represents
            the start of the pixel's data.
    */
    return (aY * mWidth + aX) * 4;	
}

/*
    Given that images in memory are often stored as on-dimensional array, we need to first locate the pixels in that array. Once found, we can set its
    Red, Green, and Blue components to the desired values.
*/

inline
void Surface::set_pixel_srgb(Index aX, Index aY, ColorU8_sRGB const& aColor)
{   
    /*
        Parameters:
            - aX: This is the x-coordinate (horizontal position of the pixel).
            - aY: This is the y-coordinate (vertical position of the pixel).
            - aColor: This is a struct or datatype that holds the Red, Green, and Blue Components of a color.


        Other variables:
            - mWidth: This is a member-variable of "Surface" class that likely to represent the width of 
                      the image.

    */

    // Ensure that the aX and aY coordinates provided are within the valid range.
    assert(aX < mWidth && aY < mHeight); 

    /*
        First, we need to find out where our pixel is located in the one-dimensional "mSurface" array.
        We do this by converting the 2D coordinates (aX, aY) into a 1D index.
    */
    Index index = get_linear_index(aX, aY);

    /*
        Once we have our index, it's time to set the color values:
        
        - The position denoted by "index" corresponds to the Red component of the pixel.
        - The next position "index + 1" is for the Green component.
        - The position after that "index + 2" is for the Blue component.
        - As for the fourth component "index + 3", it's a padding byte. In the RGBx format, this value 
          is ignored, and hence, there's no need to set it (though we could set it to 0 or any value if required).
    */
    mSurface[index + 0] = aColor.r;  //Set red
    mSurface[index + 1] = aColor.g;  //Set green
    mSurface[index + 2] = aColor.b;  //Set Blue

    // Note: No need to set the fourth byte as it's for padding in the RGBx format.
}




