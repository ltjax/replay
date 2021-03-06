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

/** In-place addition.
*/
template <class type> inline replay::vector4<type>& replay::vector4<type>::operator+=(vector4<type> const& rhs)
{
    data[0] += rhs.data[0];
    data[1] += rhs.data[1];
    data[2] += rhs.data[2];
    data[3] += rhs.data[3];

    return *this;
}

/** In-place substraction.
*/
template <class type> inline replay::vector4<type>& replay::vector4<type>::operator-=(vector4<type> const& rhs)
{
    data[0] -= rhs.data[0];
    data[1] -= rhs.data[1];
    data[2] -= rhs.data[2];
    data[3] -= rhs.data[3];

    return *this;
}

/** In-place scalar multiplication.
*/
template <class type> inline replay::vector4<type>& replay::vector4<type>::operator*=(const type value)
{
    data[0] *= value;
    data[1] *= value;
    data[2] *= value;
    data[3] *= value;

    return *this;
}

/** In-place scalar division.
*/
template <class type> inline replay::vector4<type>& replay::vector4<type>::operator/=(const type value)
{
    data[0] /= value;
    data[1] /= value;
    data[2] /= value;
    data[3] /= value;

    return *this;
}

template <class type> inline bool replay::vector4<type>::operator==(vector4<type> const& operand) const
{
    return data[0] == operand.data[0] && data[1] == operand.data[1] && data[2] == operand.data[2] && data[3] == operand.data[3];
}

template <class type> inline bool replay::vector4<type>::operator!=(vector4<type> const& operand) const
{
    return data[0] != operand.data[0] || data[1] != operand.data[1] || data[2] != operand.data[2] || data[3] != operand.data[3];
}

template <class type> constexpr replay::vector4<type>& replay::vector4<type>::reset(value_type value)
{
    data[0] = value;
    data[1] = value;
    data[2] = value;
    data[3] = value;

    return *this;
}

template <class type>
constexpr replay::vector4<type>& replay::vector4<type>::reset(vector3<type> const& xyz, const value_type w)
{
    data[0] = xyz[0];
    data[1] = xyz[1];
    data[2] = xyz[2];
    data[3] = w;

    return *this;
}

template <class type>
constexpr replay::vector4<type>& replay::vector4<type>::reset(vector2<type> const& xy, vector2<type> const& zw)
{
    data[0] = xy[0];
    data[1] = xy[1];
    data[2] = zw[0];
    data[3] = zw[1];

    return *this;
}

template <class type>
constexpr replay::vector4<type>&
replay::vector4<type>::reset(vector2<type> const& xy, value_type z, value_type w)
{
    data[0] = xy[0];
    data[1] = xy[1];
    data[2] = z;
    data[3] = w;

    return *this;
}

template <class type>
constexpr replay::vector4<type>&
replay::vector4<type>::reset(const value_type x, const value_type y, const value_type z, const value_type w)
{
    data[0] = x;
    data[1] = y;
    data[2] = z;
    data[3] = w;

    return *this;
}

template <class type> inline replay::vector4<type>& replay::vector4<type>::negate()
{
    data[0] = -data[0];
    data[1] = -data[1];
    data[2] = -data[2];
    data[3] = -data[3];

    return *this;
}

/** Compute the sum of all elements. */
template <class type> inline type replay::vector4<type>::sum() const
{
    return data[0] + data[1] + data[2] + data[3];
}

/** Square this vector using the dot product. */
template <class type> inline type replay::vector4<type>::squared() const
{
    return data[0] * data[0] + data[1] * data[1] + data[2] * data[2] + data[3] * data[3];
}

/**Negated.*/
template <class type> inline replay::vector4<type> replay::vector4<type>::operator-() const
{
    return vector4<type>(-data[0], -data[1], -data[2], -data[3]);
}

template <class type> inline type replay::dot(vector4<type> const& lhs, vector4<type> const& rhs)
{
    return lhs[0] * rhs[0] + lhs[1] * rhs[1] + lhs[2] * rhs[2] + lhs[3] * rhs[3];
}

template <class type> inline replay::vector4<type> replay::comp(vector4<type> const& lhs, vector4<type> const& rhs)
{
    return replay::vector4<type>(lhs[0] * rhs[0], lhs[1] * rhs[1], lhs[2] * rhs[2], lhs[3] * rhs[3]);
}
