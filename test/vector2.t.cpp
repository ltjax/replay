#include <catch2/catch.hpp>
#include <replay/v2.hpp>

using namespace replay;

TEST_CASE("vector2: Can read values from single-parameter ctor")
{
    v2<float> v{ 4.32f };
    REQUIRE(v[0] == 4.32f);
    REQUIRE(v[1] == 4.32f);
}


TEST_CASE("vector2: Can read values from multi-parameter ctor")
{
    v2<float> v{7.1f, 13.9f};
    REQUIRE(v[0] == 7.1f);
    REQUIRE(v[1] == 13.9f);
}
