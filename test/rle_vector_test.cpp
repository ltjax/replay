#include <catch2/catch.hpp>
#include <replay/rle_vector.hpp>

using namespace replay;

TEST_CASE("can create an empty rle_vector")
{
    REQUIRE(rle_vector<double>().empty() == true);
}

TEST_CASE("push increases size according to count")
{
    rle_vector<int> rle;
    rle.push(4, 7);
    rle.push(8, 11);
    REQUIRE(rle.size() == 18);
}

TEST_CASE("push defaults to one count")
{
    rle_vector<double> rle;
    rle.push(3.24);
    REQUIRE(rle.size() == 1);
}

TEST_CASE("can initialize using an initializer list")
{
    rle_vector<std::string> rle = { { "three", 2 }, { "seven", 1 }, { "four", 2 } };
    REQUIRE(!rle.empty());
}

TEST_CASE("can unpack using the copy algorithm")
{
    rle_vector<std::string> rle = { { "three", 2 }, { "seven", 1 }, { "four", 2 } };
    std::vector<std::string> unpacked;
    std::copy(rle.begin(), rle.end(), std::back_inserter(unpacked));
    REQUIRE(unpacked == std::vector<std::string>{"three", "three", "seven", "four", "four"});
}

TEST_CASE("can use the iterators arrow operator")
{
    rle_vector<std::vector<int>> rle;
    rle.push({1, 2, 3});
    REQUIRE(rle.begin()->size() == 3);
}
