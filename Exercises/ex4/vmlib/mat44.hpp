#ifndef MAT44_HPP_E7187A26_469E_48AD_A3D2_63150F05A4CA
#define MAT44_HPP_E7187A26_469E_48AD_A3D2_63150F05A4CA

#include <cmath>
#include <cassert>
#include <cstdlib>

#include "vec3.hpp"
#include "vec4.hpp"

struct Mat44f
{
    float v[16];

    constexpr
    float& operator() (std::size_t aI, std::size_t aJ) noexcept {
        assert(aI < 4 && aJ < 4);
        return v[aI * 4 + aJ];
    }

    constexpr
    float const& operator() (std::size_t aI, std::size_t aJ) const noexcept {
        assert(aI < 4 && aJ < 4);
        return v[aI * 4 + aJ];
    }
};

constexpr Mat44f kIdentity44f = {
    1.f, 0.f, 0.f, 0.f,
    0.f, 1.f, 0.f, 0.f,
    0.f, 0.f, 1.f, 0.f,
    0.f, 0.f, 0.f, 1.f
};

constexpr
Mat44f operator*(Mat44f const& aLeft, Mat44f const& aRight) noexcept {
    Mat44f result{};
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result(i, j) = 0.f;
            for (int k = 0; k < 4; ++k) {
                result(i, j) += aLeft(i, k) * aRight(k, j);
            }
        }
    }
    return result;
}

constexpr
Vec4f operator*(Mat44f const& aLeft, Vec4f const& aRight) noexcept {
    Vec4f result{};
    for (int i = 0; i < 4; ++i) {
        result[i] = 0.f;
        for (int j = 0; j < 4; ++j) {
            result[i] += aLeft(i, j) * aRight[j];
        }
    }
    return result;
}

inline
Mat44f make_rotation_x(float aAngle) noexcept {
    float c = std::cos(aAngle);
    float s = std::sin(aAngle);
    return Mat44f{
        1.f, 0.f, 0.f, 0.f,
        0.f, c,   -s,  0.f,
        0.f, s,   c,   0.f,
        0.f, 0.f, 0.f, 1.f
    };
}

inline
Mat44f make_rotation_y(float aAngle) noexcept {
    float c = std::cos(aAngle);
    float s = std::sin(aAngle);
    return Mat44f{
        c,   0.f, s,   0.f,
        0.f, 1.f, 0.f, 0.f,
        -s,  0.f, c,   0.f,
        0.f, 0.f, 0.f, 1.f
    };
}

inline
Mat44f make_rotation_z(float aAngle) noexcept {
    float c = std::cos(aAngle);
    float s = std::sin(aAngle);
    return Mat44f{
        c,   -s,  0.f, 0.f,
        s,   c,   0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        0.f, 0.f, 0.f, 1.f
    };
}

inline
Mat44f make_translation(Vec3f aTranslation) noexcept {
    return Mat44f{
        1.f, 0.f, 0.f, aTranslation.x,
        0.f, 1.f, 0.f, aTranslation.y,
        0.f, 0.f, 1.f, aTranslation.z,
        0.f, 0.f, 0.f, 1.f
    };
}

inline
Mat44f make_perspective_projection(float aFovInRadians, float aAspect, float aNear, float aFar) noexcept {
    assert(aNear != aFar);
    float const tanHalfFovy = std::tan(aFovInRadians / 2.f);
    Mat44f result{};
    result(0, 0) = 1.f / (aAspect * tanHalfFovy);
    result(1, 1) = 1.f / (tanHalfFovy);
    result(2, 2) = -(aFar + aNear) / (aFar - aNear);
    result(2, 3) = -(2.f * aFar * aNear) / (aFar - aNear);
    result(3, 2) = -1.f;
    result(3, 3) = 0.f;
    return result;
}

inline 
Mat44f lookAt(Vec3f const& eye, Vec3f const& center, Vec3f const& up) noexcept {
    Vec3f f = normalize(center - eye);
    Vec3f s = normalize(cross(f, up));
    Vec3f u = cross(s, f);

    Mat44f result = kIdentity44f;
    result(0, 0) = s.x;
    result(1, 0) = s.y;
    result(2, 0) = s.z;
    result(0, 1) = u.x;
    result(1, 1) = u.y;
    result(2, 1) = u.z;
    result(0, 2) = -f.x;
    result(1, 2) = -f.y;
    result(2, 2) = -f.z;
    result(3, 0) = -dot(s, eye);
    result(3, 1) = -dot(u, eye);
    result(3, 2) = dot(f, eye);
    return result;
}

inline Mat44f rotateY(float angle) noexcept{
    float c = cos(angle);
    float s = sin(angle);

    return Mat44f{
        c,   0.0f, s,   0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        -s,  0.0f, c,   0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
}


#endif // MAT44_HPP_E7187A26_469E_48AD_A3D2_63150F05A4CA