#include "image.hpp"

#include <memory>
#include <algorithm>

#include <cstdio>
#include <cstring>
#include <cassert>

#include <stb_image.h>

#include "surface.hpp"

#include "../support/error.hpp"

namespace
{
	struct STBImageRGBA_ : public ImageRGBA
	{
		STBImageRGBA_( Index, Index, std::uint8_t* );
		virtual ~STBImageRGBA_();
	};
}

ImageRGBA::ImageRGBA()
	: mWidth( 0 )
	, mHeight( 0 )
	, mData( nullptr )
{}

ImageRGBA::~ImageRGBA() = default;


std::unique_ptr<ImageRGBA> load_image( char const* aPath )
{
	assert( aPath );

	stbi_set_flip_vertically_on_load( true );

	int w, h, channels;
	stbi_uc* ptr = stbi_load( aPath, &w, &h, &channels, 4 );
	if( !ptr )
		throw Error( "Unable to load image \"%s\"", aPath );

	return std::make_unique<STBImageRGBA_>(
		ImageRGBA::Index(w),
		ImageRGBA::Index(h),
		ptr
	);
}

/*
void blit_masked( Surface& aSurface, ImageRGBA const& aImage, Vec2f aPosition )
{
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	//TODO: your implementation goes here
	(void)aSurface;  // Avoid warnings about unused arguments until the
	(void)aImage;    // function is properly implemented.
	(void)aPosition;
}
*/

void blit_masked( Surface& aSurface, ImageRGBA const& aImage, Vec2f aPosition )
{
    for (ImageRGBA::Index y = 0; y < aImage.get_height(); ++y)
    {
        for (ImageRGBA::Index x = 0; x < aImage.get_width(); ++x)
        {
            ColorU8_sRGB_Alpha pixel = aImage.get_pixel(x, y);

            if (pixel.a >= 128) // Check the alpha value.
            {
                // Convert aPosition (float) to integer coordinates.
                int destX = static_cast<int>(aPosition.x) + x;
                int destY = static_cast<int>(aPosition.y) + y;

                // Ensure we are within the surface boundaries.
                if (destX >= 0 && destX < static_cast<int>(aSurface.get_width()) && destY >= 0 && destY < static_cast<int>(aSurface.get_height()))
                {
                    ColorU8_sRGB convertedPixel = { pixel.r, pixel.g, pixel.b };
                    aSurface.set_pixel_srgb(destX, destY, convertedPixel);
                }
            }
        }
    }
}



namespace
{
	STBImageRGBA_::STBImageRGBA_( Index aWidth, Index aHeight, std::uint8_t* aPtr )
	{
		mWidth = aWidth;
		mHeight = aHeight;
		mData = aPtr;
	}

	STBImageRGBA_::~STBImageRGBA_()
	{
		if( mData )
			stbi_image_free( mData );
	}
}
