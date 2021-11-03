#ifndef replay_v4_hpp
#define replay_v4_hpp

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

#include <replay/v2.hpp>
#include <replay/v3.hpp>

namespace replay
{

/** 4-dimensional vector.
    \ingroup Math
*/
template <class T> class v4
{
public:
    /** Element type.
     */
    typedef T value_type;

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

    /** Set all elements to a single value. Default to zero.
     */
    constexpr v4& reset(value_type value = value_type(0));

    /** Assemble a 4D vector by concatenating a 3D vector and a 4th element.
     */
    constexpr v4& reset(v3<T> const& xyz, value_type w);

    /** Assemble a 4D vector by concatenating two 2D vectors.
     */
    constexpr v4& reset(v2<T> const& xy, v2<T> const& zw);

    /** Assemble a 4D vector by concatenating a 2D vector and 2 more values.
     */
    constexpr v4& reset(v2<T> const& xy, value_type z, value_type w);

    /** Set the elements of the vector individually.
        \param x The first component.
        \param y The second component.
        \param z The third component.
        \param w The fourth component.
    */
    constexpr v4& reset(value_type x, value_type y, value_type z, value_type w);

    v4& operator+=(v4 const& rhs);
    v4& operator-=(v4 const& rhs);
    v4& operator*=(T value);
    v4& operator/=(T value);
    v4 operator-() const;

    bool operator==(v4 const& operand) const;
    bool operator!=(v4 const& operand) const;

    /** In-place negate.
        Negates each component of this vector.
    */
    v4& negate();

    T sum() const;
    T squared() const;

    /** Non-initializing constructor.
        Leaves all elements uninitialized.
    */
    explicit v4(uninitialized_tag)
    {
    }

    /** Constructor for user-defined conversions.
        \see convertible_tag
    */
    template <class source_type> v4(const source_type& other, typename convertible_tag<source_type, v4>::type empty = 0)
    {
        *this = convert(other);
    }

    /** Single-value constructor.
        Sets all elements to the given value. Defaults to zero.
    */
    constexpr explicit v4(value_type value = {})
    {
        reset(value);
    }

    /** Assemble a 4D vector by concatenating a 3D vector and a 4th element.
     */
    constexpr v4(v3<T> const& xyz, value_type w)
    {
        reset(xyz, w);
    }

    /** Assemble a 4D vector by concatenating two 2D vectors.
     */
    constexpr v4(v2<T> const& xy, v2<T> const& zw)
    {
        reset(xy, zw);
    }

    /** Create a new vector from seperate values.
        \param x The first component.
        \param y The second component.
        \param z The third component.
        \param w The fourth component.
    */
    constexpr v4(value_type x, value_type y, value_type z, value_type w)
    {
        reset(x, y, z, w);
    }

    /** Assemble a 4D vector by concatenating a 2D vector and 2 more values.
     */
    constexpr v4(v2<T> const& xy, value_type z, value_type w)
    {
        reset(xy, z, w);
    }

    /** Convert an array-like type to a 4D vector.
        The parameter needs to be indexable for at least 4 elements.
    */
    template <class array_type> constexpr static v4 cast(array_type const& src)
    {
        return { static_cast<T>(src[0]), static_cast<T>(src[1]), static_cast<T>(src[2]), static_cast<T>(src[3]) };
    }

private:
    /** The actual data.
     */
    T data[4];
};

/** Scalar dot product of two 4D vectors.
    \relates v4
    \ingroup Math
*/
template <class type> inline type dot(v4<type> const& lhs, v4<type> const& rhs);

/** Component wise multiplication of two 4D vectors.
    \relates v4
    \ingroup Math
*/
template <class type> inline v4<type> comp(v4<type> const& lhs, v4<type> const& rhs);

/** Scalar product.
    \relates v4
    \ingroup Math
*/
template <class type> v4<type> operator*(const type lhs, v4<type> rhs)
{
    return rhs *= lhs;
}

/** Addition.
    \relates v4
    \ingroup Math
*/
template <class type> v4<type> operator+(v4<type> lhs, v4<type> const& rhs)
{
    return lhs += rhs;
}

/** Substraction.
    \relates v4
    \ingroup Math
*/
template <class type> v4<type> operator-(v4<type> lhs, v4<type> const& rhs)
{
    return lhs -= rhs;
}

/** Scalar product.
    \relates v4
    \ingroup Math
*/
template <class type> v4<type> operator*(v4<type> lhs, const type rhs)
{
    return lhs *= rhs;
}

/** Scalar division.
    \relates v4
    \ingroup Math
*/
template <class type> v4<type> operator/(v4<type> lhs, const type rhs)
{
    return lhs /= rhs;
}

template <class T> inline v3<T> perspective_divide(v4<T> v)
{
    return { v[0] / v[3], v[1] / v[3], v[2] / v[3] };
}

} // namespace replay

#include "v4.inl"

#endif // replay_v4_hpp
