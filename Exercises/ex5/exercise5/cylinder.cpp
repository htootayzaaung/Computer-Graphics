#include "cylinder.hpp"
#include <cmath>

SimpleMeshData make_cylinder(bool aCapped, std::size_t aSubdivs, Vec3f aColor, Mat44f aPreTransform) {
    std::vector<Vec3f> pos; // Store positions of vertices
    std::vector<Vec3f> col; // Store colors of vertices
    float prevY = std::cos(0.f);
    float prevZ = std::sin(0.f);

    const float kPi = 3.1415926f;

    // Generate cylinder shell
    for (std::size_t i = 0; i < aSubdivs; ++i) {
        float const angle = (i + 1) / float(aSubdivs) * 2.f * kPi;
        float y = std::cos(angle);
        float z = std::sin(angle);

        // Two triangles create one segment of the cylinder's shell
        pos.emplace_back(Vec3f{0.f, prevY, prevZ});
        pos.emplace_back(Vec3f{0.f, y, z});
        pos.emplace_back(Vec3f{1.f, prevY, prevZ});

        pos.emplace_back(Vec3f{0.f, y, z});
        pos.emplace_back(Vec3f{1.f, y, z});
        pos.emplace_back(Vec3f{1.f, prevY, prevZ});

        prevY = y;
        prevZ = z;
    }

    // Generate caps if required
    if (aCapped) {
        // Top cap
        prevY = std::cos(0.f);
        prevZ = std::sin(0.f);
        for (std::size_t i = 0; i < aSubdivs; ++i) {
            float const angle = (i + 1) / float(aSubdivs) * 2.f * kPi;
            float y = std::cos(angle);
            float z = std::sin(angle);

            pos.emplace_back(Vec3f{1.f, 0.f, 0.f}); // center top
            pos.emplace_back(Vec3f{1.f, prevY, prevZ});
            pos.emplace_back(Vec3f{1.f, y, z});

            prevY = y;
            prevZ = z;
        }

        // Bottom cap
        prevY = std::cos(0.f);
        prevZ = std::sin(0.f);
        for (std::size_t i = 0; i < aSubdivs; ++i) {
            float const angle = (i + 1) / float(aSubdivs) * 2.f * kPi;
            float y = std::cos(angle);
            float z = std::sin(angle);

            pos.emplace_back(Vec3f{0.f, 0.f, 0.f}); // center bottom
            pos.emplace_back(Vec3f{0.f, y, z});
            pos.emplace_back(Vec3f{0.f, prevY, prevZ});

            prevY = y;
            prevZ = z;
        }
    }

    // Apply the pre-transformation to each vertex
    for (auto& vertex : pos) {
        Vec4f transformed = aPreTransform * Vec4f{vertex.x, vertex.y, vertex.z, 1.0f};
        vertex = Vec3f{transformed.x, transformed.y, transformed.z};
    }

    // Assign color to each vertex
    col = std::vector<Vec3f>(pos.size(), aColor);

    return SimpleMeshData{ std::move(pos), std::move(col) };
}