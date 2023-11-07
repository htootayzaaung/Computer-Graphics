#include <benchmark/benchmark.h>
#include <cmath> // For std::round and std::abs
#include "../draw2d/draw.hpp"
#include "../draw2d/surface.hpp"

// DDA line drawing function
void draw_line_dda(Surface& surface, int x1, int y1, int x2, int y2, ColorU8_sRGB color) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int steps = std::max(std::abs(dx), std::abs(dy));
    float xInc = dx / static_cast<float>(steps);
    float yInc = dy / static_cast<float>(steps);
    float x = x1;
    float y = y1;
    for (int i = 0; i <= steps; ++i) {
        surface.set_pixel_srgb(static_cast<int>(std::round(x)), static_cast<int>(std::round(y)), color);
        x += xInc;
        y += yInc;
    }
}

// Benchmark function for DDA line drawing
void line_drawing_dda_benchmark(benchmark::State& aState) {
    auto const width = std::uint32_t(aState.range(0));
    auto const height = std::uint32_t(aState.range(1));

    Surface surface(width, height);
    ColorU8_sRGB color = {255, 255, 255}; // White color for the line

    for (auto _ : aState) {
        surface.clear();
        draw_line_dda(surface, 0, 0, width - 1, height - 1, color);
        benchmark::ClobberMemory();
    }

    aState.SetBytesProcessed(static_cast<int64_t>(width + height - 1) * aState.iterations());
}

// Assuming the Bresenham's line drawing function is named draw_line_solid and declared in draw.hpp
void line_drawing_bresenham_benchmark(benchmark::State& aState) {
    auto const width = std::uint32_t(aState.range(0));
    auto const height = std::uint32_t(aState.range(1));

    Surface surface(width, height);
    ColorU8_sRGB color = {255, 255, 255}; // White color for the line
    Vec2f begin{0, 0};
    Vec2f end{static_cast<float>(width - 1), static_cast<float>(height - 1)};

    for (auto _ : aState) {
        surface.clear();
        draw_line_solid(surface, begin, end, color); // Call your Bresenham's line drawing function here
        benchmark::ClobberMemory();
    }

    aState.SetBytesProcessed(static_cast<int64_t>(width + height - 1) * aState.iterations());
}

// Register the benchmark functions
BENCHMARK(line_drawing_dda_benchmark)
    ->Args({1920, 1080})
    ->Args({7680, 4320});

BENCHMARK(line_drawing_bresenham_benchmark)
    ->Args({1920, 1080})
    ->Args({7680, 4320});

BENCHMARK_MAIN();
