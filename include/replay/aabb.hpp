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

#ifndef replay_aabb_hpp
#define replay_aabb_hpp

#include <algorithm>
#include <array>
#include <replay/plane3.hpp>
#include <replay/vector3.hpp>

namespace replay
{

/** An iso-box in \f$\mathbb{R}^3\f$.
    Represents the intersection of intervals on the 3 principal axes.
    \ingroup Math
*/
class aabb : private std::array<vector3f, 2>
{
public:
    /** Classification relative to a plane.
        \see classify
    */
    enum classify_result
    {
        negative, /**< All points have a negative distance to the plane. */
        positive, /**< All points have a positive distance to the plane. */
        spanning  /**< The points have mixed signs in their distances to the plane, so the box intersects the plane. */
    };
    aabb();
    explicit aabb(float half_extends);
    explicit aabb(vector3f const& point);
    aabb(vector3f const& min, vector3f const& max);

    /** Constructor for user-defined conversions.
        \see convertible_tag
    */
    template <class source_type>
    aabb(const source_type& other, typename convertible_tag<source_type, aabb>::type empty = 0)
    {
        *this = convert(other);
    }

    aabb& clear();
    aabb& move(vector3f const& delta);

    /** Check if this bounding box is empty, i.e. does not contain any points.
     */
    bool empty() const;

    /** Enlarge the box to contain the given point.
        \param point The point to be inserted.
    */
    aabb& insert(vector3f const& point)
    {
        for (unsigned int i = 0; i < 3; ++i)
        {
            if (point[i] < min(i))
                min(i) = point[i];
            if (point[i] > max(i))
                max(i) = point[i];
        }

        return *this;
    }

    aabb& insert(vector3f const* points, unsigned int count);
    aabb& insert(vector3f const* points, const unsigned short* indices, unsigned int count);
    aabb& insert(vector3f const* points, const unsigned int* indices, unsigned int count);
    aabb& insert(const aabb& rhs);
    aabb const inserted(aabb rhs) const;

    vector3f& compute_arvo_vector(vector3f const& point, vector3f& result) const;

    /** Compute the shortest vector from the point to the box.
     */
    inline vector3f compute_arvo_vector(vector3f const& point) const
    {
        vector3f temp;
        return compute_arvo_vector(point, temp);
    }

    vector3f& compute_center(vector3f& result) const;

    /** Compute the center of the aabb.
     */
    inline vector3f compute_center() const
    {
        vector3f temp;
        return compute_center(temp);
    }

    vector3f& corner(std::size_t i, vector3f& result) const;

    /** Compute a corner of the aabb.
     */
    inline vector3f corner(std::size_t i) const;

    aabb& compute_subinterval(unsigned int index, vector3f const& pivot, aabb& result) const;

    float square_distance(vector3f const& other) const;
    float distance(vector3f const& other) const;

    bool contains(vector3f const& point) const;

    std::array<float, 2> project(vector3f const& x) const;
    classify_result classify(plane3 const& x) const;

    aabb& expand(vector3f const& x);
    aabb const expanded(vector3f const& x) const;

    /** Get the minimum in all three dimensions.
     */
    vector3f& min()
    {
        return front();
    }

    /** Get the minimum in all three dimensions.
     */
    vector3f const& min() const
    {
        return front();
    }

    /** Get the minimum element in the given dimension.
     */
    template <class IndexType> float& min(IndexType i)
    {
        return front()[i];
    }

    /** Get the minimum element in the given dimension.
     */
    template <class IndexType> float min(IndexType i) const
    {
        return front()[i];
    }

    /** Get the maximum in all three dimensions.
     */
    vector3f& max()
    {
        return back();
    }

    /** Get the maximum in all three dimensions.
     */
    vector3f const& max() const
    {
        return back();
    }

    /** Get the maximum element in the given dimension.
     */
    template <class IndexType> float& max(IndexType i)
    {
        return back()[i];
    }

    /** Get the maximum element in the given dimension.
     */
    template <class IndexType> float max(IndexType i) const
    {
        return back()[i];
    }

    /** Intersect this box with another.
     */
    aabb& intersect(const replay::aabb& rhs);

    /** Create a box that is the intersection of this and another.
     */
    aabb const intersected(replay::aabb rhs) const;
};

inline bool operator==(aabb const& lhs, aabb const& rhs)
{
    return lhs.min() == rhs.min() && lhs.max() == rhs.max();
}

inline bool operator!=(aabb const& lhs, aabb const& rhs)
{
    return !(lhs == rhs);
}
} // namespace replay

inline replay::aabb const replay::aabb::inserted(replay::aabb rhs) const
{
    return rhs.insert(*this);
}

inline replay::aabb const replay::aabb::intersected(replay::aabb rhs) const
{
    return rhs.intersect(*this);
}

inline replay::vector3f replay::aabb::corner(std::size_t i) const
{
    vector3f temp((replay::uninitialized_tag()));
    return corner(i, temp);
}

#endif
