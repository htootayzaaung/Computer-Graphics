#include <benchmark/benchmark.h>
#include <algorithm>
#include <cassert>
#include "../draw2d/image.hpp"
#include "../draw2d/surface.hpp"

namespace {
    // Blit function without alpha masking using loops
    void blit_no_alpha_loops_(benchmark::State& aState) {
        auto const width = std::uint32_t(aState.range(0));
        auto const height = std::uint32_t(aState.range(1));

        Surface surface(width, height);
        surface.clear();

        auto source = load_image("assets/earth.png");
        assert(source);

        for (auto _ : aState) {
            // Blit implementation without alpha masking
            for (std::uint32_t y = 0; y < std::min(height, source->get_height()); ++y) {
                for (std::uint32_t x = 0; x < std::min(width, source->get_width()); ++x) {
                    ColorU8_sRGB_Alpha pixel = source->get_pixel(x, y);
                    // Since there's no alpha masking, we don't check the alpha value of the pixel
                    ColorU8_sRGB convertedPixel = { pixel.r, pixel.g, pixel.b };
                    int destX = static_cast<int>(x);
                    int destY = static_cast<int>(y);
                    // Make sure we don't write outside of the surface bounds
                    if (destX < static_cast<int>(width) && destY < static_cast<int>(height)) {
                        surface.set_pixel_srgb(destX, destY, convertedPixel);
                    }
                }
            }
            benchmark::ClobberMemory(); // Prevent optimization
        }

        auto const maxBlitX = std::min(width, source->get_width());
        auto const maxBlitY = std::min(height, source->get_height());

        aState.SetBytesProcessed(2 * maxBlitX * maxBlitY * 4 * aState.iterations());
    }

    // Existing benchmark function for blitting with alpha masking
    void default_blit_earth_(benchmark::State& aState) {
        auto const width = std::uint32_t(aState.range(0));
        auto const height = std::uint32_t(aState.range(1));

        Surface surface(width, height);
        surface.clear();

        auto source = load_image("assets/earth.png");
        assert(source);

        for (auto _ : aState) {
            blit_masked(surface, *source, {0.f, 0.f});
            benchmark::ClobberMemory(); // Prevent optimization
        }


		// The following enables the benchmarking library to print information
		// about the memory bandwidth. The total number of bytes processed is
		// *approximatively* two times the total number of bytes in the blit,
		// accounding for both reading and writing. ("Approximatively" since
		// not all pixels are written.)
        auto const maxBlitX = std::min(width, source->get_width());
        auto const maxBlitY = std::min(height, source->get_height());

        aState.SetBytesProcessed(2 * maxBlitX * maxBlitY * 4 * aState.iterations());
    }

    // Register the new blit function without alpha masking in the benchmark framework
    BENCHMARK(blit_no_alpha_loops_)
		->Args({320, 240})
        ->Args({1280, 720})
        ->Args({1920, 1080})
        ->Args({7680, 4320});

    // Register the existing blit function with alpha masking in the benchmark framework
    BENCHMARK(default_blit_earth_)
		->Args({320, 240})
        ->Args({1280, 720})
        ->Args({1920, 1080})
        ->Args({7680, 4320});
}

BENCHMARK_MAIN();
