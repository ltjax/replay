#include <catch2/catch.hpp>
#include <random>

#include <memory>
#include <replay/bounding_rectangle.hpp>
#include <replay/matrix2.hpp>
#include <replay/vector2.hpp>
#include <replay/vector_math.hpp>

typedef replay::vector2f vec2;

float BoundingRectangleUnderTransformation(
    const vec2* Points, std::size_t n, const replay::matrix2& M, vec2& Min, vec2& Max)
{
    Min = M * Points[0];
    Max = Min;

    for (std::size_t i = 1; i < n; ++i)
    {
        vec2 p = M * Points[i];
        float x = p[0];
        float y = p[1];

        if (x < Min[0])
            Min[0] = x;
        else if (x > Max[0])
            Max[0] = x;

        if (y < Min[1])
            Min[1] = y;
        else if (y > Max[1])
            Max[1] = y;
    }

    return (Max[0] - Min[0]) * (Max[1] - Min[1]);
}

void MinimalAreaBoundingRectangle(const vec2* ConvexHull, std::size_t n, replay::matrix2& A, vec2& MinOut, vec2& MaxOut)
{
    // simple quadratic algorithm
    // FIXME: use rotating calipers

    float MinimalArea = std::numeric_limits<float>::max();
    vec2 BestEdge;

    for (std::size_t i = 0; i < n; ++i)
    {
        vec2 Edge = ConvexHull[(i + 1) % n] - ConvexHull[i];

        float Length = magnitude(Edge);
        if (replay::math::fuzzy_zero(Length))
            continue;

        Edge /= Length;

        // make sure the direction is in the first two quadrants
        if (Edge[1] < 0.f)
            Edge.negate();

        // if the direction is in the first quadrant, use as x, otherwise, use as y
        replay::matrix2 M;
        if (Edge[0] > 0.f)
        {
            M.set(Edge[0], Edge[1], -Edge[1], Edge[0]);
        }
        else
        {
            M.set(Edge[1], -Edge[0], Edge[0], Edge[1]);
        }

        vec2 Min, Max;
        float Area = BoundingRectangleUnderTransformation(ConvexHull, n, M, Min, Max);

        if (Area < MinimalArea)
        {
            MinOut = Min;
            MaxOut = Max;
            A = M;
            MinimalArea = Area;
        }
    }
}

template <class T> inline T* ptr(std::vector<T>& v)
{
    return &(v[0]);
}

template <std::size_t n, class ArrayLike> bool fuzzy_equals_n(const ArrayLike& lhs, const ArrayLike& rhs)
{
    using namespace replay;

    for (std::size_t i = 0; i < n; ++i)
        if (!math::fuzzy_equals(lhs[i], rhs[i]))
            return false;

    return true;
}

TEST_CASE("simple minibox")
{
    using namespace replay;

    std::vector<vec2> Points{ vec2(0.f, 0.f), vec2(3.f, 0.f), vec2(2.f, 1.f), vec2(1.f, 1.f) };

    bounding_rectangle_algorithm Box(ptr(Points), Points.size());
    matrix2 M = Box.get_matrix();
    vec2 Min = Box.get_min();
    vec2 Max = Box.get_max();

    REQUIRE(fuzzy_equals_n<4>(M, matrix2(1.f, 0.f, 0.f, 1.f)));
    REQUIRE(fuzzy_equals_n<2>(Min, vec2(0.f, 0.f)));
    REQUIRE(fuzzy_equals_n<2>(Max, vec2(3.f, 1.f)));
}

TEST_CASE("random minibox")
{
    using namespace replay;

    const std::size_t test_count = 32;
    std::mt19937 rng;

    auto random_coord = [&] { return std::uniform_real_distribution<float>(-10.f, 10.f)(rng); };

    for (std::size_t k = 0; k < test_count; ++k)
    {
        std::vector<vector2f> Points;

        for (std::size_t i = 0; i < 32; ++i)
            Points.push_back(vector2f(random_coord(), random_coord()));

        std::size_t n = math::gift_wrap(ptr(Points), Points.size());
        REQUIRE(n >= 3U);

        Points.resize(n);

        bounding_rectangle_algorithm RCBox(ptr(Points), n);
        float Size = dot(RCBox.get_max() - RCBox.get_min(), vector2f(1.f));

        // Compute the minimal rectangle with an easiely provable algorithm
        matrix2 M;
        vec2 Min, Max;
        MinimalAreaBoundingRectangle(ptr(Points), n, M, Min, Max);

        float CorrectSize = dot(Max - Min, vector2f(1.f));

        // Allow 5percent error
        REQUIRE(Size == Approx(CorrectSize).epsilon(0.05f));
    }
}
