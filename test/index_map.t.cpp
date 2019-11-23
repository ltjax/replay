#include <catch2/catch.hpp>
#include <replay/index_map.hpp>

namespace
{
struct sample_payload
{
    std::uint8_t key;
    double value;
};

struct destructor_counter
{
    destructor_counter(std::size_t* destructor_calls)
        : destructor_calls_(destructor_calls)
    {
    }

    destructor_counter(destructor_counter&& rhs)
        : destructor_calls_(rhs.destructor_calls_)
    {
        rhs.destructor_calls_ = 0;
    }

    ~destructor_counter()
    {
        if (destructor_calls_)
            (*destructor_calls_)++;
    }

    std::size_t* destructor_calls_;
};

using replay::index_map;

auto constexpr SAMPLE_INDEX = 3;

index_map<sample_payload> single_element_sample()
{
    index_map<sample_payload> map;
    map.insert(std::make_pair(SAMPLE_INDEX, sample_payload{ 'F', 0.125 }));
    return map;
}

index_map<sample_payload> multi_element_sample()
{
    index_map<sample_payload> map;
    for (auto const& each : { 3, 7, 11, 13 })
        map.insert(std::make_pair(each, sample_payload{ static_cast<std::uint8_t>(each + 1), each * 2.0 }));
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

TEST_CASE("Size goes down after erasing something")
{
    auto map = single_element_sample();
    map.erase(SAMPLE_INDEX);
    REQUIRE(map.size() == 0);
}

TEST_CASE("Empty after erasing only thing")
{
    auto map = single_element_sample();
    map.erase(SAMPLE_INDEX);
    REQUIRE(map.empty());
}

TEST_CASE("Single element in a map makes it not empty")
{
    REQUIRE(!single_element_sample().empty());
}

TEST_CASE("Accessing an erased element throws")
{
    auto map = single_element_sample();
    map.erase(SAMPLE_INDEX);
    REQUIRE_THROWS_AS(map.at(SAMPLE_INDEX), std::out_of_range);
}

TEST_CASE("Erasing an element destructs it")
{
    std::size_t destructor_calls = 0;

    index_map<destructor_counter> map;
    map.insert(std::make_pair(42, destructor_counter(&destructor_calls)));
    map.erase(42);
    REQUIRE(destructor_calls == 1);
}

TEST_CASE("Destructing the map destructs the elements")
{
    std::size_t destructor_calls = 0;

    {
        index_map<destructor_counter> map;
        map.insert(std::make_pair(42, destructor_counter(&destructor_calls)));
        map.insert(std::make_pair(77, destructor_counter(&destructor_calls)));
    }

    REQUIRE(destructor_calls == 2);
}

TEST_CASE("Trying to erase a non-existant element does nothing")
{
    auto map = single_element_sample();
    map.erase(2);
    REQUIRE(map.size() == 1);
}

TEST_CASE("Can iterate a map")
{
    auto map = multi_element_sample();
    std::vector<double> result;
    for (auto const& each : map)
        result.push_back(each.value);

    REQUIRE(result == std::vector<double>{ 6.0, 14.0, 22.0, 26.0 });
}

TEST_CASE("can iterate a const map")
{
    auto const map = multi_element_sample();
    std::vector<double> result;
    for (auto const& each : map)
        result.push_back(each.value);

    REQUIRE(result == std::vector<double>{ 6.0, 14.0, 22.0, 26.0 });
}

TEST_CASE("contains returns true for existing element")
{
    auto map = single_element_sample();
    REQUIRE(map.contains(SAMPLE_INDEX));
}

TEST_CASE("contains returns false for non-existing element")
{
    auto map = single_element_sample();
    REQUIRE(!map.contains(SAMPLE_INDEX - 1));
    REQUIRE(!map.contains(100));
}

TEST_CASE("can correctly tell smallest key bound after single insert")
{
    index_map<char> map;
    map.insert(11, 'z');
    REQUIRE(map.smallest_key_bound() == 12);
}

TEST_CASE("can correctly tell smallest key bound after a removal")
{
    index_map<double> map;
    map.insert(7, 'f');
    map.insert(1, 'g');
    map.erase(7);
    REQUIRE(map.smallest_key_bound() == 2);
}