#include <replay/planar_direction.hpp>
#include <boost/math/constants/constants.hpp>
#include <catch2/catch.hpp>

using namespace replay;
namespace
{
auto constexpr pi = boost::math::constants::pi<float>();
}

TEST_CASE("Can move around singularities")
{
    planar_direction current(-4.32833433f);
    planar_direction to(1.95485115f);

    auto next = planar_direction::move(current, to, 1000.f);
    REQUIRE(next == to);
}

TEST_CASE("Normalizes high values correctly")
{
    REQUIRE(planar_direction(boost::math::constants::two_pi<float>()).normalized().angle() == Approx(0.f));
}

TEST_CASE("Normalizes low values correctly")
{
    REQUIRE(planar_direction(-1.5f * pi).normalized().angle() == Approx(pi * 0.5f));
}

TEST_CASE("Can interpolate between positive values")
{
    REQUIRE(lerp(planar_direction(0.f), planar_direction(pi*2.5f), 0.25f).angle() == Approx(pi * 0.125f));
}