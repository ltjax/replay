
#include <boost/test/unit_test.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/mersenne_twister.hpp>

#include <boost/assign.hpp>

#include <memory>
#include <replay/vector2.hpp>
#include <replay/vector_math.hpp>
#include <replay/matrix2.hpp>
#include <replay/bounding_rectangle.hpp>

typedef replay::vector2f vec2;
/*
class RotatingCalipersBox
{
public:
	RotatingCalipersBox( const vec2* P, std::size_t n ) : P(P),n(n)
	{

		// Get an initial box and the extrema
		InitialExtrema();

		// Start with no rotation
		Current.u.set(1.f,0.f);
		
		// Check all possible configurations
		float	MinArea=ComputeCurrentSize();
		Best=Current;

		for ( std::size_t i=1; i<n; ++i )
		{
			RotateSmallestAngle();

			// Only rotate a maximum of 90 degrees
			if( Current.u[0] <= 0.f )
				break;

			float Area = ComputeCurrentSize();

			// Did we find a better one?
			if ( Area < MinArea )
			{
				MinArea = Area;
				Best = Current;
			}
		}
	}

	const replay::matrix2 GetMatrix() const
	{
		replay::matrix2 Result(
			 Best.u[0], Best.u[1],
			-Best.u[1], Best.u[0] );

		return Result;
	}

	const vec2& GetMin() const
	{
		return Best.Min;
	}

	const vec2& GetMax() const
	{
		return Best.Max;
	}


private:

	static vec2 RotatedLeft( vec2 x )
	{
		return vec2( -x[1], x[0] );
	}

	inline vec2 GetEdge( std::size_t i )
	{
		return P[(i+1)%n]-P[i];
	}

	inline void RotateSmallestAngle()
	{
		float ra,ta,la,ba;
		const float ZeroDeg = 1.f - 0.0001f;
		vec2 t = Current.u;
		while ( (ba=dot( t, normalized(GetEdge(Bottom)))) >= ZeroDeg )
			Bottom = (Bottom+1)%n;
		
		t = RotatedLeft( Current.u );
		while ( (ra=dot( t, normalized(GetEdge(Right)))) >= ZeroDeg )
			Right = (Right+1)%n;

		t = RotatedLeft( t );
		while ( (ta=dot( t, normalized(GetEdge(Top)))) >= ZeroDeg)
			Top = (Top+1)%n;

		t = RotatedLeft( t );
		while ( (la=dot( t, normalized(GetEdge(Left)))) >= ZeroDeg )
			Left = (Left+1)%n;

		// find the smallest angle, which is the greatest cosine
		std::size_t s = 0;
		float m = ba;

		if ( ra > m ) { s=1; m=ra; }
		if ( ta > m ) { s=2; m=ta; }
		if ( la > m ) { s=3; m=la; }

		switch( s )
		{
		case 0:
			t = normalized(GetEdge(Bottom));
			Current.u = t;
			break;
		case 1:
			
			t = normalized(GetEdge(Right));
			Current.u.set( t[1],-t[0] );
			break;
		case 2:
			t = normalized(GetEdge(Top));
			Current.u = -t;
			break;
		case 3:
			t = normalized(GetEdge(Left));
			Current.u.set( -t[1], t[0] );
			break;
		}
	}

	inline float ComputeCurrentSize()
	{
		const vec2& u(Current.u);
		// phi is defined by the matrix:
		// u[0] -u[1]
		// u[1]  u[0]

		const vec2& l(P[Left]);
		const vec2& b(P[Bottom]);

		Current.Min.set(
			 l[0]*u[0]+l[1]*u[1], // x component of phi(P[Left])
			-b[0]*u[1]+b[1]*u[0] // y component of phi(P[Bottom])
		);

		// Likewise for right and top
		const vec2& r(P[Right]);
		const vec2& t(P[Top]);

		Current.Max.set(
			 r[0]*u[0]+r[1]*u[1],
			-t[0]*u[1]+t[1]*u[0]
		);

		float dx = Current.Max[0]-Current.Min[0];
		float dy = Current.Max[1]-Current.Min[1];

		return dx*dy;
	}

	inline void InitialExtrema()
	{
		Left = Right = Top = Bottom = 0;

		// Find an initial bounding box and the extrema
		for ( std::size_t i=1; i<n; ++i )
		{
			float x = P[i][0];
			float y = P[i][1];

			// Check for a new x
			if ( x < P[Left][0] )
				Left = i;
			else if ( x > P[Right][0] )
				Right = i;
			else
			{
				if ( x == P[Left][0] && y < P[Left][1] )
					Left = i;

				else if ( x == P[Right][0] && y > P[Right][1] )
					Right = 1;
			}

			// Check for new y
			if ( y < P[Bottom][1] )
				Bottom = i;
			else if ( y > P[Top][1] )
				Top = i;
			else
			{
				if ( y == P[Bottom][1] && x > P[Bottom][0] )
					Bottom = i;

				else if ( y == P[Top][1] && x < P[Top][0] )
					Top = i;
			}
		}
	}

	// The convex hull
	const vec2*		P;
	std::size_t		n;

	struct BoxType {
		// box
		vec2			Min;
		vec2			Max;

		// rotation
		vec2			u; 
	};

	BoxType			Current;
	BoxType			Best;

	// Current extrema
	std::size_t Top;
	std::size_t Bottom;
	std::size_t Left;
	std::size_t Right;

	// Selected edge
	std::size_t SelectedEdge;
};*/

float BoundingRectangleUnderTransformation( const vec2* Points, std::size_t n, const replay::matrix2& M,
										  vec2& Min, vec2& Max )
{
	Min=M*Points[0];
	Max=Min;

	for ( std::size_t i=1; i<n; ++i )
	{
		vec2 p=M*Points[i];
		float x=p[0];
		float y=p[1];

		if ( x < Min[0] )
			Min[0]=x;
		else if ( x > Max[0] )
			Max[0]=x;

		if ( y < Min[1] )
			Min[1]=y;
		else if ( y > Max[1] )
			Max[1]=y;	
	}

	return (Max[0]-Min[0])*(Max[1]-Min[1]);	
}

void MinimalAreaBoundingRectangle( const vec2* ConvexHull, std::size_t n, replay::matrix2& A,
								   vec2& MinOut, vec2& MaxOut )
{
	// simple quadratic algorithm
	// FIXME: use rotating calipers

	float MinimalArea=std::numeric_limits<float>::max();
	vec2 BestEdge;

	for ( std::size_t i=0; i<n; ++i  )
	{
		vec2 Edge=ConvexHull[(i+1)%n]-ConvexHull[i];

		float Length = magnitude(Edge);
		if ( replay::math::fuzzy_zero(Length) )
			continue;

		Edge /= Length;
		
		// make sure the direction is in the first two quadrants
		if ( Edge[1] < 0.f )
			Edge.negate();

		// if the direction is in the first quadrant, use as x, otherwise, use as y
		replay::matrix2 M;
		if ( Edge[0] > 0.f )
		{
			M.set(  Edge[0], Edge[1],
				   -Edge[1], Edge[0] );
		}
		else
		{
			M.set(  Edge[1],-Edge[0],
				    Edge[0], Edge[1] );
		}

		vec2 Min,Max;
		float Area = BoundingRectangleUnderTransformation( ConvexHull, n, M, Min, Max );

		if ( Area < MinimalArea )
		{
			MinOut=Min;
			MaxOut=Max;
			A=M;
			MinimalArea=Area;
		}	
	}
}

template < class T >
inline T* ptr( std::vector<T>& v )
{
	return &(v[0]);
}

template < std::size_t n, class ArrayLike >
bool fuzzy_equals_n( const ArrayLike& lhs, const ArrayLike& rhs )
{
	using namespace replay;

	for ( std::size_t i=0; i<n; ++i )
		if ( !math::fuzzy_equals( lhs[i], rhs[i] ) )
			return false;

	return true;
}

BOOST_AUTO_TEST_CASE( simple_minibox )
{
	using namespace boost::assign;
	using namespace replay;

	std::vector<vec2> Points;
	Points += vec2(0.f,0.f), vec2( 3.f, 0.f ), vec2( 2.f, 1.f ), vec2( 1.f, 1.f ); 

	bounding_rectangle_algorithm Box( ptr(Points), Points.size() );
	matrix2 M = Box.get_matrix();
	vec2 Min = Box.get_min();
	vec2 Max = Box.get_max();

	BOOST_REQUIRE( fuzzy_equals_n<4>( M, matrix2(1.f,0.f,0.f,1.f) ) );
	BOOST_REQUIRE( fuzzy_equals_n<2>( Min, vec2( 0.f, 0.f ) ) );
	BOOST_REQUIRE( fuzzy_equals_n<2>( Max, vec2( 3.f, 1.f ) ) );
}

BOOST_AUTO_TEST_CASE( random_minibox )
{
	using namespace replay;

	const std::size_t test_count = 32;
	typedef boost::uniform_real<float> range_type;
	typedef boost::variate_generator<boost::mt19937&, range_type > variate_type;
	boost::mt19937 rng;

	variate_type random_coord( rng, range_type(-10.f, 10.f) );

	for ( std::size_t k=0; k<test_count; ++k )
	{
		std::vector<vector2f> Points;

		for ( std::size_t i=0; i<32; ++i )
			Points.push_back(vector2f(random_coord(), random_coord()));

		std::size_t n = math::gift_wrap( ptr(Points), Points.size() );
		BOOST_REQUIRE_GE( n, 3U );

		Points.resize( n );


		bounding_rectangle_algorithm RCBox( ptr(Points), n );
		float Size = dot(RCBox.get_max()-RCBox.get_min(), vector2f(1.f));

		// Compute the minimal rectangle with an easiely provable algorithm
		matrix2 M;
		vec2 Min,Max;
		MinimalAreaBoundingRectangle( ptr(Points),n,M,Min,Max);

		float CorrectSize = dot(Max-Min,vector2f(1.f));


		// Allow 5percent error
		BOOST_REQUIRE_CLOSE( Size, CorrectSize, 0.5f );
	}
}
