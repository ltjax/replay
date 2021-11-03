#pragma once
#include <replay/v2.hpp>
#include <cmath>

namespace replay
{
class planar_direction
{
public:
    planar_direction()
    : planar_direction(0.f)
    {
    }

    explicit planar_direction(float angle)
    : m_angle(angle)
    {
    }

    float angle() const
    {
        return m_angle;
    }

    v2<float> as_vector() const
    {
        return { std::cos(m_angle), std::sin(m_angle) };
    }

    static planar_direction from_vector(v2<float> rhs)
    {
        return planar_direction(std::atan2(rhs[1], rhs[0]));
    }

    /** Normalize the direction into the (-pi, pi] range. */
    planar_direction normalized() const;

    planar_direction& operator+=(planar_direction const& rhs)
    {
        m_angle += rhs.m_angle;
        return *this;
    }

    planar_direction& operator-=(planar_direction const& rhs)
    {
        m_angle -= rhs.m_angle;
        return *this;
    }

    static planar_direction move(planar_direction from, planar_direction to, float max_angle_delta);

    static planar_direction average(planar_direction from, planar_direction to);

private:
    float m_angle;
};

inline planar_direction operator-(planar_direction lhs, planar_direction const& rhs)
{
    return lhs -= rhs;
}

inline planar_direction operator+(planar_direction lhs, planar_direction const& rhs)
{
    return lhs += rhs;
}

inline bool operator==(planar_direction const& lhs, planar_direction const& rhs)
{
    return lhs.angle() == rhs.angle();
}

inline bool operator!=(planar_direction const& lhs, planar_direction const& rhs)
{
    return !(lhs == rhs);
}

inline planar_direction lerp(planar_direction const& lhs, planar_direction const& rhs, float alpha)
{
    // This uses the face that the normalized difference is between -pi and pi
    auto angle_difference = (rhs - lhs).normalized();
    return planar_direction(angle_difference.angle() * alpha) + lhs;
}
}
