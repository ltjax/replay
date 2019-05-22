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

#ifndef replay_transformation_hpp
#define replay_transformation_hpp

#include <replay/affinity.hpp>
#include <replay/vector_math.hpp>

namespace replay
{

/** Describe orthogonal affine transformations.
    This is a bit more generic and powerful than \ref affinity, since it can also represent mirrors.
    \ingroup Math
*/
class transformation
{
public:
    /** Rotational part of this transformation.
    */
    quaternion orientation;

    /** Offset part of this transformation.
    */
    vector3f position;

    /** Sign for multiplication. has to be either 1 or -1.
        If the value is neither of the above, it is handled
        as if the number is -1 if sign is negative and as 1
        otherwise.
    */
    float sign;

    /** Construct a default Id transformation.
    */
    transformation()
    : sign(1.f)
    {
    }

    /** Construct a transformation from an affinity.
    */
    transformation(const affinity& rhs)
    : orientation(rhs.orientation)
    , position(rhs.position)
    , sign(1.f)
    {
    }

    /** Construct a transformation based on an offset.
    */
    explicit transformation(const vector3f& position)
    : position(position)
    , sign(1.f)
    {
    }

    /** Construct a transformation based on an orientation.
    */
    explicit transformation(const quaternion& orientation)
    : orientation(orientation)
    , sign(1.f)
    {
    }

    /** Construct a transformation based on an orientation and an offset.
    */
    transformation(const quaternion& orientation, const vector3f& position)
    : orientation(orientation)
    , position(position)
    , sign(1.f)
    {
    }

    /** Construct a transformation based on an orientation, a sign, and an offset.
    */
    transformation(const quaternion& orientation, const vector3f& position, float sign)
    : orientation(orientation)
    , position(position)
    , sign(sign)
    {
    }

    /** Concaternate two transformations a and b.
        the effect is as if b and a were executed in that order.
    */
    transformation operator*(const transformation& other) const
    {
        return transformation(orientation * other.orientation,
                              transform(orientation, math::mult_by_sign(other.position, sign)) + position,
                              sign * other.sign);
    }

    /** Concaternate two transformations a and b.
        the effect is as if b and a were executed in that order.
    */
    transformation& operator*=(const transformation& other)
    {
        this->position = transform(orientation, math::mult_by_sign(other.position, sign)) + position;
        this->orientation *= other.orientation;
        this->sign *= other.sign;
        return *this;
    }
};

/** Convert this transformation to a homogenous 4x4 matrix.
*/
inline const matrix4 to_matrix(const transformation& rhs)
{
    return matrix4(rhs.orientation, rhs.position, rhs.sign);
}

/** Find the inverse of the given transformation.
    \param t The transformation of which to find the inverse.
    \return The inverse of the transformation
*/
inline const transformation inverse(const transformation& rhs)
{
    const quaternion temp = inverse(rhs.orientation);

    return transformation(temp, -math::mult_by_sign(transform(temp, rhs.position), rhs.sign), rhs.sign);
}
}

#endif
