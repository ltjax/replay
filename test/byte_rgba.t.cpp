#include <catch2/catch.hpp>
#include <replay/byte_rgba.hpp>

TEST_CASE("Can encode white as hex-string")
{
    REQUIRE(to_rgb_hex_string(replay::palette::white) == "#FFFFFF");
}

TEST_CASE("Can encode orange as hex-string")
{
    REQUIRE(to_rgb_hex_string(replay::byte_rgba{ 255, 127, 0 }) == "#FF7F00");
}

TEST_CASE("Default color is transparent black")
{
    replay::byte_rgba default_initialized;
    REQUIRE(default_initialized == replay::byte_rgba{ 0, 0, 0, 0 });
}