#include <replay/planar_direction.hpp>
#include <boost/math/constants/constants.hpp>
#include <catch2/catch.hpp>

using namespace replay;
namespace
{
    constexpr auto pi = boost::math::constants::pi<float>();
    constexpr auto two_pi = boost::math::constants::two_pi<float>();
    constexpr auto half_pi = boost::math::constants::half_pi<float>();
}

TEST_CASE("can_move_around_singularities", "[planar_direction]")
{
    planar_direction current(-4.32833433f);
    planar_direction to(1.95485115f);

    auto next = planar_direction::move(current, to, 1000.f);
    REQUIRE(next == to);
}

TEST_CASE("can_move_toward_a_target", "[planar_direction]")
{
    planar_direction current(2.f);
    planar_direction to(1.f);

    auto next = planar_direction::move(current, to, 0.5f);
    REQUIRE(next.angle() == 1.5f);
}

TEST_CASE("move_picks_the_shorter_arc", "[planar_direction]")
{
    planar_direction current(0.1f);
    planar_direction to(two_pi - 0.1f);
    auto next = planar_direction::move(current, to, 0.1f);
    REQUIRE(next.angle() == 0.f);
}

TEST_CASE("normalizes_high_values_correctly", "[planar_direction]")
{
    REQUIRE(planar_direction(boost::math::constants::two_pi<float>()).normalized().angle() == Approx(0.f));
}

TEST_CASE("normalizes_low_values_correctly", "[planar_direction]")
{
    REQUIRE(planar_direction(-1.5f * pi).normalized().angle() == Approx(pi * 0.5f));
}

TEST_CASE("can_interpolate_between_positive_values", "[planar_direction]")
{
    REQUIRE(lerp(planar_direction(0.f), planar_direction(pi*2.5f), 0.25f).angle() == Approx(pi * 0.125f));
}