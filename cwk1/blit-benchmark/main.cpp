#include <benchmark/benchmark.h>
#include <algorithm>
#include <cassert>
#include <cstring>
#include "../draw2d/image.hpp"
#include "../draw2d/surface.hpp"

// Forward declaration of the blit_masked function.
void blit_masked(Surface& aSurface, ImageRGBA const& aImage, Vec2f aPosition);

// The function blits an image to the surface without alpha masking using loops.
void blit_no_alpha_loops_(benchmark::State& aState, const std::string& image_path) 
{
    // Get the dimensions of the framebuffer from benchmark state.
    auto const fb_width = std::uint32_t(aState.range(0));
    auto const fb_height = std::uint32_t(aState.range(1));

    // Create a surface with the specified dimensions and clear it.
    Surface surface(fb_width, fb_height);
    surface.clear();

    // Load the source image from the provided image path.
    auto source = load_image(image_path.c_str());

    // Ensure that the image was loaded successfully.
    assert(source);

    // Iterate over the benchmark state to measure performance.
    for (auto _ : aState) 
    {
        // Loop over each pixel in the source image and copy it to the surface.
        for (std::uint32_t y = 0; y < std::min(fb_height, source->get_height()); ++y) 
        {
            for (std::uint32_t x = 0; x < std::min(fb_width, source->get_width()); ++x) 
            {
                ColorU8_sRGB_Alpha pixel = source->get_pixel(x, y);
                surface.set_pixel_srgb(x, y, { pixel.r, pixel.g, pixel.b });
            }
        }
        // Ensure that the benchmarked code does not get optimized away
        benchmark::ClobberMemory();
    }
    // Calculate and set the bytes processed by the benchmark.

    /*
        To determine the number of bytes processed during the benchmark, we calculate the product of the following factors:

        1. We multiply by 2 because we're considering both the source image's RGB and Alpha channels.

        2. We calculate the minimum width between the framebuffer and the source image to ensure we don't process outside the visible area.

        3. Similarly, we calculate the minimum height between the framebuffer and the source image to limit processing to the visible area.

        4. We multiply by 4 because each pixel consists of 4 bytes (RGBA): Red, Green, Blue, and Alpha.

        5. Finally, we scale this byte count by the number of benchmark iterations to reflect the total bytes processed over all iterations.
    */
    aState.SetBytesProcessed(2 * std::min(fb_width, source->get_width()) * std::min(fb_height, source->get_height()) * 4 * aState.iterations());
}

// The function blits an image to the surface without alpha masking using std::memcpy for each line.
void blit_no_alpha_memcpy_(benchmark::State& aState, const std::string& image_path) 
{
    // Extract the width and height of the framebuffer from the benchmark state.
    auto const fb_width = std::uint32_t(aState.range(0));
    auto const fb_height = std::uint32_t(aState.range(1));

    // Create a Surface object with the specified framebuffer dimensions and clear it.
    Surface surface(fb_width, fb_height);
    surface.clear();

    // Load the source image from the provided image path.
    auto source = load_image(image_path.c_str());

    // Ensure that the image was loaded successfully.
    assert(source);

    // Determine the width and height for blitting, limited by framebuffer and source image dimensions.
    auto const blit_width = std::min(fb_width, source->get_width());
    auto const blit_height = std::min(fb_height, source->get_height());

    // Perform blitting for the specified number of iterations.
    for (auto _ : aState) 
    {
        // Get pointers to the surface data and image data.
        auto* surface_data = const_cast<std::uint8_t*>(surface.get_surface_ptr());
        auto const* image_data = source->get_image_ptr();

        // Calculate the size of each line in bytes.
        auto const line_size = blit_width * sizeof(ColorU8_sRGB);

        // Iterate through each line and copy it from the source image to the surface using std::memcpy, one for each line in
        for (std::uint32_t y = 0; y < blit_height; ++y) 
        {
            // Calculate the pointer to the destination line in the framebuffer.
            auto* dest_line = surface_data + y * fb_width * sizeof(ColorU8_sRGB);

            // Calculate the pointer to the source line in the image.
            const auto* src_line = image_data + y * source->get_width() * sizeof(ColorU8_sRGB);

            // Use std::memcpy to copy the entire line from source to destination.
            std::memcpy(dest_line, src_line, line_size);
        }
        // Ensure that the benchmarking framework does not optimize away the benchmarked code.
        benchmark::ClobberMemory();
    }

    // Calculate and set the total bytes processed by the benchmark.
    // This accounts for both the source and destination data, iterations, and the size of each element.
    aState.SetBytesProcessed(static_cast<int64_t>(blit_width) * blit_height * 4 * aState.iterations());
}

// The function blits an image to the surface with alpha masking using blit_masked function.
void benchmark_blit_masked(benchmark::State& aState, const std::string& image_path) 
{
    // Get the framebuffer width and height from the benchmark state
    auto const fb_width = std::uint32_t(aState.range(0));
    auto const fb_height = std::uint32_t(aState.range(1));

    // Create a surface with the specified width and height and clear it.
    Surface surface(fb_width, fb_height);
    surface.clear();

    // Load the source image from the given image path.
    auto source = load_image(image_path.c_str());

    // Ensure that the source image was successfully loaded.
    assert(source);

    // Run the benchmark for the specified number of iterations.
    for (auto _ : aState) 
    {
        // Call the blit_masked function to copy the source image onto the surface with alpha masking.
        // The image is placed at the top-left corner of the surface (position {0, 0}).
        blit_masked(surface, *source, {0.f, 0.f});

        // Ensure that the benchmarking framework does not optimize away the benchmarked code.
        benchmark::ClobberMemory();
    }
 
    // Calculate and set the total bytes processed by the benchmark.
    // This accounts for both the source and destination data, iterations, and the size of each element.
    aState.SetBytesProcessed(static_cast<int64_t>(fb_width) * fb_height * 4 * aState.iterations());
}

// Register the benchmark functions
BENCHMARK_CAPTURE(benchmark_blit_masked, impostor, "assets/impostor.png")
    ->Args({320, 240})
    ->Args({1280, 720})
    ->Args({1920, 1080})
    ->Args({7680, 4320});

BENCHMARK_CAPTURE(benchmark_blit_masked, mortal_kombat, "assets/mortal_kombat.png")
    ->Args({320, 240})
    ->Args({1280, 720})
    ->Args({1920, 1080})
    ->Args({7680, 4320});

/*
BENCHMARK_CAPTURE(benchmark_blit_masked, earth, "assets/earth.png")
    ->Args({320, 240})
    ->Args({1280, 720})
    ->Args({1920, 1080})
    ->Args({7680, 4320});
*/

BENCHMARK_CAPTURE(blit_no_alpha_loops_, impostor, "assets/impostor.png")
    ->Args({320, 240})
    ->Args({1280, 720})
    ->Args({1920, 1080})
    ->Args({7680, 4320});

BENCHMARK_CAPTURE(blit_no_alpha_loops_, mortal_kombat, "assets/mortal_kombat.png")
    ->Args({320, 240})
    ->Args({1280, 720})
    ->Args({1920, 1080})
    ->Args({7680, 4320});

/*
BENCHMARK_CAPTURE(blit_no_alpha_loops_, mortal_kombat, "assets/earth.png")
    ->Args({320, 240})
    ->Args({1280, 720})
    ->Args({1920, 1080})
    ->Args({7680, 4320});
*/

BENCHMARK_CAPTURE(blit_no_alpha_memcpy_, impostor, "assets/impostor.png")
    ->Args({320, 240})
    ->Args({1280, 720})
    ->Args({1920, 1080})
    ->Args({7680, 4320});

BENCHMARK_CAPTURE(blit_no_alpha_memcpy_, mortal_kombat, "assets/mortal_kombat.png")
    ->Args({320, 240})
    ->Args({1280, 720})
    ->Args({1920, 1080})
    ->Args({7680, 4320});

/*
BENCHMARK_CAPTURE(blit_no_alpha_memcpy_, mortal_kombat, "assets/earth.png")
    ->Args({320, 240})
    ->Args({1280, 720})
    ->Args({1920, 1080})
    ->Args({7680, 4320});
*/
BENCHMARK_MAIN();
