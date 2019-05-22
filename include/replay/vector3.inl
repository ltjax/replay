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

/** Set this objects components.
    \param x The first component.
    \param y The second component.
    \param z The third component.
*/
template <class type>
inline replay::vector3<type>& replay::vector3<type>::reset(value_type x, value_type y, value_type z)
{
    data[0] = x;
    data[1] = y;
    data[2] = z;

    return *this;
}

/** Set all components.
    \param value Value to set the vector to.
*/
template <class type> inline replay::vector3<type>& replay::vector3<type>::reset(value_type value)
{
    data[0] = value;
    data[1] = value;
    data[2] = value;

    return *this;
}

/** Negation.
*/
template <class type> inline replay::vector3<type> replay::vector3<type>::operator-() const
{
    return replay::vector3<type>(-data[0], -data[1], -data[2]);
}

/** In-place addition.
*/
template <class type>
inline replay::vector3<type>& replay::vector3<type>::operator+=(const replay::vector3<type>& operand)
{
    data[0] += operand.data[0];
    data[1] += operand.data[1];
    data[2] += operand.data[2];

    return *this;
}

/** In-place subtraction.
*/
template <class type>
inline replay::vector3<type>& replay::vector3<type>::operator-=(const replay::vector3<type>& operand)
{
    data[0] -= operand.data[0];
    data[1] -= operand.data[1];
    data[2] -= operand.data[2];

    return *this;
}

/** In-place scalar multiplication.
*/
template <class type> inline replay::vector3<type>& replay::vector3<type>::operator*=(const type& operand)
{
    data[0] *= operand;
    data[1] *= operand;
    data[2] *= operand;

    return *this;
}

/** In-place scalar division.
*/
template <class type> inline replay::vector3<type>& replay::vector3<type>::operator/=(const type& operand)
{
    data[0] /= operand;
    data[1] /= operand;
    data[2] /= operand;

    return *this;
}

/** Test for equality.
*/
template <class type> inline bool replay::vector3<type>::operator==(const vector3<type>& operand) const
{
    return data[0] == operand[0] && data[1] == operand[1] && data[2] == operand[2];
}

/** Test for unequality.
*/
template <class type> inline bool replay::vector3<type>::operator!=(const vector3<type>& operand) const
{
    return data[0] != operand[0] || data[1] != operand[1] || data[2] != operand[2];
}

template <class type> inline replay::vector3<type>& replay::vector3<type>::negate()
{
    data[0] = -data[0];
    data[1] = -data[1];
    data[2] = -data[2];

    return *this;
}

/** Square. Square this vector using the dot product. */
template <class type> inline type replay::vector3<type>::squared() const
{
    return data[0] * data[0] + data[1] * data[1] + data[2] * data[2];
}

/** Sum. Return a sum of all elements. */
template <class type> inline type replay::vector3<type>::sum() const
{
    return data[0] + data[1] + data[2];
}

template <class type> inline replay::vector3<type> replay::cross(const vector3<type>& lhs, const vector3<type>& rhs)
{
    return replay::vector3<type>(lhs[1] * rhs[2] - lhs[2] * rhs[1], lhs[2] * rhs[0] - lhs[0] * rhs[2],
                                 lhs[0] * rhs[1] - lhs[1] * rhs[0]);
}

template <class type> inline type replay::dot(const vector3<type>& lhs, const vector3<type>& rhs)
{
    return lhs[0] * rhs[0] + lhs[1] * rhs[1] + lhs[2] * rhs[2];
}

template <class type> inline replay::vector3<type> replay::comp(const vector3<type>& lhs, const vector3<type>& rhs)
{
    return vector3<type>(lhs[0] * rhs[0], lhs[1] * rhs[1], lhs[2] * rhs[2]);
}
