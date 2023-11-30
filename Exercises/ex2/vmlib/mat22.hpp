#ifndef MAT22_HPP_1F974C02_D0D1_4FBD_B5EE_A69C88112088
#define MAT22_HPP_1F974C02_D0D1_4FBD_B5EE_A69C88112088

#include <cmath>

#include "vec2.hpp"

/** Mat22f : 2x2 matrix with floats
 *
 * See comments for Vec2f for some discussion.
 *
 * The matrix is stored in row-major order.
 *
 * Example:
 *   Mat22f identity{ 
 *     1.f, 0.f,
 *     0.f, 1.f
 *   };
 */
struct Mat22f
{
	float _00, _01;
	float _10, _11;
};

// Common operators for Mat22f.
// Note that you will need to implement these yourself.

// 2D-matrix multiplication
constexpr Mat22f operator*(Mat22f const& aLeft, Mat22f const& aRight) noexcept
{
    return {
        aLeft._00 * aRight._00 + aLeft._01 * aRight._10,
        aLeft._00 * aRight._01 + aLeft._01 * aRight._11,
        aLeft._10 * aRight._00 + aLeft._11 * aRight._10,
        aLeft._10 * aRight._01 + aLeft._11 * aRight._11
    };

	/*	
		[ aLeft[0][0]  aLeft[0][1] ] [ aRight[0][0]  aRight[0][1] ] 
		[ aLeft[1][0]  aLeft[1][1] ] [ aRight[1][0]  aRight[1][1] ]
						2			x			2
									
									=
		
		[ aLeft[0][0] * aRight[0][0] + aLeft[0][1] * aRight[1][0]   aLeft[0][0] * aRight[0][1] + aLeft[0][1] * aRight[1][1] ]
		[ aLeft[1][0] * aRight[0][0] + aLeft[1][1] * aRight[1][0]   aLeft[1][0] * aRight[0][1] + aLeft[1][1] * aRight[1][1] ]
									2							x							2
	*/
}

// Matrix-vector multiplication
constexpr Vec2f operator*(Mat22f const& aLeft, Vec2f const& aRight) noexcept
{
    return {
        aLeft._00 * aRight.x + aLeft._01 * aRight.y,
        aLeft._10 * aRight.x + aLeft._11 * aRight.y
    };

	/*
		[ aLeft[0][0]  aLeft[0][1] ] [ aRight.x ] 
		[ aLeft[1][0]  aLeft[1][1] ] [ aRight.y ]
				2	  X		2			2	x	1
								=

		[ aLeft[0][0] * aRight.x + aLeft[0][1] * aRight.y ]
		[ aLeft[1][0] * aRight.x + aLeft[1][1] * aRight.y ]
	*/

}

// Creation of a rotation matrix
inline Mat22f make_rotation_2d(float aAngle) noexcept
{
    float cosA = std::cos(aAngle);
    float sinA = std::sin(aAngle);
    
    return {
        cosA, -sinA,
        sinA,  cosA
    };

	/*
		The standard 2D rotation in an anti-clockwise direction, rotation about origin about an angle θ is:

		[  cos(θ)  -sin(θ) ]
		[  sin(θ)   cos(θ) ]
	*/	
	
}


#endif // MAT22_HPP_1F974C02_D0D1_4FBD_B5EE_A69C88112088
