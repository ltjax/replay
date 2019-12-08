#include <catch2/catch.hpp>
#include <replay/pixbuf.hpp>

using namespace replay;

TEST_CASE("Default initialized is empty")
{
    pixbuf p;
    REQUIRE(p.empty());
}

TEST_CASE("Can fill with orange")
{
    pixbuf image(4, 4, pixbuf::color_format::rgb);
    image.fill(255, 128, 0);

    REQUIRE(image.read_pixel(2, 3) == byte_rgba{ 255, 128, 0, 255 });
}

TEST_CASE("Can move pixbufs")
{
    pixbuf left;
    pixbuf right(5, 7, pixbuf::color_format::rgba);
    left = std::move(right);
    REQUIRE(left.width() == 5);
    REQUIRE(left.height() == 7);
    REQUIRE(left.channel_count() == 4);
    REQUIRE(right.empty());
}

TEST_CASE("Can initialize pixbufs")
{
    pixbuf image(12, 17, pixbuf::color_format::greyscale);
    REQUIRE(image.width() == 12);
    REQUIRE(image.height() == 17);
    REQUIRE(image.channel_count() == 1);
    REQUIRE(image.pixel_format() == pixbuf::color_format::greyscale);
}

TEST_CASE("Can copy pixbufs")
{
    pixbuf original(7, 3, pixbuf::color_format::rgba);
    original.fill(0);
    original.assign_pixel(1, 2, { 32, 64, 96, 128 });
    original.assign_pixel(5, 0, { 1, 2, 3, 4 });
    auto copy = original;
    REQUIRE(copy.read_pixel(1, 2) == byte_rgba{ 32, 64, 96, 128 });
    REQUIRE(copy.read_pixel(5, 0) == byte_rgba{ 1, 2, 3, 4 });
}
