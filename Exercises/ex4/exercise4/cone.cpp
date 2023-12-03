#include "cone.hpp"
#include <cmath>

SimpleMeshData make_cone(bool aCapped, std::size_t aSubdivs, Vec3f aColor, Mat44f aPreTransform) {
    std::vector<Vec3f> pos;
    std::vector<Vec3f> col;
    float prevY = std::cos(0.f);
    float prevZ = std::sin(0.f);

    const float kPi = 3.1415926f;

    // Generate cone shell
    for (std::size_t i = 0; i < aSubdivs; ++i) {
        float const angle = (i + 1) / float(aSubdivs) * 2.f * kPi;
        float y = std::cos(angle);
        float z = std::sin(angle);

        // Triangle for cone shell - one vertex at base, one at apex, and one at next base point
        pos.emplace_back(Vec3f{1.f, 0.f, 0.f}); // Apex of the cone
        pos.emplace_back(Vec3f{0.f, prevY, prevZ}); // Base
        pos.emplace_back(Vec3f{0.f, y, z}); // Next point on base

        prevY = y;
        prevZ = z;
    }

    // Generate cap if required
    if (aCapped) {
        prevY = std::cos(0.f);
        prevZ = std::sin(0.f);
        for (std::size_t i = 0; i < aSubdivs; ++i) {
            float const angle = (i + 1) / float(aSubdivs) * 2.f * kPi;
            float y = std::cos(angle);
            float z = std::sin(angle);

            pos.emplace_back(Vec3f{0.f, 0.f, 0.f}); // Center of the base
            pos.emplace_back(Vec3f{0.f, y, z}); // Point on the base
            pos.emplace_back(Vec3f{0.f, prevY, prevZ}); // Next point on base

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
