#include "cylinder.hpp"
#include <cmath>

SimpleMeshData make_cylinder(bool aCapped, std::size_t aSubdivs, Vec3f aColor, Mat44f aPreTransform) {
    std::vector<Vec3f> pos; // Store positions of vertices
    float prevY = std::cos(0.f);
    float prevZ = std::sin(0.f);

    const float kPi = 3.1415926f;
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

    // TODO: Implement caps if aCapped is true

    // Apply the pre-transformation to each vertex
    for (auto& vertex : pos) {
        Vec4f transformed = aPreTransform * Vec4f{vertex.x, vertex.y, vertex.z, 1.0f};
        vertex = Vec3f{transformed.x, transformed.y, transformed.z};
    }

    // Initialize the colors vector with the same size as pos, filled with aColor
    std::vector<Vec3f> col(pos.size(), aColor);

    return SimpleMeshData{ std::move(pos), std::move(col) };
}
