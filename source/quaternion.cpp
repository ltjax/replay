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
#include <replay/quaternion.hpp>
#include <replay/vector_math.hpp>

replay::quaternion& replay::quaternion::set(const float w, const float x, const float y, const float z)
{
    this->w = w;
    this->x = x;
    this->y = y;
    this->z = z;

    return *this;
}

replay::quaternion& replay::quaternion::set_identity()
{
    return set(1.f, 0.f, 0.f, 0.f);
}

replay::quaternion& replay::quaternion::set_rotation(float angle, const vector3f& axis)
{
    angle *= 0.5f;

    w = std::cos(angle);
    angle = std::sin(angle);

    x = axis[0] * angle;
    y = axis[1] * angle;
    z = axis[2] * angle;

    return (*this);
}

replay::quaternion::quaternion()
: w(1.f)
, x(0.f)
, y(0.f)
, z(0.f)
{
}

replay::quaternion::quaternion(const float angle, const vector3f& axis)
{
    set_rotation(angle, axis);
}

replay::quaternion::quaternion(const float w, const float x, const float y, const float z)
: w(w)
, x(x)
, y(y)
, z(z)
{
}

const replay::quaternion replay::quaternion::operator*(const quaternion& operand) const
{
    return multiply(*this, operand);
}

replay::quaternion& replay::quaternion::operator*=(const quaternion& operand)
{
    return (*this = multiply(*this, operand));
}

const replay::quaternion replay::quaternion::operator*(const float rhs) const
{
    return quaternion(w * rhs, x * rhs, y * rhs, z * rhs);
}

const replay::quaternion replay::quaternion::operator+(const quaternion& q) const
{
    return quaternion(w + q.w, x + q.x, y + q.y, z + q.z);
}

const replay::quaternion replay::quaternion::operator-(const quaternion& q) const
{
    return quaternion(w - q.w, x - q.x, y - q.y, z - q.z);
}

const replay::quaternion replay::quaternion::operator/(const float value) const
{
    return ((*this) * (1.f / value));
}

replay::quaternion& replay::quaternion::operator*=(const float value)
{
    w *= value;
    x *= value;
    y *= value;
    z *= value;

    return (*this);
}

replay::quaternion& replay::quaternion::operator/=(const float value)
{
    return ((*this) *= (1.f / value));
}

const float replay::quaternion::squared() const
{
    return w * w + x * x + y * y + z * z;
}

const float replay::quaternion::magnitude() const
{
    return std::sqrt(squared());
}

replay::quaternion& replay::quaternion::conjugate()
{
    x = -x;
    y = -y;
    z = -z;

    return *this;
}

const replay::quaternion replay::quaternion::conjugated() const
{
    return quaternion(w, -x, -y, -z);
}

const replay::quaternion replay::quaternion::negated() const
{
    return quaternion(-w, -x, -y, -z);
}

replay::quaternion& replay::quaternion::negate()
{
    w = -w;
    x = -x;
    y = -y;
    z = -z;
    return *this;
}

replay::quaternion& replay::quaternion::normalize()
{
    const float s = squared();

    if (s != 1.0)
        (*this) /= std::sqrt(s);

    return *this;
}

void replay::rotate(quaternion& q, const float angle, const vector3f& axis)
{
    const quaternion delta(angle, axis);

    q *= delta;
    q.normalize();
}

std::tuple<replay::vector3f, float> replay::to_axis_angle(const quaternion& obj)
{
    const float angle = 2.f * std::acos(math::clampabs(obj.w, 1.f));

    // begin calculating the inverse sine
    float factor = std::sqrt(std::max(1.f - (obj.w * obj.w), 0.f));

    // FIXME: is this needed?
    if (std::fabs(factor) < 0.0001f)
        factor = 1.f;
    else
        factor = 1.f / factor;

    return std::make_tuple(replay::vector3f(obj.x * factor, obj.y * factor, obj.z * factor), angle);
}

const replay::vector3f replay::quaternion::get_transformed_x() const
{
    return vector3f(1.f - 2.f * (y * y + z * z), 2.f * (x * y + w * z), 2.f * (x * z - w * y));
}

const replay::vector3f replay::quaternion::get_transformed_y() const
{
    return vector3f(2.f * (x * y - w * z), 1.f - 2.f * (x * x + z * z), 2.f * (y * z + w * x));
}

const replay::vector3f replay::quaternion::get_transformed_z() const
{
    return vector3f(2.f * (x * z + w * y), 2.f * (y * z - w * x), 1.f - 2.f * (x * x + y * y));
}

const float replay::dot(const quaternion& a, const quaternion& b)
{
    return a.w * b.w + a.x * b.x + a.y * b.y + a.z * b.z;
}

const replay::quaternion replay::shortest_arc(const vector3f& a, const vector3f& b)
{
    // Compute the cosine of the angle between the two vectors
    const float cos = dot(a, b);

    // Return an identity if the angle is zero
    if (math::fuzzy_equals(cos, 1.f))
        return quaternion();

    // Otherwise rotate around the perpendicular vector
    const vector3f axis = normalized(cross(a, b));

    return quaternion(std::acos(cos), axis);
}

replay::vector3f replay::transform(const quaternion& q, const vector3f& v)
{
    return vector3f((1.f - 2.f * (q.y * q.y + q.z * q.z)) * v[0] + 2.f * (q.x * q.y - q.z * q.w) * v[1] +
                        2.f * (q.x * q.z + q.y * q.w) * v[2],
                    2.f * (q.x * q.y + q.z * q.w) * v[0] + (1.f - 2.f * (q.x * q.x + q.z * q.z)) * v[1] +
                        2.f * (q.y * q.z - q.x * q.w) * v[2],
                    2.f * (q.x * q.z - q.y * q.w) * v[0] + 2.f * (q.y * q.z + q.x * q.w) * v[1] +
                        (1.f - 2.f * (q.x * q.x + q.y * q.y)) * v[2]);
}

const replay::quaternion replay::nlerp(const replay::quaternion& a, const replay::quaternion& b, const float x)
{
    quaternion result;

    // Turn the right way...
    if (dot(a, b) < 0.f)
        result = (a * (1.f - x) - b * x);
    else
        result = (a * (1.f - x) + b * x);

    result.normalize();
    return result;
}

const replay::quaternion replay::slerp(const replay::quaternion& a, const replay::quaternion& b, const float x)
{
    const float angle = dot(a, b);

    if (angle < 0.f)
    {
        if (math::fuzzy_equals(angle, -1.f)) // nlerp
        {
            quaternion result(a * (1.f - x) - b * x);
            return result.normalize();
        }

        const float theta = std::acos(math::clamp(-angle, -1.f, 1.f));
        const float sin_theta = std::sin(theta);
        const float m = std::sin((1.f - x) * theta) / sin_theta;
        const float n = std::sin(x * theta) / sin_theta;

        return a * m - b * n;
    }
    else
    {
        if (math::fuzzy_equals(angle, 1.f)) // nlerp
        {
            quaternion result(a * (1.f - x) + b * x);
            return result.normalize();
        }

        const float theta = std::acos(math::clamp(angle, -1.f, 1.f));
        const float sin_theta = std::sin(theta);
        const float m = std::sin((1.f - x) * theta) / sin_theta;
        const float n = std::sin(x * theta) / sin_theta;

        return a * m + b * n;
    }
}

const replay::quaternion replay::short_rotation(const quaternion& a, const quaternion& b)
{
    quaternion result(b * inverse(a));

    if (dot(a, b) < 0.f)
        result.negate();

    return result;
}

const replay::quaternion replay::inverse(const quaternion& a)
{
    return a.conjugated() / a.squared();
}