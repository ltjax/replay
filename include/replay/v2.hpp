#ifndef replay_v2_hpp
#define replay_v2_hpp

/*
replay
Software Library

Copyright (c) 2010-2021 Marius Elvert

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

namespace replay
{

/** 2-dimensional vector.
    \ingroup Math
*/
template <class T> class v2
{
public:
    /** Element type of this vector.
    */
    using value_type = T;

    /** Get a pointer to the internal array.
    */
    constexpr T* ptr()
    {
        return data;
    }

    /** Get a pointer to the internal array.
    */
    constexpr const T* ptr() const
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

    constexpr v2& reset(value_type value = value_type(0));
    constexpr v2& reset(value_type x, value_type y);

    /** Non-initializing constructor.
        Leaves all values uninitialized
    */
    explicit v2(uninitialized_tag)
    {
    }

    /** Constructor for user-defined conversions.
        \see convertible_tag
    */
    template <class source_type>
    v2(source_type const& other, typename convertible_tag<source_type, v2>::type empty = 0)
    {
        *this = convert(other);
    }

    /** Initialize vector by setting all elements to a single value.
    */
    constexpr explicit v2(value_type value = {})
    : data{value, value}
    {
    }

    /** Initialize vector from given individual values.
    */
    constexpr v2(value_type x, value_type y)
    : data{x, y}
    {
    }

    v2 operator-() const;

    v2& operator+=(v2 const& rhs);
    v2& operator-=(v2 const& rhs);
    v2& operator*=(value_type rhs);
    v2& operator/=(value_type rhs);

    bool operator==(v2 const& rhs) const;
    bool operator!=(v2 const& rhs) const;

    v2& negate();

    value_type squared() const;
    value_type sum() const;

    /** Element wise static typecast.
        Works on all indexable types.
    */
    template <class array_type> constexpr inline static v2 cast(array_type const& src)
    {
        return { static_cast<T>(src[0]), static_cast<T>(src[1]) };
    }

private:
    /** the actual data.
    */
    T data[2];
};

/** Construct the vector that is pointing away from the input 90 degrees to the left.
    \relates v2
    \ingroup Math
*/
template <class type> inline v2<type> const left(v2<type> const& rhs)
{
    return v2<type>(-rhs[1], rhs[0]);
}

/** Construct the vector that is pointing away from the input 90 degrees to the right.
    \relates v2
    \ingroup Math
*/
template <class type> inline v2<type> const right(v2<type> const& rhs)
{
    return v2<type>(rhs[1], -rhs[0]);
}

/** Vector dot product of two 2D vectors.
    \relates v2
    \ingroup Math
*/
template <class type> inline type dot(v2<type> const& lhs, v2<type> const& rhs)
{
    return lhs[0] * rhs[0] + lhs[1] * rhs[1];
}

/** Component-wise product of two 2D vectors.
    \relates v2
    \ingroup Math
*/
template <class type> inline v2<type> comp(v2<type> const& lhs, v2<type> const& rhs)
{
    return v2<type>(lhs[0] * rhs[0], lhs[1] * rhs[1]);
}

/** Vector addition.
    \relates v2
    \ingroup Math
*/
template <class type> inline v2<type> operator+(v2<type> lhs, v2<type> const& rhs)
{
    return lhs += rhs;
}

/** Vector substraction.
    \relates v2
    \ingroup Math
*/
template <class type> inline v2<type> operator-(v2<type> lhs, v2<type> const& rhs)
{
    return lhs -= rhs;
}

/** Scalar product.
    \relates v2
    \ingroup Math
*/
template <class type> inline v2<type> operator*(v2<type> lhs, type rhs)
{
    return lhs *= rhs;
}

/** Scalar product.
    \relates v2
    \ingroup Math
*/
template <class type> inline v2<type> operator*(type lhs, v2<type> rhs)
{
    // Implement commutativity
    return rhs *= lhs;
}

/** Scalar division.
    \relates v2
    \ingroup Math
*/
template <class type> inline v2<type> operator/(v2<type> lhs, type rhs)
{
    // Implement commutativity
    return lhs /= rhs;
}

}

#include "v2.inl"

#endif // replay_v2_hpp
