#include <catch2/catch.hpp>
#include <replay/vector3.hpp>

using namespace replay;

TEST_CASE("vector3: Can read values from multi-parameter ctor")
{
    constexpr v3<float> v{13.f, 11.f, 19.f};
    REQUIRE(v[0] == 13.f);
    REQUIRE(v[1] == 11.f);
    REQUIRE(v[2] == 19.f);
}