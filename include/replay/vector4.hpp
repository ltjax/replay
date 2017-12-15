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

#ifndef replay_vector4_hpp
#define replay_vector4_hpp

#include <iosfwd>
#include <replay/vector2.hpp>
#include <replay/vector3.hpp>

namespace replay
{

/** 4-dimensional vector.
    \ingroup Math
*/
template <class type> class vector4
{
public:
    /** Element type.
    */
    typedef type value_type;

    /** Get a pointer to the internal array.
    */
    inline type* ptr()
    {
        return data;
    }

    /** Get a pointer to the internal array.
    */
    inline const type* ptr() const
    {
        return data;
    }

    /** Index access operator.
    */
    template <class index_type> inline value_type& operator[](const index_type i)
    {
        return data[i];
    }

    /** Index access operator.
    */
    template <class index_type> inline const value_type operator[](const index_type i) const
    {
        return data[i];
    }

    /** Set all elements to a single value. Default to zero.
    */
    vector4<type>& reset(value_type value = value_type(0));

    /** Assemble a 4D vector by concatenating a 3D vector and a 4th element.
    */
    vector4<type>& reset(const vector3<type>& xyz, value_type w);

    /** Assemble a 4D vector by concatenating two 2D vectors.
    */
    vector4<type>& reset(const vector2<type>& xy, const vector2<type>& zw);

    /** Assemble a 4D vector by concatenating a 2D vector and 2 more values.
    */
    vector4<type>& reset(const vector2<type>& xy, value_type z, value_type w);

    /** Set the elements of the vector individually.
        \param x The first component.
        \param y The second component.
        \param z The third component.
        \param w The fourth component.
    */
    vector4<type>& reset(value_type x, value_type y, value_type z, value_type w);

    vector4<type>& operator+=(const vector4<type>& rhs);
    vector4<type>& operator-=(const vector4<type>& rhs);
    vector4<type>& operator*=(const type value);
    vector4<type>& operator/=(const type value);
    vector4<type> operator-() const;

    bool operator==(const vector4<type>& operand) const;
    bool operator!=(const vector4<type>& operand) const;

    /** In-place negate.
        Negates each component of this vector.
    */
    vector4<type>& negate();

    type sum() const;
    type squared() const;

    /** Non-initializing constructor.
        Leaves all elements uninitialized.
    */
    explicit vector4(uninitialized_tag)
    {
    }

    /** Constructor for user-defined conversions.
        \see convertible_tag
    */
    template <class source_type>
    vector4(const source_type& other, typename convertible_tag<source_type, vector4>::type empty = 0)
    {
        *this = convert(other);
    }

    /** Single-value constructor.
        Sets all elements to the given value. Defaults to zero.
    */
    explicit vector4(value_type value = value_type(0))
    {
        reset(value);
    }

    /** Assemble a 4D vector by concatenating a 3D vector and a 4th element.
    */
    vector4(const vector3<type>& xyz, value_type w)
    {
        reset(xyz, w);
    }

    /** Assemble a 4D vector by concatenating two 2D vectors.
    */
    vector4(const vector2<type>& xy, const vector2<type>& zw)
    {
        reset(xy, zw);
    }

    /** Create a new vector from seperate values.
        \param x The first component.
        \param y The second component.
        \param z The third component.
        \param w The fourth component.
    */
    vector4(value_type x, value_type y, value_type z, value_type w)
    {
        reset(x, y, z, w);
    }

    /** Assemble a 4D vector by concatenating a 2D vector and 2 more values.
    */
    vector4(const vector2<type>& xy, value_type z, value_type w)
    {
        reset(xy, z, w);
    }

    /** Convert an array-like type to a 4D vector.
        The parameter needs to be indexable for at least 4 elements.
    */
    template <class array_type> static vector4<type> cast(const array_type& src)
    {
        return vector4<type>(static_cast<type>(src[0]), static_cast<type>(src[1]), static_cast<type>(src[2]),
                             static_cast<type>(src[3]));
    }

private:
    /** The actual data.
    */
    type data[4];
};

/** Scalar dot product of two 4D vectors.
    \relates vector4
    \ingroup Math
*/
template <class type> inline type dot(const vector4<type>& lhs, const vector4<type>& rhs);

/** Component wise multiplication of two 4D vectors.
    \relates vector4
    \ingroup Math
*/
template <class type> inline vector4<type> comp(const vector4<type>& lhs, const vector4<type>& rhs);

/** Scalar product.
    \relates vector4
    \ingroup Math
*/
template <class type> vector4<type> operator*(const type lhs, vector4<type> rhs)
{
    return rhs *= lhs;
}

/** Addition.
    \relates vector4
    \ingroup Math
*/
template <class type> vector4<type> operator+(vector4<type> lhs, const vector4<type>& rhs)
{
    return lhs += rhs;
}

/** Substraction.
    \relates vector4
    \ingroup Math
*/
template <class type> vector4<type> operator-(vector4<type> lhs, const vector4<type>& rhs)
{
    return lhs -= rhs;
}

/** Scalar product.
    \relates vector4
    \ingroup Math
*/
template <class type> vector4<type> operator*(vector4<type> lhs, const type rhs)
{
    return lhs *= rhs;
}

/** Scalar division.
    \relates vector4
    \ingroup Math
*/
template <class type> vector4<type> operator/(vector4<type> lhs, const type rhs)
{
    return lhs /= rhs;
}

/** A convenience typedef for a 4d floating-point vector.
    \relates vector4
    \ingroup Math
*/
typedef vector4<float> vector4f;

/** A convenience typedef for a 4d double-precision floating-point vector.
    \relates vector4
    \ingroup Math
*/
typedef vector4<double> vector4d;
}

#include "vector4.inl"

#endif // replay_vector4_hpp
