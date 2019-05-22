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

#include <cmath>
#include <limits>
#include <replay/aabb.hpp>
#include <replay/math.hpp>
#include <replay/vector_math.hpp>

/** Create a new empty box that contains nothing.
*/
replay::aabb::aabb()
{
    clear();
}

/** Create a new box that extends in all directions from the origin.
*/
replay::aabb::aabb(float half_extends)
: aabb(vector3f(-half_extends), vector3f(half_extends))
{
}

/** Create a new box containing a single point.
*/
replay::aabb::aabb(vector3f const& point)
: aabb(point, point)
{
}

/** Create a new box from range.
*/
replay::aabb::aabb(vector3f const& min, vector3f const& max)
: std::array<vector3f, 2>{ min, max }
{
}

/** Clear the box to contain nothing.
    To be more precise the lower bounds will be MAX_FLOAT and the upper bounds will be -MAX_FLOAT.
*/
replay::aabb& replay::aabb::clear()
{
    min().reset(std::numeric_limits<float>::max());
    max().reset(-std::numeric_limits<float>::max());

    return *this;
}

bool replay::aabb::empty() const
{
    // One of the ranges has to be empty
    return max(0) < min(0) || max(1) < min(1) || max(2) < min(2);
}

/** Compute the square distance to the box using Arvo's algorithm.
*/
float replay::aabb::square_distance(vector3f const& other) const
{
    // Arvo's Algorithm
    float result = 0.f;
    float temp;

    for (unsigned int i = 0; i < 3; ++i)
    {
        if (other[i] < min(i))
        {
            temp = other[i] - min(i);
            result += temp * temp;
        }

        else if (other[i] > max(i))
        {
            temp = other[i] - max(i);
            result += temp * temp;
        }
    }
    return result;
}

/** Compute the euclidean distance to the box using Arvo's algorithm.
*/
float replay::aabb::distance(vector3f const& other) const
{
    return std::sqrt(square_distance(other));
}

/** Project the box onto a vector.
    \param x The vector that the box is supposed to be projected onto.
    \returns a An ordered range of the box projected onto the vector.
*/
std::array<float, 2> replay::aabb::project(vector3f const& x) const
{
    std::array<float, 2> result{ 0.f, 0.f };

    for (unsigned int i = 0; i < 3; ++i)
    {
        unsigned int mask = math::sign(x[i]);

        result[mask] += min(i) * x[i];
        result[mask ^ 1] += max(i) * x[i];
    }

    if (result[0] > result[1])
        std::swap(result[0], result[1]);

    return result;
}

/** Classify the box in respect to a plane.
*/
replay::aabb::classify_result replay::aabb::classify(plane3 const& x) const
{
    unsigned int mask;
    fcouple result;

    result.set(x.d, x.d);

    for (unsigned int i = 0; i < 3; ++i)
    {
        mask = math::sign(x.normal[i]);

        result[mask] += min(i) * x.normal[i];
        result[mask ^ 1] += max(i) * x.normal[i];
    }

    if (result[0] > result[1])
        result.swap();

    if (!math::same_sign(result[0], result[1]))
        return spanning;
    else if (result[1] > 0.f)
        return positive;
    else
        return negative;
}

/** Move the box.
*/
replay::aabb& replay::aabb::move(vector3f const& delta)
{
    min() += delta;
    max() += delta;
    return *this;
}

/** Enlarge the aabb to contain another aabb.
*/
replay::aabb& replay::aabb::insert(const aabb& x)
{
    for (unsigned int i = 0; i < 3; ++i)
    {
        if (x[0][i] < min(i))
            min(i) = x[0][i];
        if (x[1][i] > max(i))
            max(i) = x[1][i];
    }

    return *this;
}

/** Enlarge the aabb to contain the array of points.
*/
replay::aabb& replay::aabb::insert(vector3f const* points, unsigned int count)
{
    if (!count)
        return *this;

    float t = 0.f;

    // insert first point explicitly to assert max_i >= min_i
    insert(points[0]);

    for (unsigned int i = 1; i < count; ++i)
    {
        for (unsigned int j = 0; j < 3; ++j)
        {
            t = points[i][j];

            if (t < min()[j])
                min()[j] = t;
            else if (t > max()[j])
                max()[j] = t;
        }
    }

    return *this;
}

/** Enlarge the aabb to contain the indexed points from the array.
*/
replay::aabb& replay::aabb::insert(vector3f const* points, const unsigned int* indices, unsigned int count)
{
    if (!count)
        return *this;

    float t = 0.f;

    // insert first point explicitly to assert max_i > min_i
    insert(points[indices[0]]);

    for (unsigned int i = 1; i < count; ++i)
    {
        for (unsigned int j = 0; j < 3; ++j)
        {
            t = points[indices[i]][j];

            if (t < min()[j])
                min()[j] = t;
            else if (t > max()[j])
                max()[j] = t;
        }
    }

    return *this;
}

/** Enlarge the aabb to contain the indexed points from the array.
*/
replay::aabb& replay::aabb::insert(vector3f const* points, const unsigned short* indices, unsigned int count)
{
    if (!count)
        return *this;

    float t = 0.f;

    // insert first point explicitly to assert max_i > min_i
    insert(points[indices[0]]);

    for (unsigned int i = 1; i < count; ++i)
    {
        for (unsigned int j = 0; j < 3; ++j)
        {
            t = points[indices[i]][j];

            if (t < min()[j])
                min()[j] = t;
            else if (t > max()[j])
                max()[j] = t;
        }
    }

    return *this;
}

/** the arvo vector is the difference of point and the closest point on the interval to it.
*/
replay::vector3f& replay::aabb::compute_arvo_vector(vector3f const& point, vector3f& result) const
{
    for (unsigned int i = 0; i < 3; ++i)
    {
        if (point[i] < min(i))
            result[i] = point[i] - min(i);
        else if (point[i] > max(i))
            result[i] = point[i] - max(i);
        else
            result[i] = 0.f;
    }

    return result;
}

/** Compute the center of the box.
*/
replay::vector3f& replay::aabb::compute_center(vector3f& result) const
{
    return (result = (min() + max()) * 0.5f);
}

/** Compute a given corner of the box, whereas the n'th bit in the index corresponds to the n'th axis in space.
*/
replay::vector3f& replay::aabb::corner(std::size_t index, vector3f& result) const
{
    for (std::size_t axis = 0; axis < 3; ++axis)
        result[axis] = (index & (std::size_t(1) << axis)) ? max(axis) : min(axis);

    return result;
}

/** Compute a sub-aabb for octree-like splits.
*/
replay::aabb& replay::aabb::compute_subinterval(unsigned int index, vector3f const& pivot, aabb& result) const
{
    for (unsigned int j = 0; j < 3; ++j)
    {
        result[0][j] = (((index >> j) & 1) ? pivot : min())[j];
        result[1][j] = (((index >> j) & 1) ? max() : pivot)[j];
    }

    return result;
}

/** Check if a point is inside this box.
*/
bool replay::aabb::contains(vector3f const& point) const
{
    return math::in_range(point[0], min()[0], max()[0]) && math::in_range(point[1], min()[1], max()[1]) &&
           math::in_range(point[2], min()[2], max()[2]);
}

/** Create the aabb [min-x,max+x].
*/
replay::aabb& replay::aabb::expand(const replay::vector3f& x)
{
    min() -= x;
    max() += x;
    return *this;
}

/** Create the aabb [min-x,max+x].
*/
replay::aabb const replay::aabb::expanded(const replay::vector3f& x) const
{
    return aabb(min() - x, max() + x);
}

replay::aabb& replay::aabb::intersect(const replay::aabb& rhs)
{
    min() = replay::math::vector_max(min(), rhs.min());
    max() = replay::math::vector_min(max(), rhs.max());

    return *this;
}
