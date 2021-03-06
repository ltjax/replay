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
    REQUIRE(unpacked == std::vector<std::string>{ "three", "three", "seven", "four", "four" });
}

TEST_CASE("can use the iterators arrow operator")
{
    rle_vector<std::vector<int>> rle;
    rle.push({ 1, 2, 3 });
    REQUIRE(rle.begin()->size() == 3);
}

TEST_CASE("equally constructed rle_vectors compare as equal")
{
    rle_vector<float> a{ { 3.f, 2 }, { 4.f, 7 } };
    rle_vector<float> b{ { 3.f, 2 }, { 4.f, 7 } };
    REQUIRE(a == b);
}

TEST_CASE("move retains backing memory")
{
    rle_vector<float> v{ { 9.f, 2 }, { 8.f, 1 }, { 7.f, 2 }, { 6.f, 1 } };
    auto Before = v.data();
    auto x = std::move(v);
    REQUIRE(v.empty());
    REQUIRE(x.data() == Before);
}

TEST_CASE("iterator can report repetitions")
{
    rle_vector<float> v(11, 77.7f);
    auto i = v.begin();
    ++i;
    ++i;
    REQUIRE(i.repetition_count() == 9);
}

TEST_CASE("iterator has in-place addition")
{
    rle_vector<float> v{ { 56.7f, 6 }, { 123.4f, 7 } };
    auto i = v.begin();
    i += 10;
    REQUIRE(*i == 123.4f);
    REQUIRE(i.repetition_count() == 3);
}

TEST_CASE("iterator has out-of-place addition")
{
    rle_vector<float> v{ { 56.78f, 11 }, { 123.45f, 4 } };
    auto b = v.begin() + std::size_t(6);
    REQUIRE(*b == 56.78f);
    REQUIRE(b.repetition_count() == 5);
}

TEST_CASE("can use suffix increment")
{
    rle_vector<std::uint64_t> v{ { 0xffaaffaaffaaffaaUL, 1 }, { 0x2277227722772277UL, 3 } };
    auto i = v.begin();
    REQUIRE(*i++ == 0xffaaffaaffaaffaaUL);
    REQUIRE(*i == 0x2277227722772277UL);
}
