/*
replay
Software Library

Copyright (c) 2010 Marius Elvert

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.

*/

#ifndef replay_quaternion_hpp
#define replay_quaternion_hpp

#include <replay/vector3.hpp>

namespace replay {

/** A quaternion is a 4-dimensional vector extended with a multiplication operation.
	A unit-length quaternion can be intuitively mapped to an axis-angle 3-dimensional rotation,
	although this mapping is ambiguous (a negative of a quaternion represents the same rotation).
	\ingroup Math
*/
class quaternion
{
public:
	float					w; /**<w*/
	float					x; /**<x*/
	float					y; /**<y*/
	float					z; /**<z*/

	/** Set this to the identity rotation.
	*/
	quaternion&				set_identity();
		
	/** Set this to a rotation around an axis.
		Converts from an axis/angle rotation.
		\param[in] angle Angle of the rotation, in radians.
		\param[in] v Axis of the rotation. Has to be unit length.
		\returns A reference to this object.
		\see convert_to_axis_angle, rotate
	*/
	quaternion&				set_rotation( float angle, const vector3f& v );
		
	/** Set all individual components.
		\returns A reference to this object.
	*/
	quaternion&				set( const float w, const float x, const float y, const float z );

	/** Create an identity quaternion.
	*/
							quaternion();
		
	/** Create a rotational quaternion.
		\see set_rotation, rotate, convert_to_axis_angle
	*/
							quaternion( const float angle, const vector3f& v );
		
	/** Create a quaternion by setting all individual components.
	*/
							quaternion( const float w, const float x, const float y, const float z );

	/** Constructor for user-defined conversions.
		\see convertible_tag
	*/
	template <class source_type> quaternion(const source_type& other, typename convertible_tag<source_type, quaternion>::type empty=0)
	{
		*this = convert(other);
	}

	/** Multiply two quaternions.
	*/
	const quaternion		operator*( const quaternion& q ) const;
		
	/** Add two quaternions.
	*/
	const quaternion		operator+( const quaternion& q ) const;

	/** Subtract two quaternions.
	*/
	const quaternion		operator-( const quaternion& q ) const;
		
	/** Scalar multiplication.
	*/
	const quaternion		operator*( const float v ) const;
		
	/** Scalar division.
	*/
	const quaternion		operator/( const float v ) const;
		
	/** Multiplicative assign of another quaternion.
		\returns A reference to this object.
	*/
	quaternion&				operator*=( const quaternion& q );
		
	/** Scale.
		\returns A reference to this object.
	*/
	quaternion&				operator*=( const float v );
		
	/** Divide.
		\returns A reference to this object.
	*/
	quaternion&				operator/=( const float v );

	/** Negated.
	*/
	const quaternion		negated() const;

	/** Negate.
		\returns A reference to this object.
	*/
	quaternion&				negate();

	/** Square this quaternion.
		This is equivalent to the inner product with itself, or the product with its conjugate.
		\see inner_product, conjugate, conjugated
	*/
	const float				squared() const;
		
	/** Euclidean 2-Norm.
		This is the square-root of this 
	*/
	const float				magnitude() const;

	/** Negate all imaginary numbers.
		\returns A reference to this object.
		\see conjugated
	*/
	quaternion&				conjugate();
		
	/** Return a quaternion with all imaginary components negated.
		\see conjugate
	*/
	const quaternion		conjugated() const;

	/** Make this quaterion unit-length.
		\returns A reference to this object.
		\note If the quaternion is zero, the results of this operation are undefined.
	*/
	quaternion&				normalize();

	/** Get the x vector after transformation by this rotation.
	*/
	const vector3f			get_transformed_x() const;
		
	/** Get the y vector after transformation by this rotation.
	*/
	const vector3f			get_transformed_y() const;
		
	/** Get the z vector after transformation by this rotation.
	*/
	const vector3f			get_transformed_z() const;

	/** Transform a vector by a quaternion.
	*/
	static vector3f			transform( const quaternion& a, const vector3f& operand );
		
	/** Multiply a quaterion.
	*/
	static quaternion&		multiply( const quaternion& a, const quaternion& b, quaternion& result );
		
	/** Compute the multiplicative inverse of a quaternion.
		\param[in] a Quaternion to compute the inverse of.
		\returns The inverse to the given quaternion.
		\note Results are undefined for the zero quaternion.
	*/
	static const quaternion	inverse( const quaternion& a );
		
	/** Concaternate a rotation to a given quaternion.
		This is equivalent to creating a new quaternion with the given axis/angle pair, inplace multiplying it from the right, and renormalizing.
		\param[in,out] q The quaternion to be modified.
		\param[in] angle The angle to rotate, in radians.
		\param[in] axis The axis to rotate around. Has to be unit-length.
	*/
	static void				rotate( quaternion&	q, const float angle, const vector3f& axis );
		
	/** Convert a quaternion rotation to an axis angle rotation.
		\param[in] q The quaternion to be converted.
		\param[out] angle_result The angle part of the result, in radians.
		\param[out] axis_result The axis part of the result. Will be unit-length.
		\note The result is undefined if the quaternion is not unit-length.
		\see set_rotation, rotate
	*/
	static void				convert_to_axis_angle( const quaternion& q, float& angle_result, vector3f& axis_result ); 
		
	/** Quaternion spherical interpolation.
	*/		
	static const quaternion	slerp( const quaternion& a, const quaternion& b, const float x );
		
	/** Quaternion linear interpolation.
		Computes are renormalized linear combination of the two given quaternions.
		When \p b is not originally in the neighbourhood of \p a, this will behave as if the negative of b was used.
		\param[in] a The quaternion the defines the result when \p x equals zero.
		\param[in] b The quaternion the defines the result when \p x equals one.
		\param[in] x The weight factor from \f$[0..1]\f$.
		\returns The interpolated and renormalized rotational quaternion.
	*/
	static const quaternion	nlerp( const quaternion& a, const quaternion& b, const float x );
		
	/** Inner/Dot product.
	*/
	static const float		inner_product( const quaternion& a, const quaternion& b );
		
	/** Find the shortest arc rotation that maps vector a to vector b.
	*/
	static const quaternion	shortest_arc( const vector3f& a, const vector3f& b );

	/** Create a short arc rotation from rotation a to rotation b.
		If the dot product of the two reference rotations is negative, this will negate the result.
	*/
	static const quaternion	short_rotation( const quaternion& a, const quaternion& b );
};


}

inline replay::quaternion&
replay::quaternion::multiply( const quaternion& a, const quaternion& b, quaternion& result )
{
	result.w = a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z;
	result.x = a.w*b.x + a.x*b.w + a.y*b.z - a.z*b.y;
	result.y = a.w*b.y + a.y*b.w + a.z*b.x - a.x*b.z;
	result.z = a.w*b.z + a.z*b.w + a.x*b.y - a.y*b.x;

	return result;
}

#endif // replay_quaternion_hpp

