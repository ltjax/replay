#include <replay/planar_direction.hpp>
#include <boost/math/constants/constants.hpp>
#include <replay/matrix2.hpp>
#include <replay/vector_math.hpp>

using namespace replay;

planar_direction planar_direction::move(planar_direction from, planar_direction to, float max_angle_delta)
{
    const replay::matrix2 local_to_global = replay::matrix2::make_rotation(from.angle());

    // Matrix is isometric, so we can use transpose multiply instead of inversion
    const auto local_delta = (*inverse(local_to_global)) * to.as_vector();

    // Move towards the direction
    float relative_target_angle = std::atan2(local_delta[1], local_delta[0]);

    if (std::abs(relative_target_angle) < max_angle_delta)
        return to;
    return planar_direction{ from.angle() + math::clamp_absolute(relative_target_angle, max_angle_delta) };
}

planar_direction planar_direction::normalized() const
{
    auto angle = m_angle;
    auto const Pi = boost::math::constants::pi<float>();
    auto const TwoPi = boost::math::constants::two_pi<float>();

    while (angle > Pi)
        angle -= TwoPi;
    while (angle <= -Pi)
        angle += TwoPi;
    return planar_direction(angle);
}

planar_direction planar_direction::average(planar_direction from, planar_direction to)
{
    return planar_direction::from_vector(from.as_vector() + to.as_vector());
}

