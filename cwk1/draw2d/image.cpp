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
	Image blitting is the process of transferring a block of pixels from one image (or part of an image) to another, usually to a different 
	location in a graphics buffer.

	Alpha-masking involves using an alpha channel to determine the transparency level of each pixel. In alpha-masking, pixels with lower alpha 
	values (more transparent) can be ignored or blended with background colors, while pixels with higher alpha values (more opaque) are rendered 
	prominently.

	My implementation uses both of Image blitting and alpha-masking. It transfers pixels from one image to a surface (blitting), and it checks
	the alpha value of each pixel to determine whether it should be copied over (alpha-masking).

	My implementation of the blit iterates through each pixel of the source image. If a pixel's alpha value is above a certain threshold (128), 
	the pixel is considered visible and is therefore copied to a corresponding location on the target surface. The location is calculated by adding an offset 
	to the source image's pixel coordinates, converting them to the target surface's coordinate space.

	My current implementation iterates through every pixel in the source image and checks the alpha value. It is straightforward but could be inefficient if 
	the image has a large number of transparent pixels (alpha less than 128), as it spends time checking pixels that won't be drawn. It could be made more 
	efficient by, for example, preprocessing the image to identify and skip regions that are entirely transparent.

*/

// This function blits an image onto a surface with alpha masking.
void blit_masked( Surface& aSurface, ImageRGBA const& aImage, Vec2f aPosition )
{
	/*
		Here, "aSurface" is the destination where you want to blit "aImage" (which is the source image containing pixels with RGBA values). "aPosition" is 
		the position on "aSurface" where the top-left corner of "aImage" should be placed.
	*/

	// Iterate over all the pixels in the image by height (y-coordinate)
    for (ImageRGBA::Index y = 0; y < aImage.get_height(); ++y)
    {
		 // Iterate over all the pixels in the image by width (x-coordinate)
        for (ImageRGBA::Index x = 0; x < aImage.get_width(); ++x)
        {
			// Retrieve the pixel color and alpha value from the image at position (x, y)
            ColorU8_sRGB_Alpha pixel = aImage.get_pixel(x, y);
			
			/*
				If the pixel's alpha value is 128 or higher, it is visible and will be blitted to the surface. 
				If it's less than 128, it is considered too transparent and is not drawn.
            */
			if (pixel.a >= 128) 
            {
				/*
					"destX" and "destY" calculate the pixel's coordinates on "aSurface" where the current pixel from "aImage" should be placed. It adds the 
					"x" and "y" offsets (aPosition.x and aPosition.y, respectively) to the current pixel's coordinates to get the correct location on aSurface.
				*/

                int destX = static_cast<int>(aPosition.x) + x;
                int destY = static_cast<int>(aPosition.y) + y;

                // Ensures that the destination coordinates (destX and destY) are within the bounds of aSurface. If the coordinates are outside aSurface, the pixel is not drawn.
                if (destX >= 0 && destX < static_cast<int>(aSurface.get_width()) && destY >= 0 && destY < static_cast<int>(aSurface.get_height()))
                {
					// "convertedPixel" is created by dropping the alpha component from pixel, resulting in an RGB color that can be used on aSurface, which does not support alpha.
                    ColorU8_sRGB convertedPixel = { pixel.r, pixel.g, pixel.b };

					// Set the pixel on the surface at the destination coordinates with the RGB color of the original image pixel
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
