/*
inline
ColorU8_sRGB_Alpha ImageRGBA::get_pixel( Index aX, Index aY ) const
{
	assert( aX < mWidth && aY < mHeight ); // Leave this at the top of the function.

	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	(void)aX;  // Avoid warnings about unused arguments until the function is
	(void)aY;  // properly implemented.
	return { 0, 0, 0, 0 }; //TODO: remove this line when you implement this
}
*/

inline
ColorU8_sRGB_Alpha ImageRGBA::get_pixel( Index aX, Index aY ) const
{
    assert( aX < mWidth && aY < mHeight );

    Index linearIndex = get_linear_index(aX, aY);

    ColorU8_sRGB_Alpha pixelColor;
    pixelColor.r = mData[4 * linearIndex];
    pixelColor.g = mData[4 * linearIndex + 1];
    pixelColor.b = mData[4 * linearIndex + 2];
    pixelColor.a = mData[4 * linearIndex + 3];

    return pixelColor;
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

/*
inline
ImageRGBA::Index ImageRGBA::get_linear_index( Index aX, Index aY ) const noexcept
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	(void)aX;  // Avoid warnings about unused arguments until the function is
	(void)aY;  // properly implemented.
	return 0; //TODO: remove this line when you implement this function
}
*/

inline
ImageRGBA::Index ImageRGBA::get_linear_index( Index aX, Index aY ) const noexcept
{
    return aY * mWidth + aX;
}
