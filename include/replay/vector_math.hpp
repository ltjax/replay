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

#ifndef replay_vector_math_hpp
#define replay_vector_math_hpp

#include <iosfwd>
#include <algorithm>
#include <boost/tuple/tuple.hpp>
#include <boost/array.hpp>
#include <replay/math.hpp>
#include <replay/vector2.hpp>
#include <replay/vector3.hpp>
#include <replay/lines3.hpp>
#include <replay/matrix3.hpp>
#include <replay/matrix4.hpp>
#include <replay/plane3.hpp>

namespace replay {

	/** A two dimensional parametric line.
		\ingroup Math
	*/
	class line2
	{
	public:
		vector2f origin; /**< origin of the line. */
		vector2f direction; /**< direction of the line. */

		/** Create a degenerated line.
		*/
		line2() {}
		
		/** Create a line by setting the origin and direction.
		*/
		line2( const vector2f& p, const vector2f& d ) : origin( p ), direction( d ) {}

		/** Get a point on the line.
		*/
		inline vector2f			get_point( float x ) const { return direction*x + origin; }
	};

	
	/** Plane by matrix multiplication. Treats the the plane as a left sided row vector.
		\ingroup Math
	*/
	inline plane3 operator*( const plane3& l, const matrix4& r )
	{
		return plane3( 
			  l.normal[ 0 ] * r[  0 ] + l.normal[ 1 ] * r[  1 ]
			+ l.normal[ 2 ] * r[  2 ] + l.d *           r[  3 ],
			  l.normal[ 0 ] * r[  4 ] + l.normal[ 1 ] * r[  5 ]
			+ l.normal[ 2 ] * r[  6 ] + l.d *           r[  7 ],
			  l.normal[ 0 ] * r[  8 ] + l.normal[ 1 ] * r[  9 ]
			+ l.normal[ 2 ] * r[ 10 ] + l.d *           r[ 11 ],
			  l.normal[ 0 ] * r[ 12 ] + l.normal[ 1 ] * r[ 13 ]
			+ l.normal[ 2 ] * r[ 14 ] + l.d *           r[ 15 ] );
	}

	/** Component wise less predicate.
		Does a lexical comparision between constant-size arrays, for example n-dimensional vectors.
		The type to be used only needs have operator[] available.
		\ingroup Math
	*/
	template < class ArrayType, std::size_t n >
	class array_less
	{
	public:
		/** Evalutate the predicate.
			\param lhs The left-hand side of the operator.
			\param rhs The right-hand side of the operator.
			\returns The result of the comparison.
		*/
		bool operator()( const ArrayType& lhs, const ArrayType& rhs ) const
		{
			for ( std::size_t i=0; i<n; ++i )
			{
				if ( lhs[i] < rhs[i] )
					return true;
				else if ( lhs[i] > rhs[i] )
					return false;
			}

			return false;
		}
	};

namespace math {



	/** compute the length of a vector in manhattan norm. (max-norm)
		\ingroup Math
	*/
	float						max_norm( const vector2f& vector );
	
	/** compute the length of a vector in manhatten norm. (max-norm)
		\ingroup Math
	*/
	float						max_norm( const vector3f& vector );


	/** Decompose a rotational matrix into a quaternion. Effectively a conversion between the two.
		\param m Matrix to decompose.
		\param result Quaternion to store the resulting rotation.
		\ingroup Math
	*/
	void						decompose_rotational_matrix( const matrix3& m, quaternion& result ); 

	/** Decompose the rotational part of a 4x4 matrix into a quaternion. The top left 3x3 part of the matrix is converted to a quaternion.
		\param m Matrix to decompose.
		\param result Quaternion to store the resulting rotation.
		\ingroup Math
	*/
	void						decompose_rotational_matrix( const matrix4& m, quaternion& result ); 

	/** Compute the determinante of a 2x2 matrix given as two vectors.
		\ingroup Math
	*/
	inline float				det( const vector2f& a, const vector2f& b )
	{
		return a[ 0 ] * b[ 1 ] - a[ 1 ] * b[ 0 ];
	}

	/** Compute the determinante of a 2x2 matrix given as 4 values.
		\ingroup Math
	*/
	inline float				det( const float a, const float b, const float c, const float d )
	{
		return a*d - b*c;
	}

	
	/** Setup a perspective matrix for homogenous coordinates.
		\ingroup Math
	*/
	void						set_perspective_matrix( matrix4& matrix, float fovy,
										float aspect, float near, float far );

	/** Setup a perspective matrix for homogenous coordinates.
		\ingroup Math
	*/
	inline matrix4				make_perspective_matrix( float fovy,
										float aspect, float near, float far )
	{
		matrix4 result;
		set_perspective_matrix( result, fovy, aspect, near, far );
		return result;
	}

	/** Setup an orthographic matrix for homogenous coordinates.
		\ingroup Math
	*/
	void						set_orthographic_matrix( matrix4& matrix, const fcouple& width,
										const fcouple& height, const fcouple& depth );

	/** Setup an orthographic matrix for homogenous coordinates.
		\ingroup Math
	*/
	inline matrix4				make_orthographic_matrix( const fcouple& width,
										const fcouple& height, const fcouple& depth )
	{
		matrix4 result;
		set_orthographic_matrix( result, width, height, depth );
		return result;
	}

	/** Construct a vector that is perpendicular to the given one.
		In general, the resulting vector will be shorter than the given one.
		\ingroup Math
	*/
	const vector3f				construct_perpendicular( const vector3f& x );


	/** vector component wise mult by sign.
		\ingroup Math
	*/
	inline vector3f				mult_by_sign( const vector3f& x, float sign )
	{
		vector3f result;
		mult_by_sign( x[ 0 ], sign, result[ 0 ] );
		mult_by_sign( x[ 1 ], sign, result[ 1 ] );
		mult_by_sign( x[ 2 ], sign, result[ 2 ] );
		return result;
	}
	
	/** vector component wise min.
		\ingroup Math
	*/
	template < class x > vector2< x >	vector_min( const vector2< x >& a, const vector2< x >& b )
		{ return vector2< x >( std::min( a[ 0 ], b[ 0 ] ), std::min( a[ 1 ], b[ 1 ] ) ); }

	/** vector component wise max.
		\ingroup Math
	*/
	template < class x > vector2< x >	vector_max( const vector2< x >& a, const vector2< x >& b )
		{ return vector2< x >( std::max( a[ 0 ], b[ 0 ] ), std::max( a[ 1 ], b[ 1 ] ) ); }

	/** vector component wise min.
		\ingroup Math
	*/
	template < class x > vector3< x >	vector_min( const vector3< x >& a, const vector3< x >& b )
		{ return vector3< x >( std::min( a[ 0 ], b[ 0 ] ), std::min( a[ 1 ], b[ 1 ] ), std::min( a[ 2 ], b[ 2 ] ) ); }

	/** vector component wise max.
		\ingroup Math
	*/
	template < class x > vector3< x >	vector_max( const vector3< x >& a, const vector3< x >& b )
		{ return vector3< x >( std::max( a[ 0 ], b[ 0 ] ), std::max( a[ 1 ], b[ 1 ] ), std::max( a[ 2 ], b[ 2 ] ) ); }

	/** splice off the first two elements.
		\ingroup Math
	*/
	template < class x > vector2< x >	splice2( const vector3< x >& v )
		{ return vector2< x >( v[ 0 ], v[ 1 ] ); }
	
	/** Computes a 2d vector pointing at the given angle relative to x+.
		\param radians Angle in radians.
		\ingroup Math
	*/
	vector2f					angle_vector( const float radians );

	/** Find the intersection point of the 3 given planes, if such a point exists.
		\ingroup Math
	*/
	vector3f					intersect_3planes( const plane3& a, const plane3& b, const plane3& c );

	/** Extract the view frustum planes from a given scene matrix.
		The normals of the resulting planes are pointing inwards, i.e. the intersection of all positive sides is 'in'.
		\param scene The scene matrix is Projection * Modelview
		\param frustum Array of the 6 frustum planes, left, right, bottom, top, near, far in that order.
		\ingroup Math
	*/
	void						extract_frustum( const matrix4& scene, plane3* frustum );

	/** Find the frustum corners from the 6 given frustum points.
		\ingroup Math
	*/
	void						compute_frustum_corners( const plane3* frustum, vector3f* points );

	
	/** Extract the view frustum side planes from a given scene matrix.
		\param scene The scene matrix is Projection * Modelview
		\param frustum Array of the 4 frustum planes, left, right, bottom, top in that order.
		\ingroup Math
	*/
	void						extract_frustum_sides( const matrix4& scene, plane3* frustum );

	/** Find a 2D convex hull of a set of 2d vectors using the gift wrap algorithm.
		The points are reordered so that the first points make up a convex hull of the set.
		The resulting hull is in counter clockwise order.
		This algorithm is output sensitive in its runtime: it will perform a number of n*k steps,
		where n is the number of points, and k is the number of points on the convex hull.
		\param points The points to find the convex hull of.
		\param count The number of points in the set.
		\return The number of points that make up the convex hull.
		\ingroup Math
	*/
	std::size_t					gift_wrap( vector2f* points, std::size_t count );


	/** Checks whether the given point is inside the given convex hull.
	*/
	unsigned int				convex_hull_contains( vector2f* hull, unsigned int hullsize,
									const vector2f& point, const float threshold );

	/** Compute the intersection of two 2d lines.
		\ingroup Math
	*/
	bool						intersect_line2( const line2& a, const line2& b, vector2f& result );


	/** Construct the minimal sphere containing a set of points using Welzl's algorithm.
		Expected linear runtime.
		\note This will permute the points in the given array.
		\param p Array of points.
		\param n Number of points in the array.
		\param m Center of the computed sphere.
		\param r Squared radius of the computed sphere.
	*/
	void						minimal_sphere( vector3f* p, std::size_t n, vector3f& m, float& r );



	/** Primitive intersection tests.
		\ingroup Math
	*/
	namespace intersection_test {

		/** Test for intersection of a line and a triangle.
			\ingroup Math
		*/
		bool		line_triangle( const linear_component3& line, const vector3f& t0, const vector3f& t1, const vector3f& t2,
								float* lambda = 0, fcouple* barycentrics = 0, float epsilon = default_epsilon );

		/** Test for intersection of a line and a sphere.
			\note This purposefully ignores differences between line, ray and segment and treats all as lines.
			\param line Linear component to test.
			\param center Center of the sphere.
			\param radius Radius of the sphere.
			\param lambda0 Line parameter for the first intersection.
			\param lambda1 Line parameter for the second intersection.
			\param epsilon Error tolerance to use for the quadratic root finder.
			\ingroup Math
		*/
		bool		line_sphere( const linear_component3& line, const vector3f& center, const float radius,
								float* lambda0 = 0, float* lambda1 = 0, float epsilon = default_epsilon );

	}
}
	
/** Compute the scalar product of two 2d vectors.
	\ingroup Math
*/
inline float				dot( const vector2f& a, const vector2f& b )
{
	return a[ 0 ] * b[ 0 ] + a[ 1 ] * b[ 1 ];
}

/** Compute the scalar product of two 3d vectors.
	\ingroup Math
*/
inline float				dot( const vector3f& a, const vector3f& b )
{
	return a|b;
}

/** Compute the scalar product of two 4d vectors.
	\ingroup Math
*/
inline float				dot( const vector4f& a, const vector4f& b )
{
	return a|b;
}

/** find the closest point.
	\param line line on which to look for the closest point.
	\param point where to find the closest point to.
	\ingroup Math
*/
vector3f					find_closest_point( const line3& line, const vector3f& point );

/** Compute the square distance of a point to a polygon.
*/
float						square_distance( const vector3f& point, const boost::array<vector3f,3>& triangle );

/** finds the square of the euclidean distance of a line and a point.
	\ingroup Math
*/
float						square_distance( const line3& line, const vector3f& point );

/** Compute the square of the euclidean distance of two points.
	\ingroup Math
*/
float						square_distance( const vector3f& point0, const vector3f& point1 );

/** Compute the square of the euclidean distance of two 3d lines.
*/
float						square_distance( const line3& la, const line3& lb );

/** Compute the euclidean distance of a line and a point.
	\ingroup Math
*/
float						distance( const replay::line3& line, const replay::vector3f& point );

/** Compute the signed distance of a plane and a point. 
	If the plane's normal is unit-length, the absolute of this
	distance is the euclidean distance.
	\ingroup Math
*/
float						distance( const replay::plane3& p, const replay::vector3<float>& point );


/** Compute the euclidean distance of two points.
	\ingroup Math
*/
inline float				distance( const replay::vector3f& lhs, const replay::vector3f& rhs ) { return std::sqrt(square_distance(lhs,rhs)); }

/** Compute the euclidean length of a vector.
	\ingroup Math
*/
float						magnitude( const vector2f& vector );

/** Compute the euclidean length of a vector.
	\ingroup Math
*/
float						magnitude( const vector3f& vector );

/** normalize the given vector.
	\ingroup Math
*/
void						normalize( vector3f& vector );

/** normalize the given vector.
	\ingroup Math
*/
void						normalize( vector2f& vector );

/** return a normalized vector.
	\ingroup Math
*/
vector3f					normalized( const vector3f& vector );

/** return a normalized vector.
	\ingroup Math
*/
vector2f					normalized( const vector2f& vector );

/** vector2f stream-out operator.
*/
std::ostream&				operator<<( std::ostream& cout, const replay::vector2f& v );

/** vector3f stream-out operator.
*/
std::ostream&				operator<<( std::ostream& cout, const replay::vector3f& v );

}

#endif // replay_vector_math_hpp
