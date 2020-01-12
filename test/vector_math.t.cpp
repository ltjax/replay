#include <catch2/catch.hpp>
#include <replay/vector_math.hpp>

TEST_CASE("Non-colinear lines intersect")
{
    replay::line2 lhs({ 0.f, 1.f }, { 1.f, -1.f });
    replay::line2 rhs({ 1.f, -2.f }, { -1.f, 0.f });

    auto intersection = intersect_planar_lines(lhs, rhs);

    SECTION("And the intersection is valid")
    {
        REQUIRE(intersection);
    }
    SECTION("And the intersection is correct")
    {
        auto p = *intersection;
        REQUIRE(p[0] == Approx(3.f));
        REQUIRE(p[1] == Approx(-2.f));
    }
}

TEST_CASE("Nearly colinear lines depend on the epsilon")
{
    replay::line2 lhs{ {917.986694f, 150.000000f} , replay::normalized({-20.0000000f, -34.6408691f}) };
    replay::line2 rhs{ {913.656494f, 152.499954f} , replay::normalized({-34.6408691f, -59.9999924f}) };

    SECTION("They intersect with the default epsilon")
    {
        auto point = intersect_planar_lines(lhs, rhs);
        REQUIRE(point != std::nullopt);
    }
    SECTION("But they do not intersect with a greater epsilon")
    {
        auto no_point = intersect_planar_lines(lhs, rhs, 0.0001f);
        REQUIRE(no_point == std::nullopt);
    }
}
