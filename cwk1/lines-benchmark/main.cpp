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

void line_drawing_dda_benchmark(benchmark::State& aState) {
    auto const width = std::uint32_t(aState.range(0));
    auto const height = std::uint32_t(aState.range(1));

    Surface surface(width, height);

    // Four specific cases: horizontal, vertical, diagonal, and steep
    Vec2f begin, end;
    if (aState.thread_index() == 0) {
        // Horizontal line
        begin = {0.0f, static_cast<float>(height) / 2.0f};
        end = {static_cast<float>(width) - 1.0f, static_cast<float>(height) / 2.0f};
    } else if (aState.thread_index() == 1) {
        // Vertical line
        begin = {static_cast<float>(width) / 2.0f, 0.0f};
        end = {static_cast<float>(width) / 2.0f, static_cast<float>(height) - 1.0f};
    } else if (aState.thread_index() == 2) {
        // Diagonal line
        begin = {0.0f, 0.0f};
        end = {static_cast<float>(width) - 1.0f, static_cast<float>(height) - 1.0f};
    } else if (aState.thread_index() == 3) {
        // Steep line
        begin = {0.0f, 0.0f};
        end = {static_cast<float>(height) - 1.0f, static_cast<float>(width) - 1.0f};
    }

    for (auto _ : aState) {
        surface.clear();
        draw_line_dda(surface, static_cast<int>(begin.x), static_cast<int>(begin.y), static_cast<int>(end.x), static_cast<int>(end.y), {255, 255, 255}); // White color for the line
        benchmark::ClobberMemory();
    }

    aState.SetBytesProcessed(static_cast<int64_t>(width + height - 1) * aState.iterations());
}

void line_drawing_bresenham_benchmark(benchmark::State& aState) {
    auto const width = std::uint32_t(aState.range(0));
    auto const height = std::uint32_t(aState.range(1));

    Surface surface(width, height);

    // Four specific cases: horizontal, vertical, diagonal, and steep
    Vec2f begin, end;
    if (aState.thread_index() == 0) {
        // Horizontal line
        begin = {0.0f, static_cast<float>(height) / 2.0f};
        end = {static_cast<float>(width) - 1.0f, static_cast<float>(height) / 2.0f};
    } else if (aState.thread_index() == 1) {
        // Vertical line
        begin = {static_cast<float>(width) / 2.0f, 0.0f};
        end = {static_cast<float>(width) / 2.0f, static_cast<float>(height) - 1.0f};
    } else if (aState.thread_index() == 2) {
        // Diagonal line
        begin = {0.0f, 0.0f};
        end = {static_cast<float>(width) - 1.0f, static_cast<float>(height) - 1.0f};
    } else if (aState.thread_index() == 3) {
        // Steep line
        begin = {0.0f, 0.0f};
        end = {static_cast<float>(height) - 1.0f, static_cast<float>(width) - 1.0f};
    }

    for (auto _ : aState) {
        surface.clear();
        draw_line_solid(surface, begin, end, {255, 255, 255}); // White color for the line
        benchmark::ClobberMemory();
    }


    aState.SetBytesProcessed(static_cast<int64_t>(width + height - 1) * aState.iterations());
}

// Register the benchmark functions
BENCHMARK(line_drawing_dda_benchmark)
    ->Args({320, 240})
    ->Args({1280, 720})
    ->Args({1920, 1080})
    ->Args({7680, 4320});

BENCHMARK(line_drawing_bresenham_benchmark)
    ->Args({320, 240})
    ->Args({1280, 720})
    ->Args({1920, 1080})
    ->Args({7680, 4320});

BENCHMARK_MAIN();
