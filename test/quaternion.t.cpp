#include <boost/math/constants/constants.hpp>
#include <catch2/catch.hpp>
#include <replay/quaternion.hpp>
#include <replay/matrix3.hpp>
using namespace replay;

namespace
{
constexpr auto pi = boost::math::constants::pi<float>();
}
TEST_CASE("quaternion_uses_hamilton_convention", "[quaternion]")
{
    // Make sure we are using  i * j = k , j * k = i , k * i = j for the imaginary number parts
    auto const i = quaternion{ 0.f, 1.f, 0.f, 0.f };
    auto const j = quaternion{ 0.f, 0.f, 1.f, 0.f };
    auto const k = quaternion{ 0.f, 0.f, 0.f, 1.f };

    auto ij = i * j;
    auto jk = j * k;
    auto ki = k * i;

    REQUIRE(ij == k);
    REQUIRE(jk == i);
    REQUIRE(ki == j);
}

TEST_CASE("quaternion_uses_left_chain_convention", "[quaternion]")
{
    v3 v{ 1.f, 0.f, 0.f };
    quaternion q{ pi * 0.25f, v3<float>{ 0.0f, 1.f, 0.f } };
    
    // Convert vector to a pure quaternion
    quaternion p{ 0.f, v[0], v[1], v[2] };

    auto left_chain = q * p * q.conjugated();
    // As opposed to..
    auto right_chain = q.conjugated() * p * q;
    CHECK(left_chain.z == Approx(-std::sqrt(0.5f)).margin(0.00001f));
    CHECK(right_chain.z == Approx(std::sqrt(0.5f)).margin(0.00001f));

    v3 v_{left_chain.x, left_chain.y, left_chain.z};
    
    // Test the other transform functions against this
    v3 v_transform = transform(q, v);
    v3 v_matrix = matrix3(q) * v;
    REQUIRE(v_ == v_transform);
    REQUIRE(v_ == v_matrix);
}
