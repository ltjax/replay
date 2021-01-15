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

#ifndef replay_vector_math_hpp
#define replay_vector_math_hpp

#include <algorithm>
#include <array>
#include <optional>
#include <ostream>
#include <cmath>
#include <replay/lines3.hpp>
#include <replay/math.hpp>
#include <replay/matrix2.hpp>
#include <replay/matrix3.hpp>
#include <replay/matrix4.hpp>
#include <replay/plane3.hpp>
#include <replay/vector2.hpp>
#include <replay/vector3.hpp>

namespace replay
{

/** A two dimensional parametric line.
    \ingroup Math
*/
class line2
{
public:
    vector2f origin;    /**< origin of the line. */
    vector2f direction; /**< direction of the line. */

    /** Create a degenerated line.
     */
    line2()
    {
    }

    /** Create a line by setting the origin and direction.
     */
    line2(const vector2f& p, const vector2f& d)
    : origin(p)
    , direction(d)
    {
    }

    /** Get a point on the line.
     */
    inline vector2f get_point(float x) const
    {
        return direction * x + origin;
    }
};

/** Plane by matrix multiplication. Treats the the plane as a left sided row vector.
    \ingroup Math
*/
inline plane3 operator*(const plane3& l, const matrix4& r)
{
    return plane3(l.normal[0] * r[0] + l.normal[1] * r[1] + l.normal[2] * r[2] + l.d * r[3],
                  l.normal[0] * r[4] + l.normal[1] * r[5] + l.normal[2] * r[6] + l.d * r[7],
                  l.normal[0] * r[8] + l.normal[1] * r[9] + l.normal[2] * r[10] + l.d * r[11],
                  l.normal[0] * r[12] + l.normal[1] * r[13] + l.normal[2] * r[14] + l.d * r[15]);
}

/** Component wise less predicate.
    Does a lexical comparision between constant-size arrays, for example n-dimensional vectors.
    The type to be used only needs have operator[] available.
    \ingroup Math
*/
template <std::size_t n> class array_less
{
public:
    /** Evalutate the predicate.
        \param lhs The left-hand side of the operator.
        \param rhs The right-hand side of the operator.
        \returns The result of the comparison.
    */
    template <class ArrayType> bool operator()(const ArrayType& lhs, const ArrayType& rhs) const
    {
        for (std::size_t i = 0; i < n; ++i)
        {
            if (lhs[i] < rhs[i])
                return true;
            else if (lhs[i] > rhs[i])
                return false;
        }

        return false;
    }
};

/** Compute the intersection of two 2d lines.
    \ingroup Math
*/
std::optional<v2<float>> intersect_planar_lines(line2 const& a, line2 const& b, float epsilon = math::default_epsilon);

namespace math
{

/** Compute the length of a vector in manhattan norm. (max-norm)
    \ingroup Math
*/
float max_norm(const vector2f& vector);

/** Compute the length of a vector in manhatten norm. (max-norm)
    \ingroup Math
*/
float max_norm(const vector3f& vector);

/** Decompose a rotational matrix into a quaternion. Effectively a conversion between the two.
    \param m Matrix to decompose.
    \param result Quaternion to store the resulting rotation.
    \ingroup Math
*/
void decompose_rotational_matrix(const matrix3& m, quaternion& result);

/** Decompose the rotational part of a 4x4 matrix into a quaternion. The top left 3x3 part of the matrix is converted to
   a quaternion.
    \param m Matrix to decompose.
    \param result Quaternion to store the resulting rotation.
    \ingroup Math
*/
void decompose_rotational_matrix(const matrix4& m, quaternion& result);

/** Compute the determinante of a 2x2 matrix given as two vectors.
    \ingroup Math
*/
inline float det(const vector2f& a, const vector2f& b)
{
    return a[0] * b[1] - a[1] * b[0];
}

/** Compute the determinante of a 2x2 matrix given as 4 values.
    \ingroup Math
*/
inline float det(const float a, const float b, const float c, const float d)
{
    return a * d - b * c;
}

/** Setup a perspective matrix for homogenous coordinates.
    \ingroup Math
*/
void set_perspective_matrix(matrix4& matrix, float fovy, float aspect, float neardist, float fardist);

/** Setup a perspective matrix for homogenous coordinates.
    \ingroup Math
*/
inline matrix4 make_perspective_matrix(float fovy, float aspect, float neardist, float fardist)
{
    matrix4 result((uninitialized_tag()));
    set_perspective_matrix(result, fovy, aspect, neardist, fardist);
    return result;
}

/** Setup an orthographic matrix for homogenous coordinates.
    \ingroup Math
*/
void set_orthographic_matrix(matrix4& matrix, interval<> const& width, interval<> const& height, interval<> const& depth);

/** Setup an orthographic matrix for homogenous coordinates.
    \ingroup Math
*/
inline matrix4 make_orthographic_matrix(interval<> const& width, interval<> const& height, interval<> const& depth)
{
    matrix4 result((uninitialized_tag()));
    set_orthographic_matrix(result, width, height, depth);
    return result;
}

/** Construct a vector that is perpendicular to the given one.
    In general, the resulting vector will be shorter than the given one.
    \ingroup Math
*/
const vector3f construct_perpendicular(const vector3f& x);

/** vector component wise mult by sign.
    \ingroup Math
*/
inline vector3f mult_by_sign(const vector3f& x, float sign)
{
    vector3f result;
    mult_by_sign(x[0], sign, result[0]);
    mult_by_sign(x[1], sign, result[1]);
    mult_by_sign(x[2], sign, result[2]);
    return result;
}

/** vector component wise min.
    \ingroup Math
*/
template <class x> vector2<x> vector_min(const vector2<x>& a, const vector2<x>& b)
{
    return vector2<x>(std::min(a[0], b[0]), std::min(a[1], b[1]));
}

/** vector component wise max.
    \ingroup Math
*/
template <class x> vector2<x> vector_max(const vector2<x>& a, const vector2<x>& b)
{
    return vector2<x>(std::max(a[0], b[0]), std::max(a[1], b[1]));
}

/** vector component wise min.
    \ingroup Math
*/
template <class x> vector3<x> vector_min(const vector3<x>& a, const vector3<x>& b)
{
    return vector3<x>(std::min(a[0], b[0]), std::min(a[1], b[1]), std::min(a[2], b[2]));
}

/** vector component wise max.
    \ingroup Math
*/
template <class x> vector3<x> vector_max(const vector3<x>& a, const vector3<x>& b)
{
    return vector3<x>(std::max(a[0], b[0]), std::max(a[1], b[1]), std::max(a[2], b[2]));
}

/** Computes a 2d vector pointing at the given angle relative to x+.
    \param radians Angle in radians.
    \ingroup Math
*/
vector2f angle_vector(const float radians);

/** Find the intersection point of the 3 given planes, if such a point exists.
    \ingroup Math
*/
vector3f intersect_3planes(const plane3& a, const plane3& b, const plane3& c);

/** Extract the view frustum planes from a given scene matrix.
    The normals of the resulting planes are pointing inwards, i.e. the intersection of all positive sides is 'in'.
    \param scene The scene matrix is Projection * Modelview
    \param frustum Array of the 6 frustum planes, left, right, bottom, top, near, far in that order.
    \ingroup Math
*/
void extract_frustum(const matrix4& scene, plane3* frustum);

/** Find the frustum corners from the 6 given frustum points.
    \ingroup Math
*/
void compute_frustum_corners(const plane3* frustum, vector3f* points);

/** Extract the view frustum side planes from a given scene matrix.
    \param scene The scene matrix is Projection * Modelview
    \param frustum Array of the 4 frustum planes, left, right, bottom, top in that order.
    \ingroup Math
*/
void extract_frustum_sides(const matrix4& scene, plane3* frustum);

/** Find a 2D convex hull of a set of 2d vectors using the gift wrap algorithm.
    The points are reordered so that the first points make up a convex hull of the set.
    The resulting hull is in counter clockwise order.
    This algorithm is output sensitive in its runtime: it will perform a number of n*k steps,
    where n is the number of points, and k is the number of points on the convex hull.
    \param points The points to find the convex hull of.
    \param count The number of points in the set.
    \return The number of points that make up the convex hull.
    \ingroup Math
*/
std::size_t gift_wrap(vector2f* points, std::size_t count);

/** Checks whether the given point is inside the given convex hull.
 */
unsigned int convex_hull_contains(vector2f* hull, unsigned int hullsize, const vector2f& point, const float threshold);

/** Facade to construct the minimal sphere containing a set of points using Welzl's algorithm.
    Expected linear runtime.
    \param p Array of points.
    \param n Number of points in the array.
    \returns Center and square-radius of the computed sphere.
*/
std::tuple<v3<float>, float> minimal_sphere(std::vector<v3<float>> points);

/** Matrix decomposition into lower and upper matrices.
 */
namespace lup
{
/** Decompose the given matrix into a lower triangular matrix,
    an upper triangular matrix and a permutation matrix.
    The decomposition happens mostly in-place: the lower triangular matrix has ones on the diagonal,
    so these are not explicitly stored and used for the diagonal of the upper triangular matrix.
    \param m The matrix to be decomposed and the output space for the lower and upper triangular elements.
    \param p The permutation used for the decomposition.
    \param epsilon Error value to test for zeros.
    \returns true, exactly if the decomposition was successful, i.e., when the matrix is not singular.
*/
bool decompose(matrix3& m, vector3<std::size_t>& p, float epsilon = default_epsilon);

/** Solve an equation in LUP decomposed form.
 */
vector3f solve(const matrix3& lu, const vector3<std::size_t>& p, const vector3f& rhs);

/** Solve an equation.
    The matrix does not need to be decomposed for this form.
    This is just a convenience function that does the LUP decomposition internally.
    Do not use is
*/
vector3f solve(matrix3 m, const vector3f& rhs);
}

/** Primitive intersection tests.
    \ingroup Math
*/
namespace intersection_test
{

/** Test for intersection of a line and a triangle.
    \ingroup Math
*/
bool line_triangle(const linear_component3& line,
                   const vector3f& t0,
                   const vector3f& t1,
                   const vector3f& t2,
                   float* lambda = 0,
                   v2<float>* barycentrics = 0,
                   float epsilon = default_epsilon);

/** Test for intersection of a line and a sphere.
    \note This purposefully ignores differences between line, ray and segment and treats all as lines.
    \param line Linear component to test.
    \param center Center of the sphere.
    \param radius Radius of the sphere.
    \param lambda0 Line parameter for the first intersection.
    \param lambda1 Line parameter for the second intersection.
    \param epsilon Error tolerance to use for the quadratic root finder.
    \ingroup Math
*/
bool line_sphere(const linear_component3& line,
                 const vector3f& center,
                 const float radius,
                 float* lambda0 = 0,
                 float* lambda1 = 0,
                 float epsilon = default_epsilon);
}
}

/** Get the inverse the matrix using the cramer-rule.
    \param rhs The matrix to find the inverse of
    \param epsilon Numerical epsilon to determine if the matrix is singular. Must be 0.0 or greater.
    \returns The inverse of the given matrix on success, none otherwise.
*/
inline std::optional<matrix2> inverse(matrix2 rhs)
{
    if (!rhs.invert())
        return {};

    return rhs;
}

/** Get the inverse the matrix using the cramer-rule.
    \param rhs The matrix to find the inverse of
    \param epsilon Numerical epsilon to determine if the matrix is singular. Must be 0.0 or greater.
    \returns The inverse of the given matrix on success, none otherwise.
*/
std::optional<matrix4> inverse(const matrix4& rhs, double epsilon = 0.0);

/** Find the closest point to a given point on a line.
    \param line line on which to look for the closest point.
    \param point where to find the closest point to.
    \ingroup Math
*/
vector3f find_closest_point(const line3& line, const vector3f& point);

/** Compute the square distance of a point to a polygon.
 */
float square_distance(const vector3f& point, const std::array<vector3f, 3>& triangle);

/** finds the square of the euclidean distance of a line and a point.
    \ingroup Math
*/
float square_distance(const line3& line, const vector3f& point);

/** Compute the square of the euclidean distance of two points.
\ingroup Math
*/
float square_distance(vector2f const& lhs, vector2f const& rhs);

/** Compute the square of the euclidean distance of two points.
\ingroup Math
*/
float square_distance(vector3f const& lhs, vector3f const& rhs);

/** Compute the square of the euclidean distance of two 3d lines.
 */
float square_distance(const line3& la, const line3& lb);

/** Compute the euclidean distance of a line and a point.
    \ingroup Math
*/
float distance(const replay::line3& line, const replay::vector3f& point);

/** Compute the signed distance of a plane and a point.
    If the plane's normal is unit-length, the absolute of this
    distance is the euclidean distance.
    \ingroup Math
*/
float distance(const replay::plane3& p, const replay::vector3f& point);

/** Compute the euclidean distance of two points.
    \ingroup Math
*/
inline float distance(replay::vector2f const& lhs, replay::vector2f const& rhs)
{
    return std::sqrt(square_distance(lhs, rhs));
}

/** Compute the euclidean distance of two points.
\ingroup Math
*/
inline float distance(replay::vector3f const& lhs, replay::vector3f const& rhs)
{
    return std::sqrt(square_distance(lhs, rhs));
}

/** Compute the euclidean length of a vector.
    \ingroup Math
*/
float magnitude(const vector2f& vector);

/** Compute the euclidean length of a vector.
    \ingroup Math
*/
float magnitude(const vector3f& vector);

/** Compute the euclidean length of a vector.
    \ingroup Math
*/
float magnitude(const vector4f& vector);

/** In-place normalize the given vector.
    \ingroup Math
*/
void normalize(vector4f& vector);

/** In-place normalize the given vector.
    \ingroup Math
*/
void normalize(vector3f& vector);

/** In-place normalize the given vector.
    \ingroup Math
*/
void normalize(vector2f& vector);

/** Return a normalized vector.
    \ingroup Math
*/
vector4f normalized(const vector4f& vector);

/** Return a normalized vector.
    \ingroup Math
*/
vector3f normalized(const vector3f& vector);

/** Return a normalized vector.
    \ingroup Math
*/
vector2f normalized(const vector2f& vector);

/** Stream-out a vector in human-readable form.
    This streams a vector with elements x and y as "(x y)".
    \note The element type needs to be streamable.
    \ingroup Math
*/
template <class type> inline std::ostream& operator<<(std::ostream& lhs, const replay::vector2<type>& rhs)
{
    return lhs << '(' << rhs[0] << ' ' << rhs[1] << ')';
}

/** Stream-out a vector in human-readable form.
    This streams a vector with elements x,y and z as "(x y z)".
    \note The element type needs to be streamable.
    \relates vector3
    \ingroup Math
*/
template <class type> inline std::ostream& operator<<(std::ostream& lhs, const replay::vector3<type>& rhs)
{
    return lhs << '(' << rhs[0] << ' ' << rhs[1] << ' ' << rhs[2] << ')';
}

/** Stream-out a vector in human-readable form.
    This streams a vector with elements x,y,z and w as "(x y z w)".
    \note The element type needs to be streamable.
    \relates vector4
    \ingroup Math
*/
template <class type> inline std::ostream& operator<<(std::ostream& lhs, const replay::vector4<type>& rhs)
{
    return lhs << '(' << rhs[0] << ' ' << rhs[1] << ' ' << rhs[2] << ' ' << rhs[3] << ')';
}

inline vector2<float> ceil(vector2<float> rhs)
{
    return { std::ceil(rhs[0]), std::ceil(rhs[1]) };
}

inline vector3<float> ceil(vector3<float> rhs)
{
    return { std::ceil(rhs[0]), std::ceil(rhs[1]), std::ceil(rhs[2]) };
}

inline vector4<float> ceil(vector4<float> rhs)
{
    return { std::ceil(rhs[0]), std::ceil(rhs[1]), std::ceil(rhs[2]), std::ceil(rhs[3]) };
}

inline vector2<float> floor(vector2<float> rhs)
{
    return { std::floor(rhs[0]), std::floor(rhs[1]) };
}

inline vector3<float> floor(vector3<float> rhs)
{
    return { std::floor(rhs[0]), std::floor(rhs[1]), std::floor(rhs[2]) };
}

inline vector4<float> floor(vector4<float> rhs)
{
    return { std::floor(rhs[0]), std::floor(rhs[1]), std::floor(rhs[2]), std::floor(rhs[3]) };
}

}

#endif // replay_vector_math_hpp
