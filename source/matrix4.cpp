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
#include <replay/math.hpp>
#include <replay/matrix3.hpp>
#include <replay/matrix4.hpp>
#include <replay/plane3.hpp>
#include <replay/quaternion.hpp>

namespace
{
inline float det3(float a, float b, float c, float d, float e, float f, float g, float h, float i)
{

    float r = a * (e * i - f * h);
    r += b * (f * g - d * i);
    r += c * (d * h - e * g);

    return r;
}
} // namespace
replay::plane3 operator*(replay::plane3 const& p, replay::matrix4 const& m)
{
    replay::plane3 result;

    result.normal[0] = m[0] * p.normal[0] + m[1] * p.normal[1] + m[2] * p.normal[2] + m[3] * p.d;
    result.normal[1] = m[4] * p.normal[0] + m[5] * p.normal[1] + m[6] * p.normal[2] + m[7] * p.d;
    result.normal[2] = m[8] * p.normal[0] + m[9] * p.normal[1] + m[10] * p.normal[2] + m[11] * p.d;
    result.d = m[12] * p.normal[0] + m[13] * p.normal[1] + m[14] * p.normal[2] + m[15] * p.d;

    return result;
}

replay::matrix4::matrix4(uninitialized_tag)
{
}

replay::matrix4::matrix4(float d)
{
    set(d, 0.f, 0.f, 0.f, 0.f, d, 0.f, 0.f, 0.f, 0.f, d, 0.f, 0.f, 0.f, 0.f, d);
}

replay::matrix4::matrix4(v4<float> const& d)
{
    set(d[0], 0.f, 0.f, 0.f, 0.f, d[1], 0.f, 0.f, 0.f, 0.f, d[2], 0.f, 0.f, 0.f, 0.f, d[3]);
}

/** Create a matrix from a rotational part and an optional offset.
 */
replay::matrix4::matrix4(quaternion const& rotation, v3<float> const& offset)
{
    *this = rotation;

    data[12] = offset[0];
    data[13] = offset[1];
    data[14] = offset[2];
}

/** Create a matrix from a 3D matrix part and an optional offset.
 */
replay::matrix4::matrix4(matrix3 const& rotation, v3<float> const& offset)
{
    // Copy the rotational part
    for (std::size_t i = 0; i < 3; ++i)
        for (std::size_t j = 0; j < 3; ++j)
            operator()(i, j) = rotation(i, j);

    // Copy the offset
    for (std::size_t i = 0; i < 3; ++i)
        operator()(i, 3) = offset[i];

    // Setup the affine row
    data[3] = 0.f;
    data[7] = 0.f;
    data[11] = 0.f;
    data[15] = 1.f;
}

/** Create a matrix from scale and offset.
 */
replay::matrix4::matrix4(v3<float> const& s, v3<float> const& o)
{
    set(s[0], 0.f, 0.f, o[0], 0.f, s[1], 0.f, o[1], 0.f, 0.f, s[2], o[2], 0.f, 0.f, 0.f, 1.f);
}

/** Create a matrix from a rotational part, sign and an offset.
 */
replay::matrix4::matrix4(quaternion const& q, v3<float> const& offset, float sign)
{
    *this = q;

    for (unsigned int i = 0; i < 9; ++i)
        math::mult_ref_by_sign(data[(i / 3) * 4 + (i % 3)], sign);

    data[12] = offset[0];
    data[13] = offset[1];
    data[14] = offset[2];
}

/** Create a matrix from the given components.
 */
replay::matrix4::matrix4(float a11,
                         float a21,
                         float a31,
                         float a41,
                         float a12,
                         float a22,
                         float a32,
                         float a42,
                         float a13,
                         float a23,
                         float a33,
                         float a43,
                         float a14,
                         float a24,
                         float a34,
                         float a44)
{
    data[0] = a11;
    data[4] = a21;
    data[8] = a31;
    data[12] = a41;
    data[1] = a12;
    data[5] = a22;
    data[9] = a32;
    data[13] = a42;
    data[2] = a13;
    data[6] = a23;
    data[10] = a33;
    data[14] = a43;
    data[3] = a14;
    data[7] = a24;
    data[11] = a34;
    data[15] = a44;
}

/** Set a matrix from given components.
 */
replay::matrix4& replay::matrix4::set(float a11,
                                      float a21,
                                      float a31,
                                      float a41,
                                      float a12,
                                      float a22,
                                      float a32,
                                      float a42,
                                      float a13,
                                      float a23,
                                      float a33,
                                      float a43,
                                      float a14,
                                      float a24,
                                      float a34,
                                      float a44)
{
    data[0] = a11;
    data[4] = a21;
    data[8] = a31;
    data[12] = a41;
    data[1] = a12;
    data[5] = a22;
    data[9] = a32;
    data[13] = a42;
    data[2] = a13;
    data[6] = a23;
    data[10] = a33;
    data[14] = a43;
    data[3] = a14;
    data[7] = a24;
    data[11] = a34;
    data[15] = a44;

    return *this;
}

/** Set an identity matrix.
 */
replay::matrix4 replay::matrix4::identity()
{
    return from_scale({ 1.f, 1.f, 1.f });
}

/** Create a scale matrix.
 */
replay::matrix4 replay::matrix4::from_scale(v3<float> const& scale)
{
    return from_scale_and_translation(scale, v3<float>{ 0.f });
}

/** Create a translation matrix.
 */
replay::matrix4 replay::matrix4::from_translation(v3<float> const& translation)
{
    return from_scale_and_translation(v3<float>{ 1.f }, translation);
}

/** Create a scale & translation matrix.
 */
replay::matrix4 replay::matrix4::from_scale_and_translation(v3<float> const& scale, v3<float> const& translation)
{
    return { scale[0], 0.f, 0.f, translation[0], 0.f, scale[1], 0.f, translation[1], 0.f, 0.f, scale[2], translation[2], 0.f, 0.f, 0.f, 1.f };
}

/** Scale the given transformation.
 */
replay::matrix4& replay::matrix4::scale(const float x, const float y, const float z)
{
    data[0] *= x;
    data[4] *= y;
    data[8] *= z;
    data[1] *= x;
    data[5] *= y;
    data[9] *= z;
    data[2] *= x;
    data[6] *= y;
    data[10] *= z;

    return *this;
}

/** Scale the given transformation.
 */
replay::matrix4& replay::matrix4::scale(v3<float> const& v)
{
    data[0] *= v[0];
    data[4] *= v[1];
    data[8] *= v[2];
    data[1] *= v[0];
    data[5] *= v[1];
    data[9] *= v[2];
    data[2] *= v[0];
    data[6] *= v[1];
    data[10] *= v[2];

    return *this;
}

/** Translate by the given offset.
 */
replay::matrix4& replay::matrix4::translate(v3<float> const& rhs)
{
    v3<float> rhs0(rhs[0] * data[0] + rhs[1] * data[4] + rhs[2] * data[8],
                   rhs[0] * data[1] + rhs[1] * data[5] + rhs[2] * data[9],
                   rhs[0] * data[2] + rhs[1] * data[6] + rhs[2] * data[10]);

    data[12] += rhs0[0];
    data[13] += rhs0[1];
    data[14] += rhs0[2];

    return *this;
}

/** Translate by the given offset.
 */
replay::matrix4& replay::matrix4::translate(const float x, const float y, const float z)
{
    return translate(v3<float>(x, y, z));
}

/** Set a rotational matrix.
 */
replay::matrix4 replay::matrix4::from_rotation(float angle, v3<float> const& axis)
{
    return matrix4{quaternion(angle, axis)};
}

/** Invert the matrix if it is orthogonal.
 */
const replay::matrix4 replay::matrix4::inverted_orthogonal() const
{
    return matrix4(data[0], data[1], data[2], -(data[12] * data[0] + data[13] * data[1] + data[14] * data[2]), data[4],
                   data[5], data[6], -(data[12] * data[4] + data[13] * data[5] + data[14] * data[6]), data[8], data[9],
                   data[10], -(data[12] * data[8] + data[13] * data[9] + data[14] * data[10]), 0.f, 0.f, 0.f, 1.f);
}

/** Assign a quaternion.
 */
replay::matrix4& replay::matrix4::operator=(quaternion const& q)
{
    data[0] = 1.f - 2.f * (q.y * q.y + q.z * q.z);
    data[1] = 2.f * (q.x * q.y + q.w * q.z);
    data[2] = 2.f * (q.x * q.z - q.w * q.y);
    data[3] = 0.f;

    data[4] = 2.f * (q.x * q.y - q.w * q.z);
    data[5] = 1.f - 2.f * (q.x * q.x + q.z * q.z);
    data[6] = 2.f * (q.y * q.z + q.w * q.x);
    data[7] = 0.f;

    data[8] = 2.f * (q.x * q.z + q.w * q.y);
    data[9] = 2.f * (q.y * q.z - q.w * q.x);
    data[10] = 1.f - 2.f * (q.x * q.x + q.y * q.y);
    data[11] = 0.f;

    data[12] = 0.f;
    data[13] = 0.f;
    data[14] = 0.f;
    data[15] = 1.f;

    return (*this);
}

/** Multiply the matrix by a scalar.
 */
const replay::matrix4 replay::matrix4::operator*(float rhs) const
{
    matrix4 result = *this;
    return result *= rhs;
}

/** Inplace add two matrices component wise.
 */
replay::matrix4& replay::matrix4::operator+=(const matrix4& other)
{
    for (std::size_t i = 0; i < 16; ++i)
        data[i] += other[i];

    return *this;
}

/** Add two matrices component wise.
 */
const replay::matrix4 replay::matrix4::operator+(const matrix4& other) const
{
    matrix4 result(*this);
    result += other;
    return result;
}

/** Set a rotation around the x axis.
 */
replay::matrix4 replay::matrix4::from_rotation_x(float angle)
{
    const float cos = std::cos(angle);
    const float sin = std::sin(angle);

    return {1.f, 0.f, 0.f, 0.f, 0.f, cos, -sin, 0.f, 0.f, sin, cos, 0.f, 0.f, 0.f, 0.f, 1.f};
}

/** Set a rotation around the y axis.
 */
replay::matrix4 replay::matrix4::from_rotation_y(float angle)
{
    const float cos = std::cos(angle);
    const float sin = std::sin(angle);

    return {cos, 0.f, -sin, 0.f, 0.f, 1.f, 0.f, 0.f, sin, 0.f, cos, 0.f, 0.f, 0.f, 0.f, 1.f};
}

/** Set a rotation around the z axis.
 */
replay::matrix4 replay::matrix4::from_rotation_z(float angle)
{
    const float cos = std::cos(angle);
    const float sin = std::sin(angle);

    return {cos, -sin, 0.f, 0.f, sin, cos, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f};
}

/** Set a column in the matrix.
 */
void replay::matrix4::set_column(unsigned int i, v4<float> const& column)
{
    float* const dst = data + (i * 4);
    dst[0] = column[0];
    dst[1] = column[1];
    dst[2] = column[2];
    dst[3] = column[3];
}

/** Swap two columns.
 */
void replay::matrix4::swap_column(unsigned int i, unsigned int j)
{
    float* const column0 = data + (i * 4);
    float* const column1 = data + (j * 4);

    for (unsigned int k = 0; k < 4; ++k)
        std::swap(column0[k], column1[k]);
}

/** Swap two rows.
 */
void replay::matrix4::swap_row(unsigned int i, unsigned int j)
{
    for (unsigned int k = 0; k < 4; ++k)
        std::swap(data[k * 4 + i], data[k * 4 + j]);
}

/** Compute a determinat.
 */
float replay::matrix4::determinant() const
{
    unsigned int index = 0;

    // find the first column that has the 4th element enequal to zero
    while (index < 4 && math::fuzzy_zero(data[index * 4 + 3]))
        ++index;

    if (index == 4)
        return 0.f;

    if (index == 3)
    {
        const float minor = det3(data[0], data[4], data[8], data[1], data[5], data[9], data[2], data[6], data[10]);

        return data[15] * minor;
    }
    else // index is 0..2
    {
        matrix4 temp(*this);
        temp.swap_column(index, 3);
        v4<float> const last = temp.get_column(3);

        while (index < 3)
        {
            float factor = temp.data[index * 4 + 3];
            if (!math::fuzzy_zero(factor))
            {
                factor = -(factor / last[3]);
                temp.set_column(index, temp.get_column(index) + factor * last);
            }
            ++index;
        }

        const float minor = det3(temp[0], temp[4], temp[8], temp[1], temp[5], temp[9], temp[2], temp[6], temp[10]);

        // the swap negates the result
        return -last[3] * minor;
    }
}

/** Set a row in a matrix.
 */
void replay::matrix4::set_row(unsigned int i, v4<float> const& row)
{
    data[i] = row[0];
    data[i + 4] = row[1];
    data[i + 8] = row[2];
    data[i + 12] = row[3];
}

/** Transpose the matrix.
 */
void replay::matrix4::transpose()
{
    std::swap(data[1], data[4]);
    std::swap(data[2], data[8]);
    std::swap(data[3], data[12]);
    std::swap(data[6], data[9]);
    std::swap(data[7], data[13]);
    std::swap(data[11], data[14]);
}

/** Get a column of the matrix.
 */
const replay::v4<float> replay::matrix4::get_column(unsigned int i) const
{
    return v4<float>::cast(data + (i * 4));
}

/** Get a row in the matrix.
 */
const replay::v4<float> replay::matrix4::get_row(unsigned int i) const
{
    return v4<float>(data[i], data[4 + i], data[8 + i], data[12 + i]);
}

/** Compare two matrices for equality.
    Performs and element-wise comparison.
*/
bool replay::matrix4::operator==(const matrix4& rhs) const
{
    for (size_t i = 0; i < 16; ++i)
        if (data[i] != rhs[i])
            return false;

    return true;
}
