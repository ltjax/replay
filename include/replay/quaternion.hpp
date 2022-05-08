#pragma once

#include <replay/v3.hpp>
#include <tuple>

namespace replay
{

/** 4-dimensional vector extended with a multiplication operation.
   Multiplication is defined by viewing it as an imaginary number: w+xi+yj+zk, with i²=j²=k²=-1 and ij=k,jk=i,ki=j (aka
   Hamilton convention). A unit-length quaternion can be intuitively mapped to an axis-angle 3-dimensional rotation,
   although this mapping is ambiguous (a negative of a quaternion represents the same rotation).
   \ingroup Math
*/
class quaternion
{
public:
    float w; /**<w*/
    float x; /**<x*/
    float y; /**<y*/
    float z; /**<z*/

    /** Set this to the identity rotation.
     */
    quaternion& set_identity();

    /** Set this to a rotation around an axis.
        Converts from an axis/angle rotation.
        \param[in] angle Angle of the rotation, in radians.
        \param[in] v Axis of the rotation. Has to be unit length.
        \returns A reference to this object.
        \see convert_to_axis_angle, rotate
    */
    quaternion& set_rotation(float angle, v3<float> const& axis);

    /** Set all individual components.
        \returns A reference to this object.
    */
    quaternion& set(float w, float x, float y, float z);

    /** Create an identity quaternion.
     */
    quaternion();

    /** Create a rotational quaternion.
        \see set_rotation, rotate, convert_to_axis_angle
    */
    quaternion(float angle, v3<float> const& axis);

    /** Create a quaternion by setting all individual components.
     */
    quaternion(float w, float x, float y, float z);

    /** Constructor for user-defined conversions.
        \see convertible_tag
    */
    template <class T> quaternion(const T& other, typename convertible_tag<T, quaternion>::type empty = 0)
    {
        *this = convert(other);
    }

    /** Multiply two quaternions.
     */
    quaternion operator*(const quaternion& rhs) const;

    /** Add two quaternions.
     */
    quaternion operator+(const quaternion& rhs) const;

    /** Subtract two quaternions.
     */
    quaternion operator-(const quaternion& rhs) const;

    /** Scalar multiplication.
     */
    quaternion operator*(float v) const;

    /** Scalar division.
     */
    quaternion operator/(float v) const;

    /** Multiplicative assign of another quaternion.
        \returns A reference to this object.
    */
    quaternion& operator*=(const quaternion& rhs);

    /** Scale.
        \returns A reference to this object.
    */
    quaternion& operator*=(float rhs);

    /** Divide.
        \returns A reference to this object.
    */
    quaternion& operator/=(float rhs);

    /** Negated.
     */
    const quaternion negated() const;

    /** Negate.
        \returns A reference to this object.
    */
    quaternion& negate();

    /** Square this quaternion.
        This is equivalent to the inner product with itself, or the product with its conjugate.
        \see inner_product, conjugate, conjugated
    */
    float squared() const;

    /** Euclidean 2-Norm.
        This is the square-root of this
    */
    float magnitude() const;

    /** Negate all imaginary numbers.
        \returns A reference to this object.
        \see conjugated
    */
    quaternion& conjugate();

    /** Return a quaternion with all imaginary components negated.
        \see conjugate
    */
    const quaternion conjugated() const;

    /** Make this quaterion unit-length.
        \returns A reference to this object.
        \note If the quaternion is zero, the results of this operation are undefined.
    */
    quaternion& normalize();

    /** Get the x vector after transformation by this rotation.
     */
    v3<float> get_transformed_x() const;

    /** Get the y vector after transformation by this rotation.
     */
    v3<float> get_transformed_y() const;

    /** Get the z vector after transformation by this rotation.
     */
    v3<float> get_transformed_z() const;
};

/** Compare two quaternions for equality.
 */
inline bool operator==(quaternion const& lhs, quaternion const& rhs)
{
    return lhs.w == rhs.w && lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

/** Compare two quaternions for inequality.
 */
inline bool operator!=(quaternion const& lhs, quaternion const& rhs)
{
    return !(lhs == rhs);
}

/** Compute the multiplicative inverse of a quaternion.
    \param[in] a Quaternion to compute the inverse of.
    \returns The inverse to the given quaternion.
    \note Results are undefined for the zero quaternion.
*/
quaternion inverse(quaternion const& rhs);

/** Inner/Dot product.
    This is equivalent to a dot product with 4-dimensional vectors.
*/
float dot(quaternion const& lhs, quaternion const& rhs);

/** Quaternion spherical interpolation.
 */
quaternion slerp(quaternion const& lhs, quaternion const& rhs, float alpha);

/** Quaternion linear interpolation.
    Computes are renormalized linear combination of the two given quaternions.
    When \p b is not originally in the neighbourhood of \p a, this will behave as if the negative of b was used.
    \param[in] a The quaternion the defines the result when \p x equals zero.
    \param[in] b The quaternion the defines the result when \p x equals one.
    \param[in] x The weight factor from \f$[0..1]\f$.
    \returns The interpolated and renormalized rotational quaternion.
*/
quaternion nlerp(quaternion const& lhs, quaternion const& rhs, float alpha);

/** Create a short arc rotation from rotation lhs to rotation rhs.
    If the dot product of the two reference rotations is negative, this will negate the result.
*/
quaternion short_rotation(quaternion const& lhs, quaternion const& rhs);

/** Multiply two quaternions.
 */
inline const quaternion multiply(const quaternion& lhs, const quaternion& rhs)
{
    return quaternion(lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z,
                      lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y,
                      lhs.w * rhs.y + lhs.y * rhs.w + lhs.z * rhs.x - lhs.x * rhs.z,
                      lhs.w * rhs.z + lhs.z * rhs.w + lhs.x * rhs.y - lhs.y * rhs.x);
}

/** Convert a quaternion rotation to an axis angle rotation.
    \param[in] q The quaternion to be converted.
    \param[out] angle_result The angle part of the result, in radians.
    \param[out] axis_result The axis part of the result. Will be unit-length.
    \note The result is undefined if the quaternion is not unit-length.
    \see set_rotation, rotate
*/
std::tuple<v3<float>, float> to_axis_angle(const quaternion& obj);

/** Transform a vector by a quaternion.
 */
v3<float> transform(quaternion const& lhs, v3<float> const& rhs);

/** Concaternate a rotation to a given quaternion.
    This is equivalent to creating a new quaternion with the given axis/angle pair, inplace multiplying it from the
   right, and renormalizing.
    \param[in,out] q The quaternion to be modified.
    \param[in] angle The angle to rotate, in radians.
    \param[in] axis The axis to rotate around. Has to be unit-length.
*/
void rotate(quaternion& obj, float angle, v3<float> const& axis);

/** Find the shortest arc rotation that maps vector a to vector b.
 */
const quaternion shortest_arc(v3<float> const& a, v3<float> const& b);

} // namespace replay
