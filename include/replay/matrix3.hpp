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

#ifndef replay_matrix3_hpp
#define replay_matrix3_hpp

#include <replay/vector3.hpp>

namespace replay
{

class quaternion;

/** 3x3 float matrix.
    \ingroup Math
*/
class matrix3
{
public:
    explicit matrix3(uninitialized_tag);

    explicit matrix3(float d);

    matrix3(float m11, float m21, float m31, float m12, float m22, float m32, float m13, float m23, float m33);

    matrix3(const vector3f& a, const vector3f& b, const vector3f& c);

    explicit matrix3(const quaternion& q);

    /** Constructor for user-defined conversions.
        \see convertible_tag
    */
    template <class source_type>
    matrix3(const source_type& other, typename convertible_tag<source_type, matrix3>::type empty = 0)
    {
        *this = convert(other);
    }

    /** Get a pointer to the data.
    */
    const float* ptr() const
    {
        return data;
    }

    /** Get a pointer to the data.
    */
    float* ptr()
    {
        return data;
    }

    /** Get matrix elements by their column-major index.
    */
    template <class index_type> float operator[](const index_type i) const
    {
        return data[i];
    }

    /** Get matrix elements by their column-major index.
    */
    template <class index_type> float& operator[](const index_type i)
    {
        return data[i];
    }

    /** Get matrix elements by their indices.
    */
    float& operator()(unsigned int r, unsigned int c)
    {
        return data[(c * 3) + r];
    }

    /** Get matrix elements by their indices.
    */
    float operator()(unsigned int r, unsigned int c) const
    {
        return data[(c * 3) + r];
    }

    void set_identity();
    void set_rotation(const float angle, const vector3f& axis);
    matrix3& set_rotation_x(const float angle);
    matrix3& set_rotation_y(const float angle);
    matrix3& set_rotation_z(const float angle);
    void set_scale(const float x, const float y, const float z);
    void set_scale(const vector3f& v);
    void set(const vector3f& a, const vector3f& b, const vector3f& c);
    matrix3& set(float m11, float m21, float m31, float m12, float m22, float m32, float m13, float m23, float m33);

    float determinant() const;

    matrix3& transpose();
    matrix3& invert();

    const matrix3 transposed() const;
    const matrix3 inverted() const;

    const matrix3 operator*(const matrix3& m) const;
    const matrix3 operator*(const float f) const;
    const matrix3 operator+(const matrix3& m) const;

    const vector3f operator*(const vector3f& v) const;
    const vector3f operator|(const vector3f& v) const; // transpose multiplication

    matrix3& operator=(const quaternion& q);
    matrix3& operator*=(const matrix3& m);
    matrix3& operator*=(const float f);
    matrix3& operator+=(const matrix3& m);

    /** Get matrix elements by their indices.
    */
    template <class X, class Y> const float operator()(X r, Y c) const
    {
        return data[c * 3 + r];
    }

    /** Get matrix elements by their indices.
    */
    template <class X, class Y> float& operator()(X r, Y c)
    {
        return data[c * 3 + r];
    }

    static void rotate(matrix3& m, const float angle, const vector3f& v);
    static void scale(matrix3& m, const float x, const float y, const float z);
    static void scale(matrix3& m, const vector3f& v);

    static matrix3& multiply(const matrix3& a, const matrix3& b, matrix3& result);
    static vector3f& multiply(const matrix3& a, const vector3f& v, vector3f& result);

    /** Get a matrix column.
    */
    const vector3f get_column(unsigned int index) const
    {
        return vector3f::cast(data + (index * 3));
    }

    /** Get a matrix row.
    */
    const vector3f get_row(unsigned int index) const
    {
        return vector3f(data[index], data[index + 3], data[index + 6]);
    }

    /** Set a row in the matrix.
    */

    void set_row(unsigned int index, const vector3f& v)
    {
        data[index] = v[0];
        data[index + 3] = v[1];
        data[index + 6] = v[2];
    }

    /** Swap matrix rows.
    */
    void swap_rows(unsigned int a, unsigned int b)
    {
        vector3f temp = get_row(a);
        set_row(a, get_row(b));
        set_row(b, temp);
    }

    /** Scale the given row.
    */
    void scale_row(unsigned int i, float x)
    {
        data[i] *= x;
        data[i + 3] *= x;
        data[i + 6] *= x;
    }

    /** Add a scaled row.
    */
    void add_scaled_row(unsigned int src, float x, unsigned int dst)
    {
        data[dst] += x * data[src];
        data[dst + 3] += x * data[src + 3];
        data[dst + 6] += x * data[src + 6];
    }

    /** Get a column via template parameter. */
    template <unsigned int idx> const vector3f get_column() const
    {
        return vector3f::cast(data + idx * 3);
    }

    /** get the first row. */
    template <unsigned int idx> const vector3f get_row() const
    {
        return vector3f(data[idx], data[idx + 3], data[idx + 6]);
    }

private:
    float data[9];
};

// internal order:
/* 0 3 6
   1 4 7
   2 5 8 */

/** Add two matrices.
*/
inline const matrix3 matrix3::operator+(const matrix3& m) const
{
    matrix3 result(*this);
    result += m;
    return result;
}

/** Multiply a matrix by a scalar.
*/
inline const matrix3 matrix3::operator*(const float f) const
{
    matrix3 result(*this);
    result *= f;
    return result;
}
}

#endif // replay_matrix3_hpp
