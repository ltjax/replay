#pragma once

/*
replay
Software Library

Copyright (c) 2010-2022 Marius Elvert

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

#include <replay/common.hpp>
#include <replay/v2.hpp>

namespace replay
{

/** 3-dimensional vector.
    \note The element type is supposed to be a mathematical group.
    \ingroup Math
*/
template <class T> class v3
{
public:
    /** Element type.
     */
    using value_type = T;

    /** Get a pointer to the internal array.
     */
    inline T* ptr()
    {
        return data;
    }

    /** Get a pointer to the internal array.
     */
    inline const T* ptr() const
    {
        return data;
    }

    /** Index access operator.
     */
    template <class index_type> constexpr value_type& operator[](index_type i)
    {
        return data[i];
    }

    /** Index access operator.
     */
    template <class index_type> constexpr const value_type operator[](index_type i) const
    {
        return data[i];
    }

    // Access
    constexpr v3& reset(value_type x, value_type y, value_type z);
    constexpr v3& reset(value_type value = value_type(0));

    // Linear Algebra
    v3 operator-() const; // Negation

    /** Create a new vector.
        This constructor will leave all values uninitialized.
    */
    explicit v3(uninitialized_tag)
    {
    }

    /** Constructor for user-defined conversions.
        \see convertible_tag
    */
    template <class source_type>
    v3(const source_type& other, typename convertible_tag<source_type, v3>::type empty = 0)
    {
        *this = convert(other);
    }

    /** Create a vector with all elements the same value.
     */
    constexpr explicit v3(value_type value = {})
    : data{ value, value, value }
    {
    }

    /** Create a new vector from seperate values.
        \param xy The first two components.
        \param z The third component.
    */
    constexpr v3(v2<value_type> const& xy, value_type z)
    : data{ xy[0], xy[1], z }
    {
    }

    /** Create a new vector from seperate values.
        \param x The first component.
        \param y The second component.
        \param z The third component.
    */
    constexpr v3(value_type x, value_type y, value_type z)
    : data{x, y, z}
    {
    }

    v3& operator+=(v3<T> const& operand);
    v3& operator-=(v3<T> const& operand);
    v3& operator*=(T const& operand);
    v3& operator/=(T const& operand);

    bool operator==(v3 const& operand) const;
    bool operator!=(v3 const& operand) const;

    /** In-place negate.
        Negates each element of this vector.
    */
    v3& negate();

    value_type squared() const;
    value_type sum() const;

    /** Static element wise type cast.
        This can be used on all indexable array-like types.
    */
    template <class array_type> constexpr inline static v3 cast(array_type const& src)
    {
        return v3<T>(static_cast<T>(src[0]), static_cast<T>(src[1]), static_cast<T>(src[2]));
    }

private:
    /** the actual data.
     */
    T data[3];
};

/** Cross product.
    Also referred to as vector-product, this operator returns a vector that
    is perpendicular to both input vectors.
    The length of this vector is equal to the area of the parallelogram spanned
    by the two input vectors.
    \relates v3
    \ingroup Math
*/
template <class type> inline v3<type> cross(v3<type> const& lhs, v3<type> const& rhs);

/** Dot product of two 3D vectors.
    \relates v3
    \ingroup Math
*/
template <class type> inline type dot(v3<type> const& lhs, v3<type> const& rhs);

/** Component wise product of two 3D vectors.
    \relates v3
    \ingroup Math
*/
template <class type> inline v3<type> comp(v3<type> const& lhs, v3<type> const& rhs);

/** Addition.
    \relates v3
    \ingroup Math
*/
template <class type> inline v3<type> operator+(v3<type> lhs, v3<type> const& rhs)
{
    return lhs += rhs;
}

/** Subtraction.
    \relates v3
    \ingroup Math
*/
template <class type> inline v3<type> operator-(v3<type> lhs, v3<type> const& rhs)
{
    return lhs -= rhs;
}

/** Scalar product.
    \relates v3
    \ingroup Math
*/
template <class type> inline v3<type> operator*(v3<type> lhs, type rhs)
{
    return lhs *= rhs;
}

/** Scalar product.
    \relates v3
    \ingroup Math
*/
template <class type> inline v3<type> operator*(type lhs, v3<type> rhs)
{
    return rhs *= lhs;
}

/** Scalar division.
    \relates v3
    \ingroup Math
*/
template <class type> inline v3<type> operator/(v3<type> lhs, type rhs)
{
    return lhs /= rhs;
}

/** Scalar division.
    \relates v3
    \ingroup Math
*/
template <class type> inline v3<type> operator/(type lhs, v3<type> const& rhs)
{
    return v3<type>(lhs / rhs[0], lhs / rhs[1], lhs / rhs[2]);
}

} // namespace replay


/** Set this objects components.
    \param x The first component.
    \param y The second component.
    \param z The third component.
*/
template <class type>
inline constexpr replay::v3<type>& replay::v3<type>::reset(value_type x, value_type y, value_type z)
{
    data[0] = x;
    data[1] = y;
    data[2] = z;

    return *this;
}

/** Set all components.
    \param value Value to set the vector to.
*/
template <class type> inline constexpr replay::v3<type>& replay::v3<type>::reset(value_type value)
{
    data[0] = value;
    data[1] = value;
    data[2] = value;

    return *this;
}

/** Negation.
*/
template <class type> inline replay::v3<type> replay::v3<type>::operator-() const
{
    return replay::v3<type>(-data[0], -data[1], -data[2]);
}

/** In-place addition.
*/
template <class type>
inline replay::v3<type>& replay::v3<type>::operator+=(const replay::v3<type>& operand)
{
    data[0] += operand.data[0];
    data[1] += operand.data[1];
    data[2] += operand.data[2];

    return *this;
}

/** In-place subtraction.
*/
template <class type>
inline replay::v3<type>& replay::v3<type>::operator-=(const replay::v3<type>& operand)
{
    data[0] -= operand.data[0];
    data[1] -= operand.data[1];
    data[2] -= operand.data[2];

    return *this;
}

/** In-place scalar multiplication.
*/
template <class type> inline replay::v3<type>& replay::v3<type>::operator*=(const type& operand)
{
    data[0] *= operand;
    data[1] *= operand;
    data[2] *= operand;

    return *this;
}

/** In-place scalar division.
*/
template <class type> inline replay::v3<type>& replay::v3<type>::operator/=(const type& operand)
{
    data[0] /= operand;
    data[1] /= operand;
    data[2] /= operand;

    return *this;
}

/** Test for equality.
*/
template <class type> inline bool replay::v3<type>::operator==(v3<type> const& operand) const
{
    return data[0] == operand[0] && data[1] == operand[1] && data[2] == operand[2];
}

/** Test for unequality.
*/
template <class type> inline bool replay::v3<type>::operator!=(v3<type> const& operand) const
{
    return data[0] != operand[0] || data[1] != operand[1] || data[2] != operand[2];
}

template <class type> inline replay::v3<type>& replay::v3<type>::negate()
{
    data[0] = -data[0];
    data[1] = -data[1];
    data[2] = -data[2];

    return *this;
}

/** Square. Square this vector using the dot product. */
template <class type> inline type replay::v3<type>::squared() const
{
    return data[0] * data[0] + data[1] * data[1] + data[2] * data[2];
}

/** Sum. Return a sum of all elements. */
template <class type> inline type replay::v3<type>::sum() const
{
    return data[0] + data[1] + data[2];
}

template <class type> inline replay::v3<type> replay::cross(v3<type> const& lhs, v3<type> const& rhs)
{
    return replay::v3<type>(lhs[1] * rhs[2] - lhs[2] * rhs[1], lhs[2] * rhs[0] - lhs[0] * rhs[2],
                                 lhs[0] * rhs[1] - lhs[1] * rhs[0]);
}

template <class type> inline type replay::dot(v3<type> const& lhs, v3<type> const& rhs)
{
    return lhs[0] * rhs[0] + lhs[1] * rhs[1] + lhs[2] * rhs[2];
}

template <class type> inline replay::v3<type> replay::comp(v3<type> const& lhs, v3<type> const& rhs)
{
    return v3<type>(lhs[0] * rhs[0], lhs[1] * rhs[1], lhs[2] * rhs[2]);
}
