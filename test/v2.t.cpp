#include <catch2/catch.hpp>
#include <replay/v2.hpp>

using namespace replay;

TEST_CASE("v2_can_read_from_single-parameter_ctor", "[v2]")
{
    v2<float> v{ 4.32f };
    REQUIRE(v[0] == 4.32f);
    REQUIRE(v[1] == 4.32f);
}


TEST_CASE("v2_can_read_from_multi-parameter_ctor", "[v2]")
{
    v2<float> v{7.1f, 13.9f};
    REQUIRE(v[0] == 7.1f);
    REQUIRE(v[1] == 13.9f);
}
