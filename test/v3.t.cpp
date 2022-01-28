#include <catch2/catch.hpp>
#include <replay/v3.hpp>

using namespace replay;

TEST_CASE("v3_can_read_from_multi-parameter_ctor", "[v3]")
{
    constexpr v3<float> v{13.f, 11.f, 19.f};
    REQUIRE(v[0] == 13.f);
    REQUIRE(v[1] == 11.f);
    REQUIRE(v[2] == 19.f);
}
