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

index_map<sample_payload> single_element_sample()
{
    index_map<sample_payload> map;
    map.insert(std::make_pair(3, sample_payload{ 'F', 0.125 }));
    return map;
}

bool has_sample_element(index_map<sample_payload> const& map)
{
    auto& x = map.at(3);
    return (x.key == 'F') && (x.value == 0.125);
}

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

TEST_CASE("at on an element behind capacity throws")
{
    index_map<sample_payload> map;
    REQUIRE_THROWS_AS(map.at(0), std::out_of_range);
}

TEST_CASE("at on an uninitialized element throws")
{
    index_map<sample_payload> map;
    map.insert(std::make_pair(3, sample_payload{ 'F', 0.125 }));
    REQUIRE_THROWS_AS(map.at(2), std::out_of_range);
}

TEST_CASE("at on an initialized element works")
{
    index_map<sample_payload> map;
    map.insert(std::make_pair(3, sample_payload{ 'F', 0.125 }));
    REQUIRE(has_sample_element(map));
}

TEST_CASE("Can move-construct the map")
{
    auto map = single_element_sample();
    REQUIRE(has_sample_element(map));
}

TEST_CASE("Can move-assign the map")
{
    index_map<sample_payload> map;
    map = single_element_sample();
    REQUIRE(has_sample_element(map));
}

TEST_CASE("Can copy-construct the map")
{
    auto first = single_element_sample();
    auto second = first;
    REQUIRE(has_sample_element(first));
    REQUIRE(has_sample_element(second));
}

TEST_CASE("Can copy-assign the map")
{
    index_map<sample_payload> second;
    auto first = single_element_sample();
    second = first;
    REQUIRE(has_sample_element(first));
    REQUIRE(has_sample_element(second));

}