#pragma once
#include <cmath>
#include <replay/v2.hpp>

namespace replay
{
class planar_direction
{
public:
    constexpr planar_direction()
    : planar_direction(0.f)
    {
    }

    constexpr explicit planar_direction(float angle)
    : m_angle(angle)
    {
    }

    constexpr float angle() const
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

    constexpr planar_direction& operator+=(planar_direction const& rhs)
    {
        m_angle += rhs.m_angle;
        return *this;
    }

    constexpr planar_direction& operator-=(planar_direction const& rhs)
    {
        m_angle -= rhs.m_angle;
        return *this;
    }

    constexpr planar_direction& operator*=(float rhs)
    {
        m_angle *= rhs;
        return *this;
    }

    constexpr planar_direction& operator/=(float rhs)
    {
        m_angle /= rhs;
        return *this;
    }

    constexpr planar_direction operator-() const
    {
        return planar_direction{ -m_angle };
    }

    constexpr auto operator<=>(planar_direction const& rhs) const = default;

    static planar_direction move(planar_direction from, planar_direction to, float max_angle_delta);
    static planar_direction move(planar_direction from, planar_direction to, planar_direction max_angle_delta);

    static planar_direction average(planar_direction from, planar_direction to);

private:
    float m_angle;
};

constexpr inline planar_direction operator-(planar_direction lhs, planar_direction const& rhs)
{
    return lhs -= rhs;
}

constexpr inline planar_direction operator+(planar_direction lhs, planar_direction const& rhs)
{
    return lhs += rhs;
}

constexpr inline planar_direction operator*(planar_direction lhs, float rhs)
{
    return lhs *= rhs;
}

constexpr inline planar_direction operator/(planar_direction lhs, float rhs)
{
    return lhs /= rhs;
}

constexpr inline planar_direction operator*(float lhs, planar_direction rhs)
{
    return rhs * lhs;
}

constexpr inline planar_direction operator/(float lhs, planar_direction rhs)
{
    return rhs / lhs;
}

inline planar_direction lerp(planar_direction const& lhs, planar_direction const& rhs, float alpha)
{
    // This uses the face that the normalized difference is between -pi and pi
    auto angle_difference = (rhs - lhs).normalized();
    return planar_direction(angle_difference.angle() * alpha) + lhs;
}
} // namespace replay
