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

#include <algorithm>
#include <cmath>
#include <replay/matrix3.hpp>
#include <replay/quaternion.hpp>

/** Multiply two matrices.
    \note Uses 27 mults and 18 adds.
*/
replay::matrix3& replay::matrix3::multiply(const matrix3& a, const matrix3& b, matrix3& result)
{
    result[0] = a.data[0] * b.data[0] + a.data[3] * b.data[1] + a.data[6] * b.data[2];
    result[1] = a.data[1] * b.data[0] + a.data[4] * b.data[1] + a.data[7] * b.data[2];
    result[2] = a.data[2] * b.data[0] + a.data[5] * b.data[1] + a.data[8] * b.data[2];

    result[3] = a.data[0] * b.data[3] + a.data[3] * b.data[4] + a.data[6] * b.data[5];
    result[4] = a.data[1] * b.data[3] + a.data[4] * b.data[4] + a.data[7] * b.data[5];
    result[5] = a.data[2] * b.data[3] + a.data[5] * b.data[4] + a.data[8] * b.data[5];

    result[6] = a.data[0] * b.data[6] + a.data[3] * b.data[7] + a.data[6] * b.data[8];
    result[7] = a.data[1] * b.data[6] + a.data[4] * b.data[7] + a.data[7] * b.data[8];
    result[8] = a.data[2] * b.data[6] + a.data[5] * b.data[7] + a.data[8] * b.data[8];

    return result;
}

/** Multiply a vector by a matrix.
    \note 9 mults, 6 adds
*/
replay::vector3f& replay::matrix3::multiply(const matrix3& a, const vector3f& v, vector3f& result)
{
    result[0] = a.data[0] * v[0] + a.data[3] * v[1] + a.data[6] * v[2];
    result[1] = a.data[1] * v[0] + a.data[4] * v[1] + a.data[7] * v[2];
    result[2] = a.data[2] * v[0] + a.data[5] * v[1] + a.data[8] * v[2];

    return result;
}

/** Multiply two matrices.
*/
const replay::matrix3 replay::matrix3::operator*(const matrix3& other) const
{
    matrix3 result((uninitialized_tag()));

    multiply(*this, other, result);

    return result;
}

/** Set the matrix by it's individual components.
*/
replay::matrix3&
replay::matrix3::set(float m11, float m21, float m31, float m12, float m22, float m32, float m13, float m23, float m33)
{
    data[0] = m11;
    data[3] = m21;
    data[6] = m31;
    data[1] = m12;
    data[4] = m22;
    data[7] = m32;
    data[2] = m13;
    data[5] = m23;
    data[8] = m33;

    return *this;
}

/** Multiply a vector by a matrix.
*/
const replay::vector3f replay::matrix3::operator*(const vector3f& operand) const
{
    return vector3f(data[0] * operand[0] + data[3] * operand[1] + data[6] * operand[2],
                    data[1] * operand[0] + data[4] * operand[1] + data[7] * operand[2],
                    data[2] * operand[0] + data[5] * operand[1] + data[8] * operand[2]);
}

/**	transposed multiplication.
    transposed( operand ) * this
*/
const replay::vector3f replay::matrix3::operator|(const vector3f& operand) const
{
    return vector3f(data[0] * operand[0] + data[1] * operand[1] + data[2] * operand[2],
                    data[3] * operand[0] + data[4] * operand[1] + data[5] * operand[2],
                    data[6] * operand[0] + data[7] * operand[1] + data[8] * operand[2]);
}

/** Assign a quaternion to this matrix.
*/
replay::matrix3& replay::matrix3::operator=(const quaternion& q)
{
    data[0] = 1.f - 2.f * (q.y * q.y + q.z * q.z);
    data[1] = 2.f * (q.x * q.y + q.z * q.w);
    data[2] = 2.f * (q.x * q.z - q.y * q.w);

    data[3] = 2.f * (q.x * q.y - q.z * q.w);
    data[4] = 1.f - 2.f * (q.x * q.x + q.z * q.z);
    data[5] = 2.f * (q.y * q.z + q.x * q.w);

    data[6] = 2.f * (q.x * q.z + q.y * q.w);
    data[7] = 2.f * (q.y * q.z - q.x * q.w);
    data[8] = 1.f - 2.f * (q.x * q.x + q.y * q.y);

    return (*this);
}

/** Inplace multiply two matrices.
    \note This creates another temporary matrix internally.
*/
replay::matrix3& replay::matrix3::operator*=(const matrix3& other)
{
    (*this) = ((*this) * other);
    return *this;
}

/** Inplace multiply a matrix by a scalar.
*/
replay::matrix3& replay::matrix3::operator*=(const float f)
{
    for (std::size_t i = 0; i < 9; ++i)
        data[i] *= f;

    return *this;
}

/** Inplace add two matrices.
*/
replay::matrix3& replay::matrix3::operator+=(const matrix3& m)
{
    for (std::size_t i = 0; i < 9; ++i)
        data[i] += m.data[i];

    return *this;
}

/** Create an uninitialized matrix.
    \note Contents at this point are undefined!
*/
replay::matrix3::matrix3(uninitialized_tag)
{
}

/** Create a uniform scaling matrix.
*/
replay::matrix3::matrix3(float d)
{
    data[0] = d;
    data[3] = 0.f;
    data[6] = 0.f;
    data[1] = 0.f;
    data[4] = d;
    data[7] = 0.f;
    data[2] = 0.f;
    data[5] = 0.f;
    data[8] = d;
}

/** Create a matrix from the individual components.
*/
replay::matrix3::matrix3(
    float m11, float m21, float m31, float m12, float m22, float m32, float m13, float m23, float m33)
{
    data[0] = m11;
    data[3] = m21;
    data[6] = m31;
    data[1] = m12;
    data[4] = m22;
    data[7] = m32;
    data[2] = m13;
    data[5] = m23;
    data[8] = m33;
}

/** Column wise init.
*/
replay::matrix3::matrix3(const vector3f& a, const vector3f& b, const vector3f& c)
{
    data[0] = a[0];
    data[3] = b[0];
    data[6] = c[0];
    data[1] = a[1];
    data[4] = b[1];
    data[7] = c[1];
    data[2] = a[2];
    data[5] = b[2];
    data[8] = c[2];
}

/** Column wise set.
*/
void replay::matrix3::set(const vector3f& a, const vector3f& b, const vector3f& c)
{
    data[0] = a[0];
    data[3] = b[0];
    data[6] = c[0];
    data[1] = a[1];
    data[4] = b[1];
    data[7] = c[1];
    data[2] = a[2];
    data[5] = b[2];
    data[8] = c[2];
}

/** Assign a rotational quaternion.
*/
replay::matrix3::matrix3(const quaternion& q)
{
    (*this) = q;
}

/** Set the identity matrix.
*/
void replay::matrix3::set_identity()
{
    data[0] = 1.0f;
    data[3] = 0.0f;
    data[6] = 0.0f;
    data[1] = 0.0f;
    data[4] = 1.0f;
    data[7] = 0.0f;
    data[2] = 0.0f;
    data[5] = 0.0f;
    data[8] = 1.0f;
}

/** Set rotational matrix.
*/
void replay::matrix3::set_rotation(const float angle, const vector3f& axis)
{
    const quaternion rotation(angle, axis);

    *this = rotation;
}

/** Set a rotation around the x axis.
*/
replay::matrix3& replay::matrix3::set_rotation_x(const float angle)
{
    const float cos = std::cos(angle);
    const float sin = std::sin(angle);

    return set(1.f, 0.f, 0.f, 0.f, cos, -sin, 0.f, sin, cos);
}

/** Set a rotation around the y axis.
*/
replay::matrix3& replay::matrix3::set_rotation_y(const float angle)
{
    const float cos = std::cos(angle);
    const float sin = std::sin(angle);

    return set(cos, 0.f, -sin, 0.f, 1.f, 0.f, sin, 0.f, cos);
}

/** Set a rotation around the z axis.
*/
replay::matrix3& replay::matrix3::set_rotation_z(const float angle)
{
    const float cos = std::cos(angle);
    const float sin = std::sin(angle);

    return set(cos, -sin, 0.f, sin, cos, 0.f, 0.f, 0.f, 1.f);
}

/** Set a scale matrix.
*/
void replay::matrix3::set_scale(const float x, const float y, const float z)
{
    data[0] = x;
    data[3] = 0.0f;
    data[6] = 0.0f;
    data[1] = 0.0f;
    data[4] = y;
    data[7] = 0.0f;
    data[2] = 0.0f;
    data[5] = 0.0f;
    data[8] = z;
}

/** Set a scale matrix.
*/
void replay::matrix3::set_scale(const vector3f& v)
{
    data[0] = v[0];
    data[3] = 0.0f;
    data[6] = 0.0f;
    data[1] = 0.0f;
    data[4] = v[1];
    data[7] = 0.0f;
    data[2] = 0.0f;
    data[5] = 0.0f;
    data[8] = v[2];
}

/** Concaternate this matrix with a rotational matrix.
*/
void replay::matrix3::rotate(matrix3& m, const float angle, const vector3f& axis)
{
    matrix3 temp((replay::uninitialized_tag()));

    temp.set_rotation(angle, axis);

    m *= temp;
}

/** Concaternate this matrix with a scale matrix.
*/
void replay::matrix3::scale(matrix3& m, const float x, const float y, const float z)
{
    matrix3 temp((replay::uninitialized_tag()));

    temp.set_scale(x, y, z);

    m *= temp;
}

/** Concaternate this matrix with a scale matrix.
*/
void replay::matrix3::scale(matrix3& m, vector3f const& v)
{
    matrix3 temp((replay::uninitialized_tag()));

    temp.set_scale(v);

    m *= temp;
}

/** Transpose the matrix.
*/
replay::matrix3& replay::matrix3::transpose()
{
    std::swap(data[1], data[3]);
    std::swap(data[5], data[7]);
    std::swap(data[2], data[6]);

    return *this;
}

/** Invert this matrix.
*/
replay::matrix3& replay::matrix3::invert()
{
    *this = this->inverted();

    return *this;
}

/** Compute the determinat.
*/
float replay::matrix3::determinant() const
{
    return data[0] * (data[4] * data[8] - data[5] * data[7]) + data[3] * (data[2] * data[7] - data[1] * data[8]) +
           data[6] * (data[1] * data[5] - data[2] * data[4]);
}

/** Get the inverted matrix.
*/
const replay::matrix3 replay::matrix3::inverted() const
{
    float const d = determinant();

    return matrix3((data[4] * data[8] - data[7] * data[5]) / d, -(data[3] * data[8] - data[5] * data[6]) / d,
                   (data[3] * data[7] - data[4] * data[6]) / d, -(data[1] * data[8] - data[7] * data[2]) / d,
                   (data[0] * data[8] - data[2] * data[6]) / d, -(data[0] * data[7] - data[1] * data[6]) / d,
                   (data[1] * data[5] - data[2] * data[4]) / d, -(data[0] * data[5] - data[2] * data[3]) / d,
                   (data[0] * data[4] - data[3] * data[1]) / d);
}

/** Get the transposed matrix.
*/
const replay::matrix3 replay::matrix3::transposed() const
{
    return matrix3(data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8]);
}
