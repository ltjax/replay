/*
replay
Software Library

Copyright (c) 2010 Marius Elvert

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

#ifndef replay_matrix2_hpp
#define replay_matrix2_hpp

#include <boost/optional/optional.hpp>
#include <replay/vector2.hpp>

namespace replay
{

/** 2x2 matrix of floating-point values.
    Can represent 2D linear maps.
    \ingroup Math
*/
class matrix2
{

public:
    /** Create a new uninitialized matrix.
        \note This leaves the contents of the matrix uninitialized.
    */
    matrix2(uninitialized_tag)
    {
    }

    /** Create a new matrix with a given diagonal value.
        All the elements on the diagonal will be set to it, all others will be zero.
        \param value The value to be used for initialization.
    */
    matrix2(float diagonal = 0.f);

    /** Create a matrix from two column vectors.
    */
    matrix2(const vector2f& a, const vector2f& b);

    /** Create a matrix from four values.
    */
    matrix2(float m11, float m21, float m12, float m22);

    /** Constructor for user-defined conversions.
        \see convertible_tag
    */
    template <class source_type>
    matrix2(const source_type& other, typename convertible_tag<source_type, matrix2>::type empty = 0)
    {
        *this = convert(other);
    }

    /** Set the matrix from four values.
    */
    matrix2& set(float m11, float m21, float m12, float m22);

    /** Set the matrix to the identity transformation.
    */
    static matrix2 make_identity();

    /** Set the matrix to a rotational transformation.
        \param angle The angle of the rotation in radians.
    */
    static matrix2 make_rotation(float angle);

    /** Set the matrix to a scale transformation.
    */
    static matrix2 make_scale(const vector2f& scale);

    /** Concaternate the given matrix with a rotational transformation.
    */
    static matrix2& rotate(matrix2& m, const float angle);

    /** Concaternate the given matrix with a scale transformation.
    */
    static matrix2& scale(matrix2& m, const vector2f& scale);

    /** Get a matrix element by two indices.
    */
    float& operator()(std::size_t row, std::size_t column)
    {
        return data[row + (column << 1)];
    }

    /** Get a matrix element by two indices.
    */
    float operator()(std::size_t row, std::size_t column) const
    {
        return data[row + (column << 1)];
    }

    /** Get a matrix element by its column major index.
    */
    float operator[](std::size_t i) const
    {
        return data[i];
    }

    /** Get a matrix column by its index.
    */
    const vector2f column(std::size_t i) const;

    /** Get a matrix row by its index.
    */
    const vector2f row(std::size_t i) const;

    /** Inplace multiplication by a scalar.
    */
    matrix2& operator*=(float rhs);

    /** Right-multiplay a scalar by this matrix.
    */
    const matrix2 operator*(float rhs) const;

    /** Right-multiply a (column) vector by this matrix.
    */
    const vector2f operator*(const vector2f& v) const;

    /** Multiply two matrices.
    */
    const matrix2 operator*(const matrix2& m) const;

    /** Multiply-assign a matrix.
    */
    matrix2& operator*=(const matrix2& m);

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

    /** Multiply two matrices.
    */
    static matrix2& multiply(const matrix2& a, const matrix2& b, matrix2& result);

    /** Compute the determinant of the matrix.
    */
    double determinant() const;

    /** Invert the matrix.
        \returns false if the matrix is singular, true otherwise.
    */
    bool invert(double epsilon = 0.0);

private:
    float data[4];
};

/** Left-multiplay a scalar by a matrix.
*/
inline const matrix2 operator*(float lhs, matrix2 const& rhs)
{
    // Commutative in this case
    return rhs * lhs;
}

/** Get the transpose of this matrix.
*/
inline const matrix2 transpose(const matrix2& rhs)
{
    return matrix2(rhs[0], rhs[1], rhs[2], rhs[3]);
}

/** Left-multiply a (row) vector by this matrix.
    This is equivalent to right-multiply with the transpose matrix.
*/
inline const vector2f operator*(const vector2f& lhs, const matrix2& rhs)
{
    return vector2f(lhs[0] * rhs[0] + lhs[1] * rhs[1], lhs[0] * rhs[2] + lhs[1] * rhs[3]);
}

inline const vector2f matrix2::column(std::size_t i) const
{
    i *= 2;
    return vector2f(data[i], data[i + 1]);
}

inline const vector2f matrix2::row(std::size_t i) const
{
    return vector2f(data[i], data[i + 2]);
}
}

#endif // replay_matrix2_hpp
