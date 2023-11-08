#include <benchmark/benchmark.h>
#include <algorithm>
#include <cassert>
#include <cstring>
#include "../draw2d/image.hpp"
#include "../draw2d/surface.hpp"

// Forward declaration of the blit_masked function.
void blit_masked(Surface& aSurface, ImageRGBA const& aImage, Vec2f aPosition);

// Blit function without alpha masking using loops
void blit_no_alpha_loops_(benchmark::State& aState, const std::string& image_path) {
    auto const fb_width = std::uint32_t(aState.range(0));
    auto const fb_height = std::uint32_t(aState.range(1));
    Surface surface(fb_width, fb_height);
    surface.clear();
    auto source = load_image(image_path.c_str());
    assert(source);
    for (auto _ : aState) {
        for (std::uint32_t y = 0; y < std::min(fb_height, source->get_height()); ++y) {
            for (std::uint32_t x = 0; x < std::min(fb_width, source->get_width()); ++x) {
                ColorU8_sRGB_Alpha pixel = source->get_pixel(x, y);
                surface.set_pixel_srgb(x, y, { pixel.r, pixel.g, pixel.b });
            }
        }
        benchmark::ClobberMemory();
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
    auto const blit_width = std::min(fb_width, source->get_width());
    auto const blit_height = std::min(fb_height, source->get_height());
    for (auto _ : aState) {
        auto* surface_data = const_cast<std::uint8_t*>(surface.get_surface_ptr());
        auto const* image_data = source->get_image_ptr();
        auto const line_size = blit_width * sizeof(ColorU8_sRGB);
        for (std::uint32_t y = 0; y < blit_height; ++y) {
            auto* dest_line = surface_data + y * fb_width * sizeof(ColorU8_sRGB);
            const auto* src_line = image_data + y * source->get_width() * sizeof(ColorU8_sRGB);
            std::memcpy(dest_line, src_line, line_size);
        }
        benchmark::ClobberMemory();
    }
    aState.SetBytesProcessed(static_cast<int64_t>(blit_width) * blit_height * 4 * aState.iterations());
}

// Benchmark function for alpha-masked blit operation
void benchmark_blit_masked(benchmark::State& aState, const std::string& image_path) {
    auto const fb_width = std::uint32_t(aState.range(0));
    auto const fb_height = std::uint32_t(aState.range(1));
    Surface surface(fb_width, fb_height);
    surface.clear();
    auto source = load_image(image_path.c_str());
    assert(source);
    for (auto _ : aState) {
        blit_masked(surface, *source, {0.f, 0.f});
        benchmark::ClobberMemory();
    }
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

BENCHMARK_CAPTURE(benchmark_blit_masked, earth, "assets/earth.png")
    ->Args({320, 240})
    ->Args({1280, 720})
    ->Args({1920, 1080})
    ->Args({7680, 4320});

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


BENCHMARK_CAPTURE(blit_no_alpha_loops_, mortal_kombat, "assets/earth.png")
    ->Args({320, 240})
    ->Args({1280, 720})
    ->Args({1920, 1080})
    ->Args({7680, 4320});


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


BENCHMARK_CAPTURE(blit_no_alpha_memcpy_, mortal_kombat, "assets/earth.png")
    ->Args({320, 240})
    ->Args({1280, 720})
    ->Args({1920, 1080})
    ->Args({7680, 4320});

BENCHMARK_MAIN();
