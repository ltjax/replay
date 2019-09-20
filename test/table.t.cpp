#include <catch2/catch.hpp>
#include <replay/table.hpp>

using namespace replay;

TEST_CASE("default constructed table is empty")
{
    table<float> t;
    REQUIRE(t.empty());
}

TEST_CASE("move-constructed table has stable address")
{
    table<float> source(4, 4, 42.f);
    auto old_address = source.ptr();
    table<float> target(std::move(source));
    REQUIRE(target.ptr() == old_address);
}

TEST_CASE("move-construction leaves original table empty")
{
    table<double> source(3, 3, 3.1415);
    table<double> other(std::move(source));
    REQUIRE(source.empty());
}

TEST_CASE("move-assigned table has stable address")
{
    table<int> source(5, 5, 0xffaa);
    auto old_address = source.ptr();
    table<int> other;
    other = std::move(source);
    REQUIRE(other.ptr() == old_address);
}

TEST_CASE("copy-assigned table has different address")
{
    table<std::uint16_t> source(7, 9, std::uint16_t(34));
    table<std::uint16_t> other;
    other = source;
    REQUIRE(!other.empty());
    REQUIRE(other.ptr() != source.ptr());
}
