/*
replay
Software Library

Copyright (c) 2010 Marius Elvert

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.

*/

#include <iostream>
#include <cmath>
#include <replay/vector_math.hpp>
#include <replay/matrix3.hpp>
#include <replay/quaternion.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/array.hpp>
#include <boost/range/iterator_range.hpp>
#include <replay/minimal_sphere.hpp>

namespace {

/** Compute the square distance of a point p to a segment from 0,0 to t.
*/
inline float square_distance_point_segment( const replay::vector2f& t, const replay::vector2f& p )
{
	const float lambda = dot(p,t)/t.squared();

	if ( lambda < 0.f )
		return p.squared();
	else if ( lambda > 1.f )
		return (p-t).squared();
	else
		return (p-(t*lambda)).squared();
}

/** Compute the square distance of a point x to a triangle that has one point in the origin, one in [w,0] and the third in t.
	w has to be positive. Other triangles can be rotated to match this.
*/
inline float square_distance_point_triangle( float w, const replay::vector2f& t, const replay::vector2f& p )
{
	using namespace replay;

	// below the X axis?
	if ( p[1] < 0.f )
	{
		if ( p[0] < 0.f )
			return p.squared(); // lower-left of the triangle
		else if ( p[0] > w )
			return math::square(p[0]-w)+math::square(p[1]); // lower-right of the triangle
		else
			return p[1]*p[1]; // below the triangle
	}

	// outside of the left edge
	if ( math::det(t,p) > 0.f )
	{
		// find distance from the left edge
		return square_distance_point_segment(t,p);
	}
	else // inside the lower and the left edge
	{
		// transform t to the origin
		vector2f dw(w-t[0],-t[1]);
		vector2f dp=p-t;

		// inside all edges - inside the triangle
		if ( math::det(dw,dp) < 0.f )
			return 0.f;

		// find distance from the right edge
		return square_distance_point_segment(dw,dp);
	}	
}

#if 0 // unused
inline float det3( float a, float b, float c,
				   float d, float e, float f,
				   float g, float h, float i )
{
	float r = a*(e*i-f*h);
	r += b*(f*g-d*i);
	r += c*(d*h-e*g);

	return r;
}
#endif

}

void replay::math::minimal_sphere( vector3f* p, std::size_t n, vector3f& m, float& r )
{
	boost::iterator_range<vector3f*> range(p,p+n);
	minimal_ball<float,vector3f,3> mb( range, 1e-15f );
	m = mb.center();
	r = mb.square_radius();
}

void replay::math::decompose_rotational_matrix( const matrix3& m, quaternion& result )
{
	result.w = std::sqrt( std::max( 0.f, 1.f + m(0,0) + m(1,1) + m(2,2) ) ) * 0.5f; 
	result.x = std::sqrt( std::max( 0.f, 1.f + m(0,0) - m(1,1) - m(2,2) ) ) * 0.5f; 
	result.y = std::sqrt( std::max( 0.f, 1.f - m(0,0) + m(1,1) - m(2,2) ) ) * 0.5f; 
	result.z = std::sqrt( std::max( 0.f, 1.f - m(0,0) - m(1,1) + m(2,2) ) ) * 0.5f; 

	result.x = math::copy_sign( result.x, m( 2, 1 ) - m( 1, 2 ) );
	result.y = math::copy_sign( result.y, m( 0, 2 ) - m( 2, 0 ) );
	result.z = math::copy_sign( result.z, m( 1, 0 ) - m( 0, 1 ) );
}

void replay::math::decompose_rotational_matrix( const matrix4& m, quaternion& result )
{
	result.w = std::sqrt( std::max( 0.f, 1.f + m[ 0 ] + m[ 5 ] + m[ 10 ] ) ) * 0.5f; 
	result.x = std::sqrt( std::max( 0.f, 1.f + m[ 0 ] - m[ 5 ] - m[ 10 ] ) ) * 0.5f; 
	result.y = std::sqrt( std::max( 0.f, 1.f - m[ 0 ] + m[ 5 ] - m[ 10 ] ) ) * 0.5f; 
	result.z = std::sqrt( std::max( 0.f, 1.f - m[ 0 ] - m[ 5 ] + m[ 10 ] ) ) * 0.5f; 

	result.x = math::copy_sign( result.x, m[ 6 ] - m[ 9 ] );
	result.y = math::copy_sign( result.y, m[ 8 ] - m[ 2 ] );
	result.z = math::copy_sign( result.z, m[ 1 ] - m[ 4 ] );
}

void replay::math::set_orthographic_matrix( matrix4& matrix, const fcouple& width,
										const fcouple& height, const fcouple& depth )
{
	matrix.set( 2.f / (width[1]-width[0]), 0.f, 0.f, -(width[1]+width[0])/(width[1]-width[0]),
				0.f, 2.f / (height[1]-height[0]), 0.f, -(height[1]+height[0])/(height[1]-height[0]),
				0.f, 0.f, -2.f / (depth[1]-depth[0]), -(depth[1]+depth[0])/(depth[1]-depth[0]),
				0.f, 0.f, 0.f, 1.f );
}

void replay::math::set_perspective_matrix( matrix4& matrix, float fovy,
										float aspect, float near, float far )
{
	const float f = 1.f / std::tan( fovy / 2.f );
	const float d = near - far;
	
	matrix.set( f / aspect, 0.f, 0.f, 0.f,
				0.f, f, 0.f, 0.f,
				0.f, 0.f, (far+near)/d, 2.f*far*near/d,
				0.f, 0.f, -1.f, 0.f );
}

void
replay::math::compute_frustum_corners( const plane3* frustum, vector3f* points )
{
    // LEFT - BOTTOM - NEAR
	points[ 0 ] = math::intersect_3planes( frustum[ 0 ], frustum[ 2 ], frustum[ 4 ] );
	// RIGHT - BOTTOM - NEAR
	points[ 1 ] = math::intersect_3planes( frustum[ 1 ], frustum[ 2 ], frustum[ 4 ] );
	// RIGHT - TOP - NEAR
	points[ 2 ] = math::intersect_3planes( frustum[ 1 ], frustum[ 3 ], frustum[ 4 ] );
	// LEFT - TOP - NEAR
	points[ 3 ] = math::intersect_3planes( frustum[ 0 ], frustum[ 3 ], frustum[ 4 ] );

	// LEFT - BOTTOM - FAR
	points[ 4 ] = math::intersect_3planes( frustum[ 0 ], frustum[ 2 ], frustum[ 5 ] );
	// RIGHT - BOTTOM - FAR
	points[ 5 ] = math::intersect_3planes( frustum[ 1 ], frustum[ 2 ], frustum[ 5 ] );
	// RIGHT - TOP - FAR
	points[ 6 ] = math::intersect_3planes( frustum[ 1 ], frustum[ 3 ], frustum[ 5 ] );
	// LEFT - TOP - FAR
	points[ 7 ] = math::intersect_3planes( frustum[ 0 ], frustum[ 3 ], frustum[ 5 ] );
}										
										
replay::vector3f
replay::find_closest_point( const line3& line, const vector3f& point )
{
	const float lambda = dot(point-line.origin, line.direction) / line.direction.squared();

	return line.get_point( lambda );
}

float
replay::square_distance( const line3& line, const vector3f& point )
{
	return ( point - find_closest_point( line, point ) ).squared();
}

float
replay::square_distance( const vector3f& point0, const vector3f& point1 )
{
	return ( point1 - point0 ).squared();
}

float
replay::distance( const plane3& p, const vector3f& point )
{
	return dot(p.normal, point) + p.d;
}

float
replay::distance( const line3& line, const vector3f& point )
{
	return std::sqrt(square_distance(line, point));
}

float
replay::magnitude( const vector2f& vector )
{
	return std::sqrt(vector.squared());
}

float
replay::math::max_norm( const vector2f& vector )
{
	return std::max(std::abs(vector[0]), std::abs(vector[1]));
}

float
replay::magnitude( const vector3f& vector )
{
	return std::sqrt(vector.squared());
}

float
replay::magnitude( const vector4f& vector )
{
	return std::sqrt(vector.squared());
}

float
replay::math::max_norm( const vector3f& vector )
{
	return std::max(std::max(std::abs(vector[0]),
		std::abs(vector[1])), std::abs(vector[2]));
}

void
replay::normalize( vector4f& vector )
{
	vector /= magnitude( vector );
}

replay::vector4f
replay::normalized( const vector4f& vector )
{
	return vector / magnitude( vector );
}

void
replay::normalize( vector3f& vector )
{
	vector /= magnitude( vector );
}

replay::vector3f
replay::normalized( const vector3f& vector )
{
	return vector / magnitude( vector );
}

void
replay::normalize( vector2f& vector )
{
	vector /= magnitude( vector );
}

replay::vector2f
replay::normalized( const vector2f& vector )
{
	return vector / magnitude( vector );
}

replay::vector2f
replay::math::angle_vector( const float radians )
{
	return vector2f( std::cos( radians ), std::sin( radians ) );
}

bool
replay::math::intersection_test::line_triangle( const linear_component3& line, const vector3f& t0, const vector3f& t1, const vector3f& t2,
										  float* lambda, fcouple* barycentrics, float epsilon )
{
	// find vectors for two edges sharing v0
	const vector3f edge0 = t1 - t0;
	const vector3f edge1 = t2 - t0;

	const vector3f normal = cross(edge0, edge1);

	// Backface culling
	if ( dot(normal, line.direction) > 0.0f )
		return false;

	// begin calculating determinant - also used to calculate u parameter
	const vector3f perp = cross(line.direction, edge1);

	// if determinant is near zero, ray lies in plane of triangle
	float determinant = dot(edge0, perp);
     
	if ( std::abs(determinant) < epsilon )
		return false;

	// inverse determinant
	determinant = 1.0f / determinant;

	// calculate distance from vert0 to ray origin
	const vector3f delta = line.origin - t0;

	// calculate u parameter and test bounds
	const float u = dot(delta, perp) * determinant;

	if ( u < 0.0f || u > 1.0f )
		return false;

	// prepare to test v parameter
	const vector3f temp = cross(delta, edge0);

	// calculate v parameter and test bounds
	const float v = dot(line.direction, temp) * determinant;
	if ( v < 0.0f || (u + v) > 1.0f )
		return false;

	// store the barycentric coords
	if ( barycentrics )
		barycentrics->set( u, v );

	// calculate lambda as ray intersects triangle
	if ( lambda )
	{
		*lambda = dot(edge1, temp) * determinant;
	}

	return true;
}

bool
replay::math::intersection_test::line_sphere( const linear_component3& line, const vector3f& center, const float radius,
								float* lambda0, float* lambda1 , float epsilon )
{
	const vector3f	v0 = line.origin - center;
	fcouple			result;
	float square_radius = radius*radius;

	// <d,d>
	float dd = line.direction.squared();
	// <d,a-c>
	float num = dot(line.direction, v0);

	// check point of closest approach before solving the quadratic equation
	float lambda = -num/dd;

	// out of reach?
	if ( (v0+lambda*line.direction).squared() > square_radius )
		return false;

	unsigned int result_count = math::solve_quadratic_eq( dd, 2.f*num,
		v0.squared()-square_radius, result, epsilon );

	if ( result_count == 2 )
	{
		if ( lambda0 )
			*lambda0 = result[ 0 ];

		if ( lambda1 )
			*lambda1 = result[ 1 ];

		return true;
	}

	return false;
}
void replay::math::extract_frustum_sides( const matrix4& scene, plane3* frustum )
{

	// LEFT - FIXME
	frustum[ 0 ].set( 
		scene[ 3] + scene[ 0],
		scene[ 7] + scene[ 4],
		scene[11] + scene[ 8],
		scene[15] + scene[12] );

	// RIGHT - FIXME
	frustum[ 1 ].set( 
		scene[ 3] - scene[ 0],
		scene[ 7] - scene[ 4],
		scene[11] - scene[ 8],
		scene[15] - scene[12] );


	// BOTTOM
	frustum[ 2 ].set(
		scene[ 3] + scene[ 1],
		scene[ 7] + scene[ 5],
		scene[11] + scene[ 9],
		scene[15] + scene[13] );

	// TOP
	frustum[ 3 ].set(
		scene[ 3] - scene[ 1],
		scene[ 7] - scene[ 5],
		scene[11] - scene[ 9],
		scene[15] - scene[13] );

	frustum[ 0 ].hnf();
	frustum[ 1 ].hnf();
	frustum[ 2 ].hnf();
	frustum[ 3 ].hnf();
}

void replay::math::extract_frustum( const matrix4& scene, plane3* frustum )
{
	extract_frustum_sides( scene, frustum );

	// NEAR
	frustum[ 4 ].set(
		scene[ 3] + scene[ 2],
		scene[ 7] + scene[ 6],
		scene[11] + scene[10],
		scene[15] + scene[14] );


	// FAR
	frustum[ 5 ].set(
		scene[ 3] - scene[ 2],
		scene[ 7] - scene[ 6],
		scene[11] - scene[10],
		scene[15] - scene[14] );

	frustum[ 4 ].hnf();
	frustum[ 5 ].hnf();

}

unsigned int replay::math::convex_hull_contains( vector2f* hull, unsigned int hullsize,
												const vector2f& point, const float threshold )
{
	for ( unsigned int i = 0; i < hullsize; ++i )
	{
		const unsigned int next = (i + 1) % hullsize;

		if ( math::det( hull[ next ]-hull[ i ], point - hull[ i ] ) < threshold )
			return 0;
	}

	return 1;
}

const replay::vector3f replay::math::construct_perpendicular( const vector3f& x )
{
    vector3f result;
	std::size_t p = 0;
	float m = std::abs(x[0]);

	// Find the maximum element
	for (std::size_t i=1; i<3; ++i)
	{
		register float t = std::abs(x[i]);
		if (m < t)
		{
			m = t;
			p = i;
		}
	}

	int q = (p+1)%3;
	int r = (q+1)%3;

	result[p] = -x[q];
	result[q] = x[p];
	result[r] = 0.f;

	return result;
}

std::size_t replay::math::gift_wrap( vector2f* point, std::size_t n )
{
	// No need to construct a hull
	if ( n < 3 )
		return n;

	// Find the starting point on the convex hull
	std::size_t candidate = std::min_element( point, point+n, array_less<2>() ) - point;
	std::swap( point[0], point[candidate] ); // move it to the front

	// Find all other points
	for ( std::size_t k=1; k<n; ++k )
	{
		candidate=k;
		for ( std::size_t i=k+1; i<n; ++i )
			if ( replay::math::det( point[candidate]-point[k-1], point[i]-point[candidate] ) < 0.f )
				candidate=i;

		// Try to close the hull
		if ( k > 1 && replay::math::det( point[candidate]-point[k-1], point[0]-point[candidate] ) < 0.f )
			return k;

		std::swap( point[k], point[candidate] );
	}

	return n;
}

replay::vector3f replay::math::intersect_3planes( const plane3& a, const plane3& b, const plane3& c )
{
	matrix3	Temp(
		a.normal[ 0 ], a.normal[ 1 ], a.normal[ 2 ],
		b.normal[ 0 ], b.normal[ 1 ], b.normal[ 2 ],
		c.normal[ 0 ], c.normal[ 1 ], c.normal[ 2 ] );

	Temp.invert();

	return Temp * -vector3f( a.d, b.d, c.d );
}


bool replay::math::intersect_line2( const line2& a, const line2& b, vector2f& result )
{
	float denom = det(a.direction,b.direction);

	// lines parallel?
	if ( fuzzy_zero( denom ) )
		return false;

	float num = det(b.origin-a.origin,b.direction);
	float lambda = num/denom;

	result = a.get_point(lambda);
	return true;
}

float replay::square_distance( const line3& la, const line3& lb )
{
	vector3f comp = cross( la.direction, lb.direction );
	
	float length = magnitude(comp);

	// parallel or equal
	if ( math::fuzzy_zero(length) )
	{
		// find a reference point 
		vector3f ref = find_closest_point( la, lb.origin );

		return square_distance(ref,lb.origin);
	}
	
	// compute the plane intersection
	vector3f n = cross(comp, lb.direction);

	float lambda = dot(n, lb.origin-la.origin) / dot(n, la.direction);

	// find the reference point on the first line
	vector3f pa = la.get_point(lambda);

	// find the reference point on the second line
	vector3f pb = find_closest_point(lb, pa);

	return square_distance(pa,pb);
}

float replay::square_distance( const vector3f& point, const boost::array<vector3f,3>& triangle )
{
	const vector3f u = triangle[1]-triangle[0];
	const vector3f v = triangle[2]-triangle[0];

	const vector3f p = point-triangle[0];

	float u_length = magnitude(u);

	// build and orthogonal base to translate this to 2d
	const vector3f n = normalized(cross(u,v));
	const vector3f t = u/u_length;
	const vector3f b = cross(n,t);

	float sqr_plane_distance = math::square(dot(p,n));
	
	return sqr_plane_distance + square_distance_point_triangle( u_length, 
		vector2f(dot(v,t),dot(v,b)), vector2f(dot(p,t),dot(p,b)) );
}


replay::vector3f replay::math::lup::solve( matrix3 m, const vector3f& rhs )
{
	vector3<std::size_t> permutation;

	decompose( m, permutation );
	return solve( m, permutation, rhs );
}

replay::vector3f replay::math::lup::solve( const matrix3& lu, const vector3<std::size_t>& p, const vector3f& rhs )
{
	vector3f y;

	y[0] = rhs[p[0]];
	y[1] = rhs[p[1]] - lu(1,0)*y[0];
	y[2] = rhs[p[2]] - lu(2,0)*y[0] - lu(2,1)*y[1];

	vector3f x;

	x[2] = (y[2]) / lu(2,2);
	x[1] = (y[1] - x[2]*lu(1,2)) / lu(1,1);
	x[0] = (y[0] - x[1]*lu(0,1) - x[2]*lu(0,2)) / lu(0,0);

	return x;
}

bool replay::math::lup::decompose( matrix3& m, vector3<std::size_t>& p, float epsilon )
{
	std::size_t best=0;
	p.reset(0,1,2);

	// find the pivot in the first row
	if (std::abs(m(1,0)) > std::abs(m(2,0)))
	{
		if (std::abs(m(1,0)) > std::abs(m(0,0))) // row 1 has the biggest element
		{
			best = 1;
			m.swap_rows(0,1);
		}
	}
	else
	{
		if (std::abs(m(2,0)) > std::abs(m(0,0))) // row 2 has the pivot
		{
			best = 2;
			m.swap_rows(0,2);
		}
	}

	// Keep a record of this permutation
	p[0] = best;
	p[best] = 0;

	// Bump out if the first divisor is too small
	float d=m(0,0);
	if ( fuzzy_zero(d,epsilon) )
		return false;

	m(1,0) /= d;
	m(2,0) /= d;

	m(1,1) -= m(1,0)*m(0,1); m(1,2) -= m(1,0)*m(0,2);
	m(2,1) -= m(2,0)*m(0,1); m(2,2) -= m(2,0)*m(0,2);

	// find the new pivot
	if (std::abs(m(1,1)) < std::abs(m(2,1)))
	{
		// swap rows again
		m.swap_rows( 1, 2 );
		std::swap( p[1],p[2] );
	}

	if ( fuzzy_zero(m(1,1),epsilon) )
		return false;

	m(2,1) /= m(1,1);
	m(2,2) -= m(2,1)*m(1,2);

	return true;
}

boost::optional<replay::matrix4> replay::cramer_invert(const replay::matrix4& m, double epsilon)
{
	double inv[16];

	inv[0] = m[5]*m[10]*m[15] - m[5]*m[11]*m[14] - m[9]*m[6]*m[15]
		+ m[9]*m[7]*m[14] + m[13]*m[6]*m[11] - m[13]*m[7]*m[10];
	inv[4] =-m[4]*m[10]*m[15] + m[4]*m[11]*m[14] + m[8]*m[6]*m[15]
		- m[8]*m[7]*m[14] - m[12]*m[6]*m[11] + m[12]*m[7]*m[10];
	inv[8] = m[4]*m[9]*m[15] - m[4]*m[11]*m[13] - m[8]*m[5]*m[15]
		+ m[8]*m[7]*m[13] + m[12]*m[5]*m[11] - m[12]*m[7]*m[9];
	inv[12] = -m[4]*m[9]*m[14] + m[4]*m[10]*m[13] + m[8]*m[5]*m[14]
		- m[8]*m[6]*m[13] - m[12]*m[5]*m[10] + m[12]*m[6]*m[9];
	inv[1] =  -m[1]*m[10]*m[15] + m[1]*m[11]*m[14] + m[9]*m[2]*m[15]
		- m[9]*m[3]*m[14] - m[13]*m[2]*m[11] + m[13]*m[3]*m[10];
	inv[5] =   m[0]*m[10]*m[15] - m[0]*m[11]*m[14] - m[8]*m[2]*m[15]
		+ m[8]*m[3]*m[14] + m[12]*m[2]*m[11] - m[12]*m[3]*m[10];
	inv[9] =  -m[0]*m[9]*m[15] + m[0]*m[11]*m[13] + m[8]*m[1]*m[15]
		- m[8]*m[3]*m[13] - m[12]*m[1]*m[11] + m[12]*m[3]*m[9];
	inv[13] =  m[0]*m[9]*m[14] - m[0]*m[10]*m[13] - m[8]*m[1]*m[14]
		+ m[8]*m[2]*m[13] + m[12]*m[1]*m[10] - m[12]*m[2]*m[9];
	inv[2] =   m[1]*m[6]*m[15] - m[1]*m[7]*m[14] - m[5]*m[2]*m[15]
		+ m[5]*m[3]*m[14] + m[13]*m[2]*m[7] - m[13]*m[3]*m[6];
	inv[6] =  -m[0]*m[6]*m[15] + m[0]*m[7]*m[14] + m[4]*m[2]*m[15]
		- m[4]*m[3]*m[14] - m[12]*m[2]*m[7] + m[12]*m[3]*m[6];
	inv[10] =  m[0]*m[5]*m[15] - m[0]*m[7]*m[13] - m[4]*m[1]*m[15]
		+ m[4]*m[3]*m[13] + m[12]*m[1]*m[7] - m[12]*m[3]*m[5];
	inv[14] = -m[0]*m[5]*m[14] + m[0]*m[6]*m[13] + m[4]*m[1]*m[14]
		- m[4]*m[2]*m[13] - m[12]*m[1]*m[6] + m[12]*m[2]*m[5];
	inv[3] =  -m[1]*m[6]*m[11] + m[1]*m[7]*m[10] + m[5]*m[2]*m[11]
		- m[5]*m[3]*m[10] - m[9]*m[2]*m[7] + m[9]*m[3]*m[6];
	inv[7] =   m[0]*m[6]*m[11] - m[0]*m[7]*m[10] - m[4]*m[2]*m[11]
		+ m[4]*m[3]*m[10] + m[8]*m[2]*m[7] - m[8]*m[3]*m[6];
	inv[11] = -m[0]*m[5]*m[11] + m[0]*m[7]*m[9] + m[4]*m[1]*m[11]
		- m[4]*m[3]*m[9] - m[8]*m[1]*m[7] + m[8]*m[3]*m[5];
	inv[15] =  m[0]*m[5]*m[10] - m[0]*m[6]*m[9] - m[4]*m[1]*m[10]
		+ m[4]*m[2]*m[9] + m[8]*m[1]*m[6] - m[8]*m[2]*m[5];

	double det = m[0]*inv[0] + m[1]*inv[4] + m[2]*inv[8] + m[3]*inv[12];

	if (std::abs(det) <= epsilon)
		return boost::none;

	det = 1.0 / det;
	
	matrix4 result((uninitialized_tag()));
	for (int i = 0; i < 16; i++)
		result[i] = static_cast<float>(inv[i] * det);

	return result;
}
