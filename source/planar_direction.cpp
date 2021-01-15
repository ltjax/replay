#include <replay/planar_direction.hpp>
#include <boost/math/constants/constants.hpp>
#include <replay/matrix2.hpp>
#include <replay/vector_math.hpp>

using namespace replay;

planar_direction planar_direction::move(planar_direction from, planar_direction to, float max_angle_delta)
{
    auto const relative_move = (to - from).normalized();

    if (std::abs(relative_move.angle()) < max_angle_delta)
        return to;

    return planar_direction{ from.angle() + math::clamp_absolute(relative_move.angle(), max_angle_delta) };
}

planar_direction planar_direction::normalized() const
{
    auto angle = m_angle;
    constexpr auto pi = boost::math::constants::pi<float>();
    constexpr auto two_pi = boost::math::constants::two_pi<float>();

    while (angle > pi)
        angle -= two_pi;
    while (angle <= -pi)
        angle += two_pi;
    return planar_direction(angle);
}

planar_direction planar_direction::average(planar_direction from, planar_direction to)
{
    return planar_direction::from_vector(from.as_vector() + to.as_vector());
}
