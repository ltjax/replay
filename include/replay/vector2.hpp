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

#ifndef replay_vector2_hpp
#define replay_vector2_hpp

#include <algorithm>
#include <iosfwd>
#include <replay/common.hpp>

namespace replay
{

/** 2-dimensional vector.
    \ingroup Math
*/
template <class type> class vector2
{
public:
    /** Element type of this vector.
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

    vector2<type>& reset(value_type value = value_type(0));
    vector2<type>& reset(value_type x, value_type y);

    /** Non-initializing constructor.
        Leaves all values uninitialized
    */
    explicit vector2(uninitialized_tag)
    {
    }

    /** Constructor for user-defined conversions.
        \see convertible_tag
    */
    template <class source_type>
    vector2(const source_type& other, typename convertible_tag<source_type, vector2>::type empty = 0)
    {
        *this = convert(other);
    }

    /** Initialize vector by setting all elements to a single value.
    */
    explicit vector2(value_type value = type(0))
    {
        reset(value);
    }

    /** Initialize vector from given individual values.
    */
    vector2(value_type x, value_type y)
    {
        reset(x, y);
    }

    vector2<type> operator-() const;

    vector2<type>& operator+=(const vector2<type>& operand);
    vector2<type>& operator-=(const vector2<type>& operand);
    vector2<type>& operator*=(value_type value);
    vector2<type>& operator/=(value_type value);

    bool operator==(const vector2<type>& operand) const;
    bool operator!=(const vector2<type>& operand) const;

    vector2<type>& negate();

    value_type squared() const;
    value_type sum() const;

    /** Element wise static typecast.
        Works on all indexable types.
    */
    template <class array_type> inline static vector2<type> cast(const array_type& src)
    {
        return vector2<type>(static_cast<type>(src[0]), static_cast<type>(src[1]));
    }

private:
    /** the actual data.
    */
    type data[2];
};

/** Construct the vector that is pointing away from the input 90 degrees to the left.
    \relates vector2
    \ingroup Math
*/
template <class type> inline vector2<type> const left(const vector2<type>& rhs)
{
    return vector2<type>(-rhs[1], rhs[0]);
}

/** Construct the vector that is pointing away from the input 90 degrees to the right.
    \relates vector2
    \ingroup Math
*/
template <class type> inline vector2<type> const right(const vector2<type>& rhs)
{
    return vector2<type>(rhs[1], -rhs[0]);
}

/** Vector dot product of two 2D vectors.
    \relates vector2
    \ingroup Math
*/
template <class type> inline type dot(const vector2<type>& lhs, const vector2<type>& rhs)
{
    return lhs[0] * rhs[0] + lhs[1] * rhs[1];
}

/** Component-wise product of two 2D vectors.
    \relates vector2
    \ingroup Math
*/
template <class type> inline vector2<type> comp(const vector2<type>& lhs, const vector2<type>& rhs)
{
    return vector2<type>(lhs[0] * rhs[0], lhs[1] * rhs[1]);
}

/** Vector addition.
    \relates vector2
    \ingroup Math
*/
template <class type> inline vector2<type> operator+(vector2<type> lhs, const vector2<type>& rhs)
{
    return lhs += rhs;
}

/** Vector substraction.
    \relates vector2
    \ingroup Math
*/
template <class type> inline vector2<type> operator-(vector2<type> lhs, const vector2<type>& rhs)
{
    return lhs -= rhs;
}

/** Scalar product.
    \relates vector2
    \ingroup Math
*/
template <class type> inline vector2<type> operator*(vector2<type> lhs, const type rhs)
{
    return lhs *= rhs;
}

/** Scalar product.
    \relates vector2
    \ingroup Math
*/
template <class type> inline vector2<type> operator*(const type lhs, vector2<type> rhs)
{
    // Implement commutativity
    return rhs *= lhs;
}

/** Scalar division.
    \relates vector2
    \ingroup Math
*/
template <class type> inline vector2<type> operator/(vector2<type> lhs, const type rhs)
{
    // Implement commutativity
    return lhs /= rhs;
}

/** Component-wise min
\relates vector2
\ingroup Math
*/
template <class type> inline vector2<type> min(const vector2<type>& lhs, const vector2<type>& rhs)
{
    return vector2<type>(std::min(lhs[0], rhs[0]), std::min(lhs[1], rhs[1]));
}

/** Component-wise max
\relates vector2
\ingroup Math
*/
template <class type> inline vector2<type> max(const vector2<type>& lhs, const vector2<type>& rhs)
{
    return vector2<type>(std::max(lhs[0], rhs[0]), std::max(lhs[1], rhs[1]));
}

/** Shorthandle for a 2D \c float vector.
    \relates vector2
    \ingroup Math
*/
typedef vector2<float> vector2f;

/** Shorthandle for a 2D \c int vector.
    \relates vector2
    \ingroup Math
*/
typedef vector2<int> vector2i;
}

#include "vector2.inl"

#endif // replay_vector2_hpp
