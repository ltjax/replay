#define BOOST_TEST_MODULE math
#include <boost/test/unit_test.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/scoped_array.hpp>
#include <replay/math.hpp>
#include <replay/vector_math.hpp>
#include <replay/minimal_sphere.hpp>

#include <boost/assign/std/vector.hpp>
#include <boost/assign/std/list.hpp>

namespace replay {

// FIXME: this should probably go into the library
std::ostream& operator<<(std::ostream& str, const replay::vector3<int>& rhs)
{
	return str << '(' << rhs[0] << ' ' << rhs[1] << ' ' << rhs[2] << ')';
}

}

namespace {


// FIXME: this is somewhat generically useful - lift it to a visible namespace?
replay::vector3f polar_to_model( float latitude, float longitude )
{
	latitude = replay::math::convert_to_radians(latitude);
	longitude = replay::math::convert_to_radians(longitude);

	float cw=std::cos(latitude);
	float sw=std::sin(latitude);
	float ch=std::cos(longitude);
	float sh=std::sin(longitude);

	return replay::vector3f( cw*ch,sw*ch,sh );
}

template < class IteratorType >
float distance_to_sphere( const IteratorType point_begin, const IteratorType point_end,
						 const replay::vector3f& center, const float square_radius )
{
	float max_sqr_distance = 0.f;

	for ( IteratorType i=point_begin; i!=point_end; ++i )
	{
		const float sqr_distance = (center-(*i)).squared();

		max_sqr_distance = std::max( max_sqr_distance, sqr_distance ); 
	}

	float radius = std::sqrt(square_radius);
	return std::max( 0.f, std::sqrt(max_sqr_distance)-radius );
}


}

// This test verifies integer arithmetic with a vector3.
// Hopefully, floating-point math will behave correct if this does.
BOOST_AUTO_TEST_CASE( vector3_integer_operations )
{
	using namespace replay;
	typedef vector3<int> vec3;

	const vec3 a(-1, -67, 32);
	const vec3 b(7777, 0, -111);
	const vec3 c(a-b);

	BOOST_REQUIRE_EQUAL( c-a, -b );

	const vec3 all_one(1,1,1);

	BOOST_REQUIRE_EQUAL(all_one.sum(), 3);
	BOOST_REQUIRE_EQUAL(all_one.squared(), 3);

	int checksum=dot(a, all_one);
	BOOST_REQUIRE_EQUAL(checksum, a.sum());

	checksum=dot(b, all_one);
	BOOST_REQUIRE_EQUAL(checksum, b.sum());

	BOOST_REQUIRE_EQUAL(a.sum()-b.sum(), c.sum());

	BOOST_REQUIRE_EQUAL((a*42).sum(), a.sum()*42);

	const vec3 all_two(2,2,2);
	BOOST_REQUIRE_EQUAL(all_one*2, all_two);
	BOOST_REQUIRE_EQUAL(all_one+all_one, all_two);
}

BOOST_AUTO_TEST_CASE( quadratic_equation_solver )
{
	using namespace replay;
	// Attempt to solve a few equations of the form (x-b)(x-a)=0 <=> x^2+(-a-b)*x+b*a=0

	boost::mt19937 rng;
	boost::uniform_real<float> range(-100.f,300.f);
	boost::variate_generator<boost::mt19937&, boost::uniform_real<float> > die(rng, range);

	for ( std::size_t i=0; i<32; ++i )
	{
		float a=die();
		float b=die();
		float c=die();

		if ( replay::math::fuzzy_equals( a,b ) )
			continue;

		fcouple r;
		// FIXME: use a relative epsilon
		math::solve_quadratic_eq( 1.f, -a-b, a*b, r, 0.001f );

		if ( a > b )
			std::swap(a,b);

		BOOST_REQUIRE_CLOSE( r[0], a, 0.01f );
		BOOST_REQUIRE_CLOSE( r[1], b, 0.01f );
	}
}

BOOST_AUTO_TEST_CASE( matrix4_determinant_simple )
{
	using namespace replay;
	matrix4 M( 0.f, 0.f, 3.f, 0.f,
			   4.f, 0.f, 0.f, 0.f,
			   0.f, 2.f, 0.f, 0.f,
			   0.f, 0.f, 0.f, 1.f );

	float d=M.determinant();

	BOOST_REQUIRE_CLOSE( d, 24.f, 0.0001f );

	matrix4 N( 2.f, 1.f, 0.f, 0.f,
			   1.f, 2.f, 1.f, 0.f,
			   0.f, 1.f, 2.f, 1.f,
			   0.f, 0.f, 1.f, 2.f );

	float e=N.determinant();

	BOOST_REQUIRE_CLOSE( e, 5.f, 0.0001f );
}

BOOST_AUTO_TEST_CASE( circumcircle )
{
	using namespace replay;

	// Construct a rotational matrix
	vector3f x=polar_to_model(177.f, -34.f);
	vector3f y=normalized(math::construct_perpendicular(x));
	matrix3 M(x,y,cross(x, y));

	// Construct three points on a circle and rotate them
	const float radius = 14.f;
	float angle = math::convert_to_radians(34.f);
	vector3f a = M*(vector3f(std::cos(angle),std::sin(angle),0.f)*radius);
	angle = math::convert_to_radians(134.f);
	vector3f b = M*(vector3f(std::cos(angle),std::sin(angle),0.f)*radius);
	angle = math::convert_to_radians(270.f);
	vector3f c = M*(vector3f(std::cos(angle),std::sin(angle),0.f)*radius);

	// Move the circle
	const vector3f center( 45.f, 32.f, -37.f );
	a+=center;
	b+=center;
	c+=center;

	// Reconstruct it
	vector3f result_center;

	equisphere<float,3> s(1e-16f);
	BOOST_REQUIRE(s.push(a.ptr()));
	BOOST_REQUIRE(s.push(b.ptr()));
	BOOST_REQUIRE(s.push(c.ptr()));

	vector3f equisphere_center(s.get_center());
	vector3f center_delta = center-equisphere_center;
	BOOST_REQUIRE_SMALL(center_delta.squared(), 0.001f);
	BOOST_REQUIRE_CLOSE(std::sqrt(s.get_squared_radius()),radius,0.001f);

}

// Simple test case directly testing the minimal ball solver in 3D
BOOST_AUTO_TEST_CASE( minimal_ball )
{
	using namespace replay;
	using namespace boost::assign;
	typedef vector3f vec3;
	typedef boost::uniform_real<float> range_type;
	typedef boost::variate_generator<boost::mt19937&, range_type > variate_type;

	// setup random number generators
	boost::mt19937 rng;
	variate_type random_latitude(rng, range_type(-180.f,180.f));
	variate_type random_longitude(rng, range_type(-90.f,90.f));
	variate_type random_scale(rng, range_type(0.f,1.0f));

	// setup a simple point set
	std::list<vec3> points;
	points += vec3(1.f,0.f,0.f),vec3(0.f,1.f,0.f),vec3(0.f,0.f,1.f),vec3(0.f,-1.f,0.f);

	for ( std::size_t i=0; i<32; ++i )
	{
		vector3f t = polar_to_model( random_latitude(), random_longitude() );
		float l = magnitude(t);
		float s = random_scale();
		points += t*s;
	}

	// run the solver
	replay::minimal_ball<float,replay::vector3f,3> ball(points,1e-15f);

	// check correctness
	BOOST_REQUIRE_CLOSE( ball.square_radius(), 1.f, 0.001f );
	BOOST_REQUIRE_SMALL( ball.center().squared(), 0.001f );
	BOOST_REQUIRE_LT( distance_to_sphere( points.begin(),points.end(),ball.center(),ball.square_radius()), 0.001f );
}

// Slightly more sophisticated test for the minimal ball routines using
// the wrapper from vector_math.hpp and an std::vector
BOOST_AUTO_TEST_CASE( minimal_sphere )
{
	using namespace replay;
	typedef boost::uniform_real<float> range_type;
	typedef boost::variate_generator<boost::mt19937&, range_type > variate_type;

	boost::mt19937 rng;

	variate_type random_coord(rng, boost::uniform_real<float>(-100.f,100.f));
	variate_type random_radius(rng, range_type(1.f,3.f));

	variate_type random_latitude(rng, range_type(-180.f,180.f));
	variate_type random_longitude(rng, range_type(-90.f,90.f));
	variate_type random_scale(rng, range_type(0.0f,1.0f));

	std::vector<vector3f> p(64);

	for ( std::size_t i=0; i<16; ++i )
	{
		const vector3f center(random_coord(), random_coord(), random_coord());
		const float radius = random_radius();

		std::size_t boundary_n = 2+rng()%3;

		for ( std::size_t j=0; j<boundary_n; ++j )
			p[j] = center+polar_to_model( random_latitude(), random_longitude() )*radius;

		for ( std::size_t j=boundary_n; j<64; ++j )
			p[j] = center+polar_to_model( random_latitude(), random_longitude() )*(random_scale()*radius);

		std::random_shuffle(p.begin(),p.end());

		vector3f	result_center;
		float		result_square_radius=0.f;
		math::minimal_sphere( &(p[0]),p.size(), result_center, result_square_radius );

		float		square_radius=radius*radius;

		// The generated boundary doesn't necessarily define the minimal ball, but it's an upper bound
		BOOST_REQUIRE_LE( result_square_radius, square_radius );				
		BOOST_REQUIRE_LT( distance_to_sphere(p.begin(),p.end(),result_center,result_square_radius), 0.001f );
	}
}