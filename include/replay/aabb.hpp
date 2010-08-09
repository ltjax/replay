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

#ifndef replay_aabb_hpp
#define replay_aabb_hpp

#include <algorithm>
#include <replay/couple.hpp>
#include <replay/vector3.hpp>
#include <replay/plane3.hpp>

namespace replay {

/** An iso-box in \f$\mathbb{R}^3\f$.
	Represents the intersection of intervals on the 3 principal axes.
	\ingroup Math
*/
class aabb :
	public couple<vector3f>
{
	typedef couple<vector3f>		base_class;

public:
	/** Classification relative to a plane.
		\see classify
	*/
	enum clsfctn
	{
		negative, /**< All points have a negative distance to the plane. */
		positive, /**< All points have a positive distance to the plane. */
		spanning  /**< The points have mixed signs in their distances to the plane, so the box intersects the plane. */
	};
									aabb( );
	explicit						aabb( const float extends );
									aabb( const vector3f& min, const vector3f& max );

	aabb&							clear();
	aabb&							move( const vector3f& delta );

	/** Enlarge the box to contain the given point.
		\param point The point to be inserted.
	*/
	aabb&							insert( const vector3f& point )
	{
		for ( unsigned int i = 0; i < 3; ++i )
		{
			if ( point[ i ] < (*this)[ 0 ][ i ] )
				(*this)[ 0 ][ i ] = point[ i ];
			if ( point[ i ] > (*this)[ 1 ][ i ] )
				(*this)[ 1 ][ i ] = point[ i ];
		}
		
		return *this;
	}

	aabb&							insert( const vector3f* points, unsigned int count );
	aabb&							insert( const vector3f* points, const unsigned short* indices, unsigned int count );
	aabb&							insert( const vector3f* points, const unsigned int* indices, unsigned int count );
	aabb&							insert( const aabb& x );

	vector3f&						compute_arvo_vector( const vector3f& point, vector3f& result ) const;
	
	/** Compute the shortest vector from the point to the box.
	*/
	inline vector3f					compute_arvo_vector( const vector3f& point ) const
									{ vector3f temp; return compute_arvo_vector( point, temp ); }

	vector3f&						compute_center( vector3f& result ) const;
	
	/** Compute the center of the aabb.
	*/
	inline vector3f					compute_center() const
									{ vector3f temp; return compute_center( temp ); }

	vector3f&						compute_corner( unsigned int index, vector3f& result ) const;
	
	/** Compute a corner of the aabb.
	*/
	inline vector3f					compute_corner( unsigned int index ) const
									{ vector3f temp; return compute_corner( index, temp ); }

	aabb&							compute_subinterval( unsigned int index, const vector3f& pivot, aabb& result ) const;

	float							square_distance( const vector3f& other ) const;
	float							distance( const vector3f& other ) const;

	bool							contains( const vector3f& point ) const;

	void							project( const vector3f& x, fcouple& result ) const;
	clsfctn							classify( const plane3& x ) const;

	aabb&							expand( const vector3f& x );
	aabb							expanded( const vector3f& x ) const;

	/** Get the minimum in all three dimensions.
	*/
	vector3f&						min() { return get0(); }

	/** Get the minimum in all three dimensions.
	*/
	const vector3f&					min() const { return get0(); }

	/** Get the minimum element in the given dimension.
	*/
	template < class IndexType >
	float&							min( IndexType i ) { return get0()[i]; }

	/** Get the minimum element in the given dimension.
	*/
	template < class IndexType >
	float							min( IndexType i ) const { return get0()[i]; }

	/** Get the maximum in all three dimensions.
	*/
	vector3f&						max() { return get1(); }

	/** Get the maximum in all three dimensions.
	*/
	const vector3f&					max() const { return get1(); }

	/** Get the maximum element in the given dimension.
	*/
	template < class IndexType >
	float&							max( IndexType i ) { return get1()[i]; }

	/** Get the maximum element in the given dimension.
	*/
	template < class IndexType >
	float							max( IndexType i ) const { return get1()[i]; }


};

}

#endif
