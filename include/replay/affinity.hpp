/*
replay
Software Library

Copyright (c) 2010-2019 Marius Elvert

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

#ifndef replay_affinity_hpp
#define replay_affinity_hpp

#include <replay/matrix4.hpp>
#include <replay/quaternion.hpp>
#include <replay/vector3.hpp>

namespace replay
{

/** Describe an orthogonal affine mapping, equivalent to a rotation and an offset.
    The mapping is of the form \f$x \longrightarrow Rx+p\f$, where \f$R\f$ is the
    member variable \ref orientation, represented as a quaternion, and \f$p\f$ is \ref position,
    represented as a vector3<float>.
    As such, it is probably the most common mapping used in 3d graphics.

    The following rules apply for 3 affinities \f$a,b,c\f$:\n
    Associativity, i.e. \f$(ab)c = a(bc)\f$.\n
    One, i.e. \f$a1 = 1a = a\f$.\n
    Inverse elements, i.e. \f$aa^{-1} = a^{-1}a = 1\f$.

    The one is generated by the default constructor, the inverse can be computed by \ref inverse.

    Multiplicating two affinities is isomorphic to concaternating the functions they represent, but this
    is computationally cheaper than multiplying matrices representing the same functions.
    Likewise, finding the inverse is much cheaper than doing it on their matrix counterparts.

    \ingroup Math
*/
class affinity
{
public:
    /** Rotational part of this transformation.
    */
    quaternion orientation;

    /** Offset part of this transformation.
    */
    vector3f position;

    /** construct a default identity transformation.
    */
    affinity()
    {
    }

    /** Construct a mapping based on an offset.
    */
    explicit affinity(const vector3f& position)
    : position(position)
    {
    }

    /** Construct a mapping based on an orientation.
    */
    explicit affinity(const quaternion& orientation)
    : orientation(orientation)
    {
    }

    /** Construct a mapping based on an orientation and an offset.
    */
    affinity(const quaternion& orientation, const vector3f& position)
    : orientation(orientation)
    , position(position)
    {
    }

    /** Constructor for user-defined conversions.
        \see convertible_tag
    */
    template <class T> affinity(const T& other, typename convertible_tag<T, affinity>::type empty = 0)
    {
        *this = convert(other);
    }

    /** Concaternate two mappings a and b.
        the effect is as if b and a were executed in that order.
    */
    affinity operator*(const affinity& rhs) const
    {
        return affinity(orientation * rhs.orientation, transform(orientation, rhs.position) + position);
    }

    /** Inplace concaternate two mappings a and b.
        the effect is as if b and a were executed in that order.
    */
    affinity& operator*=(const affinity& rhs)
    {
        this->position = transform(orientation, rhs.position) + position;
        this->orientation *= rhs.orientation;
        return *this;
    }

    /** Transform a position vector by this mapping.
    */
    const vector3f operator*(const vector3f& rhs) const
    {
        return transform(orientation, rhs) + position;
    }
};

/** Check two affinities for equality.
*/
inline bool operator==(const affinity& lhs, const affinity& rhs)
{
    return lhs.position == rhs.position && lhs.orientation == rhs.orientation;
}

/** Check two affinities for inequality.
*/
inline bool operator!=(const affinity& lhs, const affinity& rhs)
{
    return !(lhs == rhs);
}

/** Get this mapping as a homogenous 4x4 matrix.
*/
inline const matrix4 to_matrix(const affinity& rhs)
{
    return matrix4(rhs.orientation, rhs.position);
}

/** Find the inverse of the given affinity.
    \param rhs The original mapping of which to find the inverse.
    \return The inverse of the mapping.
*/
inline const affinity inverse(const affinity& rhs)
{
    const quaternion rho(replay::inverse(rhs.orientation));
    return affinity(rho, -transform(rho, rhs.position));
}

/** Blend smoothly between two affinities.
    \param lhs The first affinity.
    \param rhs The second affinity.
    \param alpha A value in \f$[0..1]\f$, selecting blend fraction between lhs and rhs.
    \return Result of the blending.
    \note Uses \ref quaternion::nlerp internally.
    \note This is not a linear operator.
*/
inline const affinity blend(const affinity& lhs, const affinity& rhs, float alpha)
{

    return affinity(nlerp(lhs.orientation, rhs.orientation, alpha),
                    (1.f - alpha) * lhs.position + alpha * rhs.position);
}
}

#endif
