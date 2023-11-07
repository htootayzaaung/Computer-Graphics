#include <benchmark/benchmark.h>
#include <algorithm>
#include <cassert>
#include <cstring>
#include "../draw2d/image.hpp"
#include "../draw2d/surface.hpp"

// Blit function without alpha masking using loops
void blit_no_alpha_loops_(benchmark::State& aState, const std::string& image_path) {
    auto const fb_width = std::uint32_t(aState.range(0));
    auto const fb_height = std::uint32_t(aState.range(1));

    Surface surface(fb_width, fb_height);
    surface.clear();

    auto source = load_image(image_path.c_str());
    assert(source);

    for (auto _ : aState) {
        // Blit implementation without alpha masking
        for (std::uint32_t y = 0; y < std::min(fb_height, source->get_height()); ++y) {
            for (std::uint32_t x = 0; x < std::min(fb_width, source->get_width()); ++x) {
                ColorU8_sRGB_Alpha pixel = source->get_pixel(x, y);
                surface.set_pixel_srgb(x, y, { pixel.r, pixel.g, pixel.b });
            }
        }
        benchmark::ClobberMemory(); // Prevent optimization
    }

    aState.SetBytesProcessed(2 * std::min(fb_width, source->get_width()) * std::min(fb_height, source->get_height()) * 4 * aState.iterations());
}

// Blit function using std::memcpy without alpha masking
void blit_no_alpha_memcpy_(benchmark::State& aState, const std::string& image_path) {
    auto const fb_width = std::uint32_t(aState.range(0));
    auto const fb_height = std::uint32_t(aState.range(1));

    Surface surface(fb_width, fb_height);
    surface.clear();

    auto source = load_image(image_path.c_str());
    assert(source);

    // Ensure the dimensions match for blitting
    auto const blit_width = std::min(fb_width, source->get_width());
    auto const blit_height = std::min(fb_height, source->get_height());

    for (auto _ : aState) {
        // Use const_cast to cast away the constness of the surface data pointer
        auto* surface_data = const_cast<std::uint8_t*>(surface.get_surface_ptr());
        auto const* image_data = source->get_image_ptr();

        // Calculate the line width in bytes
        auto const line_size = blit_width * sizeof(ColorU8_sRGB);

        for (std::uint32_t y = 0; y < blit_height; ++y) {
            // Compute the addresses of the source and destination lines
            auto* dest_line = surface_data + y * fb_width * sizeof(ColorU8_sRGB);
            const auto* src_line = image_data + y * source->get_width() * sizeof(ColorU8_sRGB);

            // Perform the line copy
            std::memcpy(dest_line, src_line, line_size);
        }
        benchmark::ClobberMemory(); // Prevent optimization
    }

    aState.SetBytesProcessed(static_cast<int64_t>(blit_width) * blit_height * 4 * aState.iterations());
}

void blit_no_alpha_loops_small_(benchmark::State& aState) {
    blit_no_alpha_loops_(aState, "assets/impostor.png");
}

void blit_no_alpha_loops_large_(benchmark::State& aState) {
    blit_no_alpha_loops_(aState, "assets/mortal_kombat.png");
}

void blit_no_alpha_memcpy_small_(benchmark::State& aState) {
    blit_no_alpha_memcpy_(aState, "assets/impostor.png");
}

void blit_no_alpha_memcpy_large_(benchmark::State& aState) {
    blit_no_alpha_memcpy_(aState, "assets/mortal_kombat.png");
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

// Register the benchmark functions
BENCHMARK(blit_no_alpha_memcpy_small_)
    ->Args({320, 240})
    ->Args({1280, 720})
    ->Args({1920, 1080})
    ->Args({7680, 4320});

BENCHMARK(blit_no_alpha_memcpy_large_)
    ->Args({320, 240})
    ->Args({1280, 720})
    ->Args({1920, 1080})
    ->Args({7680, 4320});

BENCHMARK(blit_no_alpha_loops_small_)
    ->Args({320, 240})
    ->Args({1280, 720})
    ->Args({1920, 1080})
    ->Args({7680, 4320});

BENCHMARK(blit_no_alpha_loops_large_)
    ->Args({320, 240})
    ->Args({1280, 720})
    ->Args({1920, 1080})
    ->Args({7680, 4320});

/*
BENCHMARK(default_blit_earth_)
    ->Args({320, 240})
    ->Args({1280, 720})
    ->Args({1920, 1080})
    ->Args({7680, 4320});
*/

BENCHMARK_MAIN();
