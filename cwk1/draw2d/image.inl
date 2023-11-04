inline	// Inline specifier suggests that this function can be expanded at compile time where it is called to optimize performance.
ColorU8_sRGB_Alpha ImageRGBA::get_pixel( Index aX, Index aY ) const
{
	// Ensures that the provided coordinates are within the bounds of the image dimensions.
    assert( aX < mWidth && aY < mHeight );

	// Converts the 2D pixel coordinates (x, y) to a linear index in a 1D array representation of the image.
    Index linearIndex = get_linear_index(aX, aY);

	// Create a variable to store the color of the pixel (RGBA format).
    ColorU8_sRGB_Alpha pixelColor;

	// Access and assign the red component of the pixel from the data array.
    // This is at the position calculated by 4 * linear index (for RGBA images).
    pixelColor.r = mData[4 * linearIndex];

	// Access and assign the green component of the pixel, which is one byte after the red component.
    pixelColor.g = mData[4 * linearIndex + 1];

	// Access and assign the blue component of the pixel, which is two bytes after the red component.
    pixelColor.b = mData[4 * linearIndex + 2];

	// Access and assign the alpha (transparency) component of the pixel, which is three bytes after the red component.
    pixelColor.a = mData[4 * linearIndex + 3];

    return pixelColor;
	/*
		Returns the color of the pixel at the specified (x, y) position.
		The `const` qualifier at the end of the function declaration indicates that this method doesn't modify any member variables.
	*/
}

inline
auto ImageRGBA::get_width() const noexcept -> Index
{
	return mWidth;
}
inline

auto ImageRGBA::get_height() const noexcept -> Index
{
	return mHeight;
}

inline
std::uint8_t* ImageRGBA::get_image_ptr() noexcept
{
	return mData;
}
inline
std::uint8_t const* ImageRGBA::get_image_ptr() const noexcept
{
	return mData;
}

// get_linear_index computes the position of the pixel in the data array as if the 2D image was laid out in a single line.
inline
ImageRGBA::Index ImageRGBA::get_linear_index( Index aX, Index aY ) const noexcept
{
	/*
		Multiply the y index by the width of the image to get the starting index of the corresponding row,
		then add the x index to find the exact position of the pixel in the 1D array.
    */ 
    return aY * mWidth + aX;
}
