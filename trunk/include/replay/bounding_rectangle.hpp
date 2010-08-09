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

#ifndef replay_bounding_rectangle_hpp
#define replay_bounding_rectangle_hpp

#include <memory>
#include <replay/vector2.hpp>
#include <replay/matrix2.hpp>
#include <replay/vector_math.hpp>

namespace replay {

/** Compute a minimal-area bounding rectangle of a 2D convex hull using a rotating calipers algorithm.
	This algorithm runs in linear time.
*/
class bounding_rectangle_algorithm
{
public:
	bounding_rectangle_algorithm( const vector2f* hull, std::size_t n )
		: hull(hull),n(n)
	{
		// Get an initial box and the extrema
		initial_extrema();

		// Start with no rotation
		current.u.set(1.f,0.f);
		
		// Check all possible configurations
		float	min_area=compute_current_size();
		best=current;

		for ( std::size_t i=1; i<n; ++i )
		{
			rotate_smallest_angle();

			// Only rotate a maximum of 90 degrees
			if( current.u[0] <= 0.f )
				break;

			float area = compute_current_size();

			// Did we find a better one?
			if ( area < min_area )
			{
				min_area = area;
				best = current;
			}
		}
	}

	/** Return the matrix used to transform into box coordinates.
	*/
	const matrix2 get_matrix() const
	{
		const matrix2 result(
			 best.u[0], best.u[1],
			-best.u[1], best.u[0] );

		return result;
	}

	const vector2f& get_min() const
	{
		return best.min;
	}

	const vector2f& get_max() const
	{
		return best.max;
	}


private:

	static inline vector2f rotated_left( const vector2f& x )
	{
		return vector2f( -x[1], x[0] );
	}

	inline vector2f get_edge( std::size_t i ) const
	{
		return hull[(i+1)%n]-hull[i];
	}

	inline void rotate_smallest_angle()
	{
		float ra,ta,la,ba;
		const float zero_deg = 1.f - 0.0001f;

		// Move along colinear edges
		vector2f t = current.u;
		while ( (ba=dot( t, normalized(get_edge(bottom)))) >= zero_deg )
			bottom = (bottom+1)%n;
		
		t = rotated_left( current.u );
		while ( (ra=dot( t, normalized(get_edge(right)))) >= zero_deg )
			right = (right+1)%n;

		t = rotated_left( t );
		while ( (ta=dot( t, normalized(get_edge(top)))) >= zero_deg)
			top = (top+1)%n;

		t = rotated_left( t );
		while ( (la=dot( t, normalized(get_edge(left)))) >= zero_deg )
			left = (left+1)%n;

		// Find the smallest angle, which is the greatest cosine
		std::size_t s = 0;
		float m = ba;

		if ( ra > m ) { s=1; m=ra; }
		if ( ta > m ) { s=2; m=ta; }
		if ( la > m ) { s=3; m=la; }

		switch( s )
		{
		case 0:
			t = normalized(get_edge(bottom));
			current.u = t;
			break;
		case 1:
			
			t = normalized(get_edge(right));
			current.u.set( t[1],-t[0] );
			break;
		case 2:
			t = normalized(get_edge(top));
			current.u = -t;
			break;
		case 3:
			t = normalized(get_edge(left));
			current.u.set( -t[1], t[0] );
			break;
		}
	}

	inline float compute_current_size()
	{
		const vector2f& u(current.u);
		// phi is defined by the matrix:
		// u[0] -u[1]
		// u[1]  u[0]

		const vector2f& l(hull[left]);
		const vector2f& b(hull[bottom]);

		current.min.set(
			 l[0]*u[0]+l[1]*u[1], // x component of phi(P[Left])
			-b[0]*u[1]+b[1]*u[0] // y component of phi(P[Bottom])
		);

		// Likewise for right and top
		const vector2f& r(hull[right]);
		const vector2f& t(hull[top]);

		current.max.set(
			 r[0]*u[0]+r[1]*u[1],
			-t[0]*u[1]+t[1]*u[0]
		);

		float dx = current.max[0]-current.min[0];
		float dy = current.max[1]-current.min[1];

		return dx*dy;
	}

	inline void initial_extrema()
	{
		left = right = top = bottom = 0;

		// Find an initial bounding box and the extrema
		for ( std::size_t i=1; i<n; ++i )
		{
			float x = hull[i][0];
			float y = hull[i][1];

			// Check for a new x
			if ( x < hull[left][0] )
				left = i;
			else if ( x > hull[right][0] )
				right = i;
			else
			{
				if ( x == hull[left][0] && y < hull[left][1] )
					left = i;

				else if ( x == hull[right][0] && y > hull[right][1] )
					right = 1;
			}

			// Check for new y
			if ( y < hull[bottom][1] )
				bottom = i;
			else if ( y > hull[top][1] )
				top = i;
			else
			{
				if ( y == hull[bottom][1] && x > hull[bottom][0] )
					bottom = i;

				else if ( y == hull[top][1] && x < hull[top][0] )
					top = i;
			}
		}
	}

	// The convex hull
	const vector2f*	hull;
	std::size_t		n;

	struct box_type {
		// box
		vector2f	min;
		vector2f	max;

		// rotation
		vector2f	u; 
	};

	box_type		current;
	box_type		best;

	// Current extrema
	std::size_t		top;
	std::size_t		bottom;
	std::size_t		left;
	std::size_t		right;

	// Selected edge
	std::size_t		selected_edge;
};

}

#endif // replay_bounding_rectangle_hpp
