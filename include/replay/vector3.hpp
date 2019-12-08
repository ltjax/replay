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

#ifndef replay_vector3_hpp
#define replay_vector3_hpp

#include <iosfwd>
#include <replay/common.hpp>
#include <replay/vector2.hpp>

namespace replay
{

/** 3-dimensional vector.
    \note The element type is supposed to be a mathematical group.
    \ingroup Math
*/
template <class type> class vector3
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
    template <class index_type> constexpr value_type& operator[](const index_type i)
    {
        return data[i];
    }

    /** Index access operator.
     */
    template <class index_type> constexpr const value_type operator[](const index_type i) const
    {
        return data[i];
    }

    // Access
    constexpr vector3<type>& reset(value_type x, value_type y, value_type z);
    constexpr vector3<type>& reset(value_type value = value_type(0));

    // Linear Algebra
    vector3<type> operator-() const; // Negation

    /** Create a new vector.
        This constructor will leave all values uninitialized.
    */
    explicit vector3(uninitialized_tag)
    {
    }

    /** Constructor for user-defined conversions.
        \see convertible_tag
    */
    template <class source_type>
    vector3(const source_type& other, typename convertible_tag<source_type, vector3>::type empty = 0)
    {
        *this = convert(other);
    }

    /** Create a vector with all elements the same value.
     */
    constexpr explicit vector3(value_type value = value_type(0))
    : data{ value, value, value }
    {
    }

    /** Create a new vector from seperate values.
        \param xy The first two components.
        \param z The third component.
    */
    constexpr vector3(vector2<value_type> const& xy, value_type z)
    : data{ xy[0], xy[1], z }
    {
    }

    /** Create a new vector from seperate values.
        \param x The first component.
        \param y The second component.
        \param z The third component.
    */
    constexpr vector3(value_type x, value_type y, value_type z)
    : data{x, y, z}
    {
    }

    vector3<type>& operator+=(vector3<type> const& operand);
    vector3<type>& operator-=(vector3<type> const& operand);
    vector3<type>& operator*=(const type& operand);
    vector3<type>& operator/=(const type& operand);

    bool operator==(vector3<type> const& operand) const;
    bool operator!=(vector3<type> const& operand) const;

    /** In-place negate.
        Negates each element of this vector.
    */
    vector3<type>& negate();

    value_type squared() const;
    value_type sum() const;

    /** Static element wise type cast.
        This can be used on all indexable array-like types.
    */
    template <class array_type> inline static vector3<type> cast(const array_type& src)
    {
        return vector3<type>(static_cast<type>(src[0]), static_cast<type>(src[1]), static_cast<type>(src[2]));
    }

private:
    /** the actual data.
     */
    type data[3];
};

/** Cross product.
    Also referred to as vector-product, this operator returns a vector that
    is perpendicular to both input vectors.
    The length of this vector is equal to the area of the parallelogram spanned
    by the two input vectors.
    \relates vector3
    \ingroup Math
*/
template <class type> inline vector3<type> cross(vector3<type> const& lhs, vector3<type> const& rhs);

/** Dot product of two 3D vectors.
    \relates vector3
    \ingroup Math
*/
template <class type> inline type dot(vector3<type> const& lhs, vector3<type> const& rhs);

/** Component wise product of two 3D vectors.
    \relates vector3
    \ingroup Math
*/
template <class type> inline vector3<type> comp(vector3<type> const& lhs, vector3<type> const& rhs);

/** Addition.
    \relates vector3
    \ingroup Math
*/
template <class type> inline vector3<type> operator+(vector3<type> lhs, vector3<type> const& rhs)
{
    return lhs += rhs;
}

/** Subtraction.
    \relates vector3
    \ingroup Math
*/
template <class type> inline vector3<type> operator-(vector3<type> lhs, vector3<type> const& rhs)
{
    return lhs -= rhs;
}

/** Scalar product.
    \relates vector3
    \ingroup Math
*/
template <class type> inline vector3<type> operator*(vector3<type> lhs, type rhs)
{
    return lhs *= rhs;
}

/** Scalar product.
    \relates vector3
    \ingroup Math
*/
template <class type> inline vector3<type> operator*(type lhs, vector3<type> rhs)
{
    return rhs *= lhs;
}

/** Scalar division.
    \relates vector3
    \ingroup Math
*/
template <class type> inline vector3<type> operator/(vector3<type> lhs, type rhs)
{
    return lhs /= rhs;
}

/** Scalar division.
    \relates vector3
    \ingroup Math
*/
template <class type> inline vector3<type> operator/(type lhs, vector3<type> const& rhs)
{
    return vector3<type>(lhs / rhs[0], lhs / rhs[1], lhs / rhs[2]);
}

/** A convenience typedef for a 3d floating-point vector.
    \relates vector3
    \ingroup Math
*/
typedef vector3<float> vector3f;

/** A convenience typedef for a 3d double-precision floating-point vector.
    \relates vector3
    \ingroup Math
*/
typedef vector3<double> vector3d;

/** Shorthandle for all 3d vectors
    \relates vector2
    \ingroup Math
*/
template <class T> using v3 = vector3<T>;
} // namespace replay

#include "vector3.inl"

#endif // replay_vector3_hpp
