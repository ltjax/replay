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

#include <limits>
#include <cmath>
#include <replay/aabb.hpp>
#include <replay/math.hpp>

/** Create a new empty box that contains nothing.
*/
replay::aabb::aabb( )
{
	clear();
}

/** Create a new box that extends in all directions from the origin.
*/
replay::aabb::aabb( const float x )
: base_class( vector3f( -x ), vector3f( x ) )
{
}

/** Create a new box from range.
*/
replay::aabb::aabb( const vector3f& min, const vector3f& max )
: base_class( min, max )
{
}

/** Clear the box to contain nothing.
	To be more precise the lower bounds will be MAX_FLOAT and the upper bounds will be -MAX_FLOAT.
*/
replay::aabb& replay::aabb::clear()
{
	(*this)[ 0 ].set( std::numeric_limits<float>::max() );
	(*this)[ 1 ].set( -std::numeric_limits<float>::max() );

	return *this;
}

/** Compute the square distance to the box using Arvo's algorithm.
*/
float replay::aabb::square_distance( const vector3f& other ) const
{
	// Arvo's Algorithm
    float result = 0.f;
	float temp;

    for( unsigned int i=0 ; i < 3 ; ++i )
    {
		if( other[ i ] < (*this)[ 0 ][ i ] )
        {
            temp = other[ i ] - (*this)[ 0 ][ i ];
            result += temp * temp; 
        }

		else if( other[ i ] > (*this)[ 1 ][ i ] )
        {
            temp = other[ i ]  - (*this)[ 1 ][ i ];
            result += temp * temp; 
        }

    }
    return result;
}

/** Compute the euclidean distance to the box using Arvo's algorithm.
*/
float replay::aabb::distance( const vector3f& other ) const
{
	return std::sqrt( square_distance( other ) );
}

/** Project the box onto a vector. 
	\param x The vector that the box is supposed to be projected onto.
	\returns a An ordered range of the box projected onto the vector.
*/
replay::fcouple replay::aabb::project( const vector3f& x ) const
{
	fcouple result;
	result.set( 0.f, 0.f );

	for ( unsigned int i = 0; i < 3; ++i )
	{
		unsigned int mask = math::sign( x[i] );

		result[ mask   ] += (*this)[ 0 ][ i ] * x[ i ];
		result[ mask^1 ] += (*this)[ 1 ][ i ] * x[ i ];
	}

	if ( result[ 0 ] > result[ 1 ] )
		result.swap();

	return result;
}

/** Classify the box in respect to a plane.
*/
replay::aabb::clsfctn replay::aabb::classify( const plane3& x ) const
{
	unsigned int mask;
	fcouple result;

	result.set( x.d, x.d );

	for ( unsigned int i = 0; i < 3; ++i )
	{
		mask = math::sign( x.normal[ i ] );

		result[ mask   ] += (*this)[ 0 ][ i ] * x.normal[ i ];
		result[ mask^1 ] += (*this)[ 1 ][ i ] * x.normal[ i ];
	}

	if ( result[ 0 ] > result[ 1 ] )
		result.swap();

	if ( !math::same_sign( result[ 0 ], result[ 1 ] ) )
		return spanning;
	else if ( result[ 1 ] > 0.f )
		return positive;
	else
		return negative;
}

/** Move the box.
*/
replay::aabb& replay::aabb::move( const vector3f& delta )
{
	get0() += delta;
	get1() += delta;
	return *this;
}

/** Enlarge the aabb to contain another aabb.
*/
replay::aabb& replay::aabb::insert( const aabb& x )
{
	for ( unsigned int i = 0; i < 3; ++i )
	{
		if ( x[ 0 ][ i ] < (*this)[ 0 ][ i ] )
			(*this)[ 0 ][ i ] = x[ 0 ][ i ];
		if ( x[ 1 ][ i ] > (*this)[ 1 ][ i ] )
			(*this)[ 1 ][ i ] = x[ 1 ][ i ];
	}
	
	return *this;
}

/** Enlarge the aabb to contain the array of points.
*/
replay::aabb& replay::aabb::insert( const vector3f* points, unsigned int count )
{
	if ( !count )
		return *this;

	float t = 0.f;
	
	// insert first point explicitly to assert max_i > min_i
	insert(points[0]);

	for ( unsigned int i=1; i < count; ++i )
	{
		for ( unsigned int j = 0; j < 3; ++j )
		{
			t = points[ i ][ j ];

			if ( t < (*this)[ 0 ][ j ] )
				(*this)[ 0 ][ j ] = t;
			else if (t > (*this)[ 1 ][ j ] )
				(*this)[ 1 ][ j ] = t;
		}
	}

	return *this;
}

/** Enlarge the aabb to contain the indexed points from the array.
*/
replay::aabb& replay::aabb::insert( const vector3f* points, const unsigned int* indices, unsigned int count )
{
	if ( !count )
		return *this;

	float t = 0.f;

	// insert first point explicitly to assert max_i > min_i
	insert(points[indices[0]]);

	for ( unsigned int i=1; i<count; ++i )
	{
		for ( unsigned int j = 0; j < 3; ++j )
		{
			t = points[ indices[ i ] ][ j ];

			if ( t < (*this)[ 0 ][ j ] )
				(*this)[ 0 ][ j ] = t;
			else if (t > (*this)[ 1 ][ j ] )
				(*this)[ 1 ][ j ] = t;
		}
	}

	return *this;
}

/** Enlarge the aabb to contain the indexed points from the array.
*/
replay::aabb& replay::aabb::insert( const vector3f* points, const unsigned short* indices, unsigned int count )
{
	if ( !count )
		return *this;

	float t = 0.f;
	
	// insert first point explicitly to assert max_i > min_i
	insert(points[indices[0]]);

	for ( unsigned int i=1; i<count; ++i )
	{
		for ( unsigned int j=0; j<3; ++j )
		{
			t = points[ indices[ i ] ][ j ];

			if ( t < (*this)[ 0 ][ j ] )
				(*this)[ 0 ][ j ] = t;
			else if (t > (*this)[ 1 ][ j ] )
				(*this)[ 1 ][ j ] = t;
		}
	}

	return *this;
}

/** the arvo vector is the difference of point and the closest point on the interval to it.
*/
replay::vector3f& replay::aabb::compute_arvo_vector( const vector3f& point, vector3f& result ) const
{
	for ( unsigned int i = 0; i < 3; ++i )
	{
		if ( point[ i ] < get0()[ i ] )
			result[ i ] = point[ i ] - get0()[ i ];
		else if ( point[ i ] > get1()[ i ] )
			result[ i ] = point[ i ] - get1()[ i ];
		else
			result[ 0 ];
	}

	return result;
}

/** Compute the center of the box.
*/
replay::vector3f& replay::aabb::compute_center( vector3f& result ) const
{
	return ( result = ( get0() + get1() ) * 0.5f );
}

/** Compute a given corner of the box, whereas the n'th bit in the index corresponds to the n'th axis in space.
*/
replay::vector3f& replay::aabb::compute_corner( unsigned int index, vector3f& result ) const
{
	for ( unsigned int i = 0; i < 3; ++i )
		result[ i ] = ((index&(1<<i))?get1():get0())[ i ];

	return result;
}

/** Compute a sub-aabb for octree-like splits.
*/
replay::aabb& replay::aabb::compute_subinterval( unsigned int index, const vector3f& pivot, aabb& result ) const
{
	for ( unsigned int j = 0; j < 3; ++j )
	{
		result[ 0 ][ j ] = (((index >> j) & 1) ? pivot : get0() )[ j ];
		result[ 1 ][ j ] = (((index >> j) & 1) ? get1() : pivot )[ j ];
	}

	return result;
}

/** Check if a point is inside this box.
*/
bool replay::aabb::contains( const vector3f& point ) const
{
	return math::in_range( point[ 0 ], get0()[ 0 ], get1()[ 0 ] )
		&& math::in_range( point[ 1 ], get0()[ 1 ], get1()[ 1 ] )
		&& math::in_range( point[ 2 ], get0()[ 2 ], get1()[ 2 ] );
}

/** Create the aabb [min-x,max+x].
*/
replay::aabb& replay::aabb::expand( const replay::vector3f& x )
{
	min() -= x;	max() += x;
	return *this;
}

/** Create the aabb [min-x,max+x].
*/
replay::aabb replay::aabb::expanded( const replay::vector3f& x ) const
{
	return aabb( this->min() - x, this->max() + x );
}

