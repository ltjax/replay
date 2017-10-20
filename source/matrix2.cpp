/*
replay
Software Library

Copyright (c) 2011 Marius Elvert

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

#include <cmath>
#include <replay/matrix2.hpp>

replay::matrix2::matrix2(float diagonal)
{
    data[0] = diagonal;
    data[1] = 0.f;
    data[2] = 0.f;
    data[3] = diagonal;
}

replay::matrix2::matrix2(const vector2f& a, const vector2f& b)
{
    data[0] = a[0];
    data[1] = a[1];
    data[2] = b[0];
    data[3] = b[1];
}

replay::matrix2::matrix2(float m11, float m21, float m12, float m22)
{
    data[0] = m11;
    data[2] = m21;
    data[1] = m12;
    data[3] = m22;
}

replay::matrix2& replay::matrix2::set(float m11, float m21, float m12, float m22)
{
    data[0] = m11;
    data[2] = m21;
    data[1] = m12;
    data[3] = m22;

    return *this;
}

replay::matrix2 replay::matrix2::make_identity()
{
    return matrix2(1.f, 0.f, 0.f, 1.f);
}

replay::matrix2 replay::matrix2::make_rotation(float angle)
{
    const float sin = std::sin(angle);
    const float cos = std::cos(angle);

    return matrix2(cos, -sin, sin, cos);
}

replay::matrix2 replay::matrix2::make_scale(const vector2f& scale)
{
    return matrix2(scale[0], 0.f, 0.f, scale[1]);
}

const replay::vector2f replay::matrix2::operator*(const vector2f& v) const
{
    return vector2f(data[0] * v[0] + data[2] * v[1], data[1] * v[0] + data[3] * v[1]);
}

const replay::matrix2 replay::matrix2::operator*(const matrix2& other) const
{
    matrix2 result((uninitialized_tag()));
    return multiply(*this, other, result);
}

replay::matrix2& replay::matrix2::multiply(const matrix2& a, const matrix2& b, matrix2& result)
{
    result.data[0] = a.data[0] * b.data[0] + a.data[2] * b.data[1];
    result.data[1] = a.data[1] * b.data[0] + a.data[3] * b.data[1];
    result.data[2] = a.data[0] * b.data[2] + a.data[2] * b.data[3];
    result.data[3] = a.data[1] * b.data[2] + a.data[3] * b.data[3];

    return result;
}

replay::matrix2& replay::matrix2::operator*=(float rhs)
{
    for (int i = 0; i < 4; ++i)
        data[i] *= rhs;

    return *this;
}

replay::matrix2& replay::matrix2::operator*=(const matrix2& Matrix)
{
    return ((*this) = ((*this) * Matrix));
}

double replay::matrix2::determinant() const
{
    return static_cast<double>(data[0]) * data[3] - data[1] * data[2];
}

bool replay::matrix2::invert(double epsilon)
{
    const double d(this->determinant());

    if (std::abs(d) <= epsilon)
        return false;

    set(static_cast<float>(data[3] / d), -static_cast<float>(data[2] / d), -static_cast<float>(data[1] / d),
        static_cast<float>(data[0] / d));

    return true;
}
