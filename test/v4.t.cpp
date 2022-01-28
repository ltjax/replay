#include <catch2/catch.hpp>
#include <replay/v4.hpp>

using namespace replay;

TEST_CASE("Can read values from multi-parameter ctor", "[v4]")
{
    v4<float> v{2.f, 3.f, 5.f, 8.f};
    REQUIRE(v[0] == 2.f);
    REQUIRE(v[1] == 3.f);
    REQUIRE(v[2] == 5.f);
    REQUIRE(v[3] == 8.f);
}

TEST_CASE("Can read values from single-parameter ctor", "[v4]")
{
    v4<float> v{ 4.32f };
    REQUIRE(v == v4<float>{4.32f, 4.32f, 4.32f, 4.32f});
}

TEST_CASE("Can use structured binding", "[v4]")
{
    v4<float> v{9.f, 8.f, 7.f, 6.f};
    auto [x, y, z, w] = v;
    REQUIRE(x == 9.f);
    REQUIRE(y == 8.f);
    REQUIRE(z == 7.f);
    REQUIRE(w == 6.f);
}
