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

#ifndef replay_lines3_hpp
#define replay_lines3_hpp

#include <limits>
#include <replay/interval.hpp>
#include <replay/v3.hpp>

namespace replay
{

/** Base class for parametric line-like objects, like lines, rays and segments.
    Represents a function \f$R \longrightarrow R^3\f$.
    \ingroup Math
*/
class linear_component3
{
public:
    /** Origin. Point at parameter 0.
    */
    v3<float> origin;
    /** Direction. Corresponds to a difference of 1 in the parameter.
    */
    v3<float> direction;

    /** Get the point: \f$origin + t * direction\f$.
        \param t The parameter to the equation.
    */
    v3<float> get_point(const float t) const
    {
        return direction * t + origin;
    }

    /** Set this by another linear component.
    */
    void set(const linear_component3& line)
    {
        origin = line.origin;
        direction = line.direction;
    }

    /** Set via origin and direction.
    */
    void set(const v3<float>& origin, const v3<float>& direction)
    {
        this->origin = origin;
        this->direction = direction;
    }

protected:
    /** Create an uninitialized linear component.
    */
    linear_component3()
    {
    }

    /** Create a linear component by origin and direction.
    */
    linear_component3(const v3<float>& origin, const v3<float>& direction)
    : origin(origin)
    , direction(direction)
    {
    }
};

/** Linear Component that exceeds to infinity in both directions.
    \ingroup Math
*/
class line3 : public linear_component3
{
public:
    /** Create an uninitialized line.
    */
    line3()
    {
    }

    /** Cast a linear component to a line.
    */
    explicit line3(const linear_component3& line)
    : linear_component3(line)
    {
    }

    /** Create a line from origin and direction.
    */
    line3(const v3<float>& origin, const v3<float>& direction)
    : linear_component3(origin, direction)
    {
    }

    /** Create a line from two points on the line.
        These points must not be colinear.
    */
    static line3 from_points(const v3<float>& a, const v3<float>& b)
    {
        return line3(a, b - a);
    }
};

/** Linear Component in the \f$[0..\infty)\f$ interval.
    \ingroup Math
*/
class ray3 : public linear_component3
{
public:
    /** Create and uninitialized ray.
    */
    ray3()
    {
    }

    /** Cast a linear component to a ray.
    */
    explicit ray3(const linear_component3& line)
    : linear_component3(line)
    {
    }

    /** Create a ray from direction and origin.
    */
    ray3(const v3<float>& origin, const v3<float>& direction)
    : linear_component3(origin, direction)
    {
    }
};

/** Linear Component in the \f$[0..1]\f$ interval.
    \ingroup Math
*/
class segment3 : public linear_component3
{
public:
    /** Create an uninitialized segment.
    */
    segment3()
    {
    }

    /** Cast a linear component to a segment.
    */
    explicit segment3(const linear_component3& line)
    : linear_component3(line)
    {
    }

    /** Create a segment from origin and difference to the second point.
    */
    segment3(const v3<float>& origin, const v3<float>& direction)
    : linear_component3(origin, direction)
    {
    }
};

/** Linear Component with a variable interval.
    This is a logical line, ray or segment, depending on whether the interval is unbounded.
    \note Althought this class is a little heavier than line, ray and segment, this is a lot better for clipping.
    \ingroup Math
*/
class line_interval3 : public linear_component3
{
public:
    replay::interval<> interval; /**< begin -> end */

    /** Create a unbounded line interval.
    */
    line_interval3()
    : interval(-std::numeric_limits<float>::max(), std::numeric_limits<float>::max())
    {
    }

    /** Cast a line to an unbounded line interval.
    */
    explicit line_interval3(const line3& x)
    : linear_component3(x)
    , interval(-std::numeric_limits<float>::max(), std::numeric_limits<float>::max())
    {
    }

    /** Cast a segment to a bounded line interval.
    */
    explicit line_interval3(const segment3& x)
    : linear_component3(x)
    , interval(0.f, 1.f)
    {
    }

    /** Cast a ray to a half bounded line interval.
    */
    explicit line_interval3(const ray3& x)
    : linear_component3(x)
    , interval(0.f, std::numeric_limits<float>::max())
    {
    }

    /** Create a line interval from a linear component and a parameter interval.
    */
    line_interval3(const linear_component3& x, replay::interval<> const& interval)
    : linear_component3(x)
    , interval(interval)
    {
    }

    /** Create a line interval from a linear component and a parameter interval.
    */
    line_interval3(const linear_component3& x, const float min, const float max)
    : linear_component3(x)
    , interval(min, max)
    {
    }

    /** Create a line interval from origin, direction and optional interval parameters.
    */
    line_interval3(const v3<float>& origin,
                   const v3<float>& direction,
                   const float min = -std::numeric_limits<float>::max(),
                   const float max = std::numeric_limits<float>::max())
    : linear_component3(origin, direction)
    , interval(min, max)
    {
    }

    /** Cast a segment to a bounded line interval.
    */
    void set_segment(const segment3& x)
    {
        linear_component3::set(x);
        interval.set(0.f, 1.f);
    }

    /** Cast a ray to a half bounded line interval.
    */
    void set_ray(const ray3& x)
    {
        linear_component3::set(x);
        interval.set(0.f, std::numeric_limits<float>::max());
    }

    /** Cast a line to an unbounded line interval.
    */
    void set_line(const line3& x)
    {
        linear_component3::set(x);
        interval.set(-std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    }

    /** Get the point on the lower interval border.
    */
    v3<float> get_min_point() const
    {
        return direction * interval[0] + origin;
    }

    /** Get the point on the upper interval border.
    */
    v3<float> get_max_point() const
    {
        return direction * interval[1] + origin;
    }

    /** Set from a linear component and an interval.
    */
    void set(const linear_component3& x, replay::interval<> const& interval)
    {
        linear_component3::set(x);
        this->interval = interval;
    }

    /** Set from a linear component and an interval.
    */
    void set(const linear_component3& x, const float min, const float max)
    {
        linear_component3::set(x);
        this->interval.set(min, max);
    }

    /** Check wether the attached interval is empty.
    */
    bool empty() const
    {
        return interval[0] > interval[1];
    }
};
}
#endif // replay_lines3_hpp
