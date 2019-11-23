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

#ifndef replay_matrix4_hpp
#define replay_matrix4_hpp

#include <cstddef>
#include <replay/common.hpp>
#include <replay/vector3.hpp>
#include <replay/vector4.hpp>

namespace replay
{

class quaternion;
class plane3;
class matrix3;

/** 4x4 float matrix.
    uses opengl-like column major internal format:
    0	4	8	12
    1	5	9	13
    2	6	10	14
    3	7	11	15
    \ingroup Math
*/
class matrix4
{
public:
    /** Create an uninitialized matrix.
     */
    explicit matrix4(uninitialized_tag);

    /** Initialize the diagonal.
        \param d Value for the diagonal elements
    */
    explicit matrix4(float diagonal);

    /** Initialize the diagonal.
        \param v 3D vector to initialize the diagonal elements
    */
    explicit matrix4(v4<float> const& diagonal);

    explicit matrix4(const matrix3& rotation, v3<float> const& offset = v3<float>(0.f));
    explicit matrix4(quaternion const& rotation, v3<float> const& offset = v3<float>(0.f));
    explicit matrix4(v3<float> const& scale, v3<float> const& offset = v3<float>(0.f));
    matrix4(quaternion const& rotation, v3<float> const& offset, float sign);
    matrix4(float a11,
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
            float a44);

    /** Constructor for user-defined conversions.
        \see convertible_tag
    */
    template <class source_type>
    matrix4(const source_type& other, typename convertible_tag<source_type, matrix4>::type empty = 0)
    {
        *this = convert(other);
    }

    float* ptr();
    const float* ptr() const;

    /** Index access operator.
     */
    template <class index_type> float& operator[](const index_type i)
    {
        return data[i];
    }

    /** Index access operator.
     */
    template <class index_type> const float& operator[](const index_type i) const
    {
        return data[i];
    }

    matrix4& set(float a11,
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
                 float a44);

    static matrix4 identity();
    static matrix4 from_rotation_x(float angle);
    static matrix4 from_rotation_y(float angle);
    static matrix4 from_rotation_z(float angle);

    static matrix4 from_rotation(float angle, v3<float> const& axis);
    static matrix4 from_scale(v3<float> const& scale);
    static matrix4 from_translation(v3<float> const& translation);

    void set_column(unsigned int i, v4<float> const& column);
    void set_row(unsigned int i, v4<float> const& row);

    void swap_column(unsigned int i, unsigned int j);
    void swap_row(unsigned int i, unsigned int j);

    v4<float> const get_column(unsigned int i) const;
    v4<float> const get_row(unsigned int i) const;

    matrix4 const inverted_orthogonal() const;
    float determinant() const;

    void transpose();

    matrix4& scale(float x, float y, float z);
    matrix4& scale(v3<float> const& v);

    matrix4& translate(float x, float y, float z);
    matrix4& translate(v3<float> const& rhs);

    static void multiply(matrix4 const& a, matrix4 const& b, matrix4& result);

    /** Get matrix elements by their indices.
     */
    float& operator()(std::size_t row, std::size_t column)
    {
        return data[(column << 2) + row];
    }

    /** Get matrix elements by their indices.
     */
    float operator()(std::size_t row, std::size_t column) const
    {
        return data[(column << 2) + row];
    }

    matrix4 const operator*(matrix4 const& other) const;
    matrix4 const operator+(matrix4 const& other) const;
    matrix4 const operator*(float rhs) const;

    v4<float> const operator*(v4<float> const& other) const;
    v3<float> const operator*(v3<float> const& other) const;

    v4<float> const multiply3(v3<float> const& rhs) const;

    matrix4& operator=(quaternion const& rotation);
    matrix4& operator*=(matrix4 const& other);
    matrix4& operator*=(float rhs);
    matrix4& operator+=(matrix4 const& other);

    bool operator==(matrix4 const& rhs) const;
    bool operator!=(matrix4 const& rhs) const
    {
        return !(operator==(rhs));
    }

private:
    float data[16];
};

plane3 operator*(plane3 const& p, matrix4 const& m);

/** Left-side scalar multiplication for matrices.
 */
inline matrix4 operator*(float lhs, matrix4 const& rhs)
{
    // Commutative in this case!
    return rhs * lhs;
}

/** Get a pointer to the elements.
 */
inline float* matrix4::ptr()
{
    return data;
}

/** Get a pointer to the elements.
 */
inline const float* matrix4::ptr() const
{
    return data;
}

/** Multiply two matrices.
 */
inline void matrix4::multiply(matrix4 const& a, matrix4 const& b, matrix4& result)
{
    result.data[0] = b.data[0] * a.data[0] + b.data[1] * a.data[4] + b.data[2] * a.data[8] + b.data[3] * a.data[12];
    result.data[1] = b.data[0] * a.data[1] + b.data[1] * a.data[5] + b.data[2] * a.data[9] + b.data[3] * a.data[13];
    result.data[2] = b.data[0] * a.data[2] + b.data[1] * a.data[6] + b.data[2] * a.data[10] + b.data[3] * a.data[14];
    result.data[3] = b.data[0] * a.data[3] + b.data[1] * a.data[7] + b.data[2] * a.data[11] + b.data[3] * a.data[15];

    result.data[4] = b.data[4] * a.data[0] + b.data[5] * a.data[4] + b.data[6] * a.data[8] + b.data[7] * a.data[12];
    result.data[5] = b.data[4] * a.data[1] + b.data[5] * a.data[5] + b.data[6] * a.data[9] + b.data[7] * a.data[13];
    result.data[6] = b.data[4] * a.data[2] + b.data[5] * a.data[6] + b.data[6] * a.data[10] + b.data[7] * a.data[14];
    result.data[7] = b.data[4] * a.data[3] + b.data[5] * a.data[7] + b.data[6] * a.data[11] + b.data[7] * a.data[15];

    result.data[8] = b.data[8] * a.data[0] + b.data[9] * a.data[4] + b.data[10] * a.data[8] + b.data[11] * a.data[12];
    result.data[9] = b.data[8] * a.data[1] + b.data[9] * a.data[5] + b.data[10] * a.data[9] + b.data[11] * a.data[13];
    result.data[10] = b.data[8] * a.data[2] + b.data[9] * a.data[6] + b.data[10] * a.data[10] + b.data[11] * a.data[14];
    result.data[11] = b.data[8] * a.data[3] + b.data[9] * a.data[7] + b.data[10] * a.data[11] + b.data[11] * a.data[15];

    result.data[12] =
        b.data[12] * a.data[0] + b.data[13] * a.data[4] + b.data[14] * a.data[8] + b.data[15] * a.data[12];
    result.data[13] =
        b.data[12] * a.data[1] + b.data[13] * a.data[5] + b.data[14] * a.data[9] + b.data[15] * a.data[13];
    result.data[14] =
        b.data[12] * a.data[2] + b.data[13] * a.data[6] + b.data[14] * a.data[10] + b.data[15] * a.data[14];
    result.data[15] =
        b.data[12] * a.data[3] + b.data[13] * a.data[7] + b.data[14] * a.data[11] + b.data[15] * a.data[15];
}

/** Multiply two matrices.
 */
inline const matrix4 matrix4::operator*(matrix4 const& m) const
{
    matrix4 result((uninitialized_tag()));

    multiply(*this, m, result);

    return result;
}

/** Multiply a vector by the matrix, assuming the forth component to be 1 and the last row in the matrix to be
 * [0,0,0,1].
 */
inline v3<float> const matrix4::operator*(v3<float> const& other) const
{
    v3<float> result;

    result[0] = data[0] * other[0] + data[4] * other[1] + data[8] * other[2] + data[12];
    result[1] = data[1] * other[0] + data[5] * other[1] + data[9] * other[2] + data[13];
    result[2] = data[2] * other[0] + data[6] * other[1] + data[10] * other[2] + data[14];

    return result;
}

/** Multiply a vector by the matrix, assuming the forth component to be 1.
 */
inline v4<float> const matrix4::multiply3(v3<float> const& other) const
{
    v4<float> result;

    result[0] = data[0] * other[0] + data[4] * other[1] + data[8] * other[2] + data[12];
    result[1] = data[1] * other[0] + data[5] * other[1] + data[9] * other[2] + data[13];
    result[2] = data[2] * other[0] + data[6] * other[1] + data[10] * other[2] + data[14];
    result[3] = data[3] * other[0] + data[7] * other[1] + data[11] * other[2] + data[15];

    return result;
}

/** Multiply a vector by the matrix.
 */
inline v4<float> const matrix4::operator*(v4<float> const& other) const
{
    v4<float> result;

    result[0] = data[0] * other[0] + data[4] * other[1] + data[8] * other[2] + data[12] * other[3];
    result[1] = data[1] * other[0] + data[5] * other[1] + data[9] * other[2] + data[13] * other[3];
    result[2] = data[2] * other[0] + data[6] * other[1] + data[10] * other[2] + data[14] * other[3];
    result[3] = data[3] * other[0] + data[7] * other[1] + data[11] * other[2] + data[15] * other[3];

    return result;
}

/** Inplace multiply the matrix by a scalar.
 */
inline matrix4& matrix4::operator*=(float rhs)
{
    for (std::size_t i = 0; i < 16; ++i)
        data[i] *= rhs;

    return *this;
}

/** Inplace multiply the matrix.
    \note Due to the nature of matrix multiplication, this will create a temporary matrix internally.
*/
inline matrix4& matrix4::operator*=(matrix4 const& Other)
{
    *this = *this * Other;
    return *this;
}
} // namespace replay

#endif // replay_matrix4_hpp
