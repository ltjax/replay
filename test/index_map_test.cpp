#include <catch2/catch.hpp>
#include <replay/index_map.hpp>

namespace
{
struct sample_payload
{
    std::uint8_t key;
    double value;
};

using replay::index_map;

} // namespace

TEST_CASE("starts out empty")
{
    index_map<sample_payload> map;
    REQUIRE(map.empty());
}

TEST_CASE("starts out with size 0")
{
    index_map<sample_payload> map;
    REQUIRE(map.size() == 0);
}

TEST_CASE("size is 1 after one insertion")
{
    index_map<sample_payload> map;
    map.insert(std::make_pair(7, sample_payload{}));
    REQUIRE(map.size() == 1);
}

TEST_CASE("can access an element after inserting it")
{
    index_map<sample_payload> map;
    map.insert(std::make_pair(9, sample_payload{ 'B', 0.5 }));
    REQUIRE(map[9].key == 'B');
    REQUIRE(map[9].value == 0.5);

}
