#include <catch2/catch.hpp>
#include <replay/math.hpp>
#include <replay/matrix2.hpp>
#include <replay/minimal_sphere.hpp>
#include <replay/vector_math.hpp>
#include <boost/math/constants/constants.hpp>
#include <random>

namespace
{

// FIXME: this is somewhat generically useful - lift it to a visible namespace?
replay::vector3f polar_to_model(float latitude, float longitude)
{
    latitude = replay::math::convert_to_radians(latitude);
    longitude = replay::math::convert_to_radians(longitude);

    float cw = std::cos(latitude);
    float sw = std::sin(latitude);
    float ch = std::cos(longitude);
    float sh = std::sin(longitude);

    return {cw * ch, sw * ch, sh};
}

template <class IteratorType>
float distance_to_sphere(const IteratorType point_begin,
                         const IteratorType point_end,
                         const replay::vector3f& center,
                         const float square_radius)
{
    float max_sqr_distance = 0.f;

    for (IteratorType i = point_begin; i != point_end; ++i)
    {
        const float sqr_distance = (center - (*i)).squared();

        max_sqr_distance = std::max(max_sqr_distance, sqr_distance);
    }

    float radius = std::sqrt(square_radius);
    return std::max(0.f, std::sqrt(max_sqr_distance) - radius);
}
} // namespace

TEST_CASE("matrix2_operations")
{
    using namespace replay;
    matrix2 Rotation = matrix2::make_rotation(boost::math::constants::pi<float>() * 0.25f); // 45deg rotation
    matrix2 Inv = Rotation;
    REQUIRE(Inv.invert());

    using math::fuzzy_equals;
    using math::fuzzy_zero;
    matrix2 I = Rotation * Inv;
    // This should be identity
    REQUIRE(fuzzy_equals(I[0], 1.f));
    REQUIRE(fuzzy_zero(I[1]));
    REQUIRE(fuzzy_zero(I[2]));
    REQUIRE(fuzzy_equals(I[3], 1.f));

    I = Inv * Rotation;
    // This should be identity
    REQUIRE(fuzzy_equals(I[0], 1.f));
    REQUIRE(fuzzy_zero(I[1]));
    REQUIRE(fuzzy_zero(I[2]));
    REQUIRE(fuzzy_equals(I[3], 1.f));
}

// This test verifies integer arithmetic with a vector3.
// Hopefully, floating-point math will behave correct if this does.
TEST_CASE("vector3_integer_operations")
{
    using namespace replay;
    typedef vector3<int> vec3;

    const vec3 a(-1, -67, 32);
    const vec3 b(7777, 0, -111);
    const vec3 c(a - b);

    REQUIRE(c - a == -b);

    const vec3 all_one(1, 1, 1);

    REQUIRE(all_one.sum() == 3);
    REQUIRE(all_one.squared() == 3);

    int checksum = dot(a, all_one);
    REQUIRE(checksum == a.sum());

    checksum = dot(b, all_one);
    REQUIRE(checksum == b.sum());

    REQUIRE(a.sum() - b.sum() == c.sum());

    REQUIRE((a * 42).sum()== a.sum() * 42);

    const vec3 all_two(2, 2, 2);
    REQUIRE(all_one * 2== all_two);
    REQUIRE(all_one + all_one== all_two);
}

TEST_CASE("quadratic_equation_solver")
{
    using namespace replay;
    using range_type = std::uniform_real_distribution<float>;
    // Attempt to solve a few equations of the form (x-b)(x-a)=0 <=> x^2+(-a-b)*x+b*a=0

    std::mt19937 rng;
    range_type range(-100.f, 300.f);
    auto die = [&] { return range(rng); };

    for (std::size_t i = 0; i < 32; ++i)
    {
        float a = die();
        float b = die();

        if (replay::math::fuzzy_equals(a, b))
            continue;

        fcouple r;
        // FIXME: use a relative epsilon
        math::solve_quadratic_eq(1.f, -a - b, a * b, r, 0.001f);

        if (a > b)
            std::swap(a, b);

        REQUIRE(r[0] == Approx(a).margin(0.01f));
        REQUIRE(r[1] == Approx(b).margin(0.01f));
    }
}

TEST_CASE("matrix4_determinant_simple")
{
    using namespace replay;
    matrix4 M(0.f, 0.f, 3.f, 0.f, 4.f, 0.f, 0.f, 0.f, 0.f, 2.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f);

    float d = M.determinant();

    REQUIRE(d == Approx(24.f).margin(0.0001f));

    matrix4 N(2.f, 1.f, 0.f, 0.f, 1.f, 2.f, 1.f, 0.f, 0.f, 1.f, 2.f, 1.f, 0.f, 0.f, 1.f, 2.f);

    float e = N.determinant();

    REQUIRE(e == Approx(5.f).margin(0.0001f));
}

TEST_CASE("circumcircle")
{
    using namespace replay;

    // Construct a rotational matrix
    vector3f x = polar_to_model(177.f, -34.f);
    vector3f y = normalized(math::construct_perpendicular(x));
    matrix3 M(x, y, cross(x, y));

    // Construct three points on a circle and rotate them
    const float radius = 14.f;
    float angle = math::convert_to_radians(34.f);
    vector3f a = M * (vector3f(std::cos(angle), std::sin(angle), 0.f) * radius);
    angle = math::convert_to_radians(134.f);
    vector3f b = M * (vector3f(std::cos(angle), std::sin(angle), 0.f) * radius);
    angle = math::convert_to_radians(270.f);
    vector3f c = M * (vector3f(std::cos(angle), std::sin(angle), 0.f) * radius);

    // Move the circle
    const vector3f center(45.f, 32.f, -37.f);
    a += center;
    b += center;
    c += center;

    // Reconstruct it
    equisphere<float, 3> s(1e-16f);
    REQUIRE(s.push(a.ptr()));
    REQUIRE(s.push(b.ptr()));
    REQUIRE(s.push(c.ptr()));

    vector3f equisphere_center(vector3f::cast(s.get_center()));
    vector3f center_delta = center - equisphere_center;
    REQUIRE(center_delta.squared() < 0.001f);
    REQUIRE(std::sqrt(s.get_squared_radius()) == Approx(radius).margin(0.001f));
}

// Simple test case directly testing the minimal ball solver in 3D
TEST_CASE("minimal_ball")
{
    using namespace replay;
    typedef vector3f vec3;
    using range_type = std::uniform_real_distribution<float>;

    // setup random number generators
    std::mt19937 rng;
    auto random_latitude = [&] { return range_type(-180.f, 180.f)(rng); };
    auto random_longitude = [&] { return range_type(-90.f, 90.f)(rng); };
    auto random_scale = [&] { return range_type(0.f, 1.0f)(rng); };

    // setup a simple point set
    std::list<vec3> points{ vec3(1.f, 0.f, 0.f), vec3(0.f, 1.f, 0.f), vec3(0.f, 0.f, 1.f), vec3(0.f, -1.f, 0.f) };

    for (std::size_t i = 0; i < 32; ++i)
    {
        vector3f t = polar_to_model(random_latitude(), random_longitude());
        float s = random_scale();
        points.push_back(t * s);
    }

    // run the solver
    replay::minimal_ball<float, replay::vector3f, 3> ball(points, 1e-15f);

    // check correctness
    REQUIRE(ball.square_radius() == Approx(1.f).margin(0.001f));
    REQUIRE(ball.center().squared() < 0.001f);
    REQUIRE(distance_to_sphere(points.begin(), points.end(), ball.center(), ball.square_radius()) < 0.001f);
}

// Slightly more sophisticated test for the minimal ball routines using
// the wrapper from vector_math.hpp and an std::vector
TEST_CASE("minimal_sphere")
{
    using namespace replay;
    using range_type = std::uniform_real_distribution<float>;

    std::mt19937 rng;

    auto random_coord = [&] { return range_type(-100.f, 100.f)(rng); };
    auto random_radius = [&] { return range_type(1.f, 3.f)(rng); };

    auto random_latitude = [&] { return range_type(-180.f, 180.f)(rng); };
    auto random_longitude = [&] { return range_type(-90.f, 90.f)(rng); };
    auto random_scale = [&] { return range_type(0.0f, 1.0f)(rng); };

    std::vector<vector3f> p(64);

    for (std::size_t i = 0; i < 16; ++i)
    {
        const vector3f center(random_coord(), random_coord(), random_coord());
        const float radius = random_radius();

        std::size_t boundary_n = 2 + rng() % 3;

        for (std::size_t j = 0; j < boundary_n; ++j)
            p[j] = center + polar_to_model(random_latitude(), random_longitude()) * radius;

        for (std::size_t j = boundary_n; j < 64; ++j)
            p[j] = center + polar_to_model(random_latitude(), random_longitude()) * (random_scale() * radius);

        std::shuffle(p.begin(), p.end(), rng);

        auto const [result_center, result_square_radius] = math::minimal_sphere(p);

        float square_radius = radius * radius;

        // The generated boundary doesn't necessarily define the minimal ball, but it's an upper bound
        REQUIRE(result_square_radius <= Approx(square_radius).margin(0.0001));
        REQUIRE(distance_to_sphere(p.begin(), p.end(), result_center, result_square_radius) < 0.001f);
    }
}