#include "loadobj.hpp"
#include <rapidobj/rapidobj.hpp>
#include "../support/error.hpp"
#include "../vmlib/vec3.hpp"

SimpleMeshData load_wavefront_obj(char const* aPath) {
    // Load the OBJ file
    auto result = rapidobj::ParseFile(aPath);
    if (result.error) {
        throw Error("Unable to load OBJ file '%s': %s", aPath, result.error.code.message().c_str());
    }

    // Triangulate any non-triangle faces
    rapidobj::Triangulate(result);

    // Convert the OBJ data into a SimpleMeshData structure
    SimpleMeshData ret;
    for (const auto& shape : result.shapes) {
        for (std::size_t i = 0; i < shape.mesh.indices.size(); ++i) {
            const auto& idx = shape.mesh.indices[i];

            // Add position
            ret.positions.emplace_back(Vec3f{
                result.attributes.positions[idx.position_index * 3 + 0],
                result.attributes.positions[idx.position_index * 3 + 1],
                result.attributes.positions[idx.position_index * 3 + 2]
            });

            // Calculate face index and get material
            const auto& mat = result.materials[shape.mesh.material_ids[i / 3]];

            // Add color based on material ambient color
            ret.colors.emplace_back(Vec3f{
                mat.ambient[0],
                mat.ambient[1],
                mat.ambient[2]
            });
        }
    }

    return ret;
}
