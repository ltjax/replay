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

#ifndef replay_box_hpp
#define replay_box_hpp

namespace replay {

/** 2-dimensional rectangle.
*/
template < class T > struct box
{
	/** x0 */
	T			left;
	/** y0 */
	T			bottom;
	/** x1 */
	T			right;
	/** y1 */
	T			top;

	/** default ctor. leaves the values uninitialized.
	*/
	box() {}

	/** cast ctor.
	*/
	template < class U > explicit
	box( const box< U >& x ) : left( static_cast< T >( x.left ) ), bottom( static_cast< T >( x.bottom ) ),
	 right( static_cast< T >( x.right ) ), top( static_cast< T >( x.top ) ) {}

	/** create a box by giving it's constraints.
	*/
	box( const T left, const T bottom, const T right, const T top )
	: left( left ), bottom( bottom ), right( right ), top( top ) {}

	/** create a box from given sizes.
	*/
	box( const T width, const T height )
	: left( 0 ), bottom( 0 ), right( width ), top( height ) {}

	/** get the width of the box.
	*/
	T		get_width() const { return right - left; }
	
	/** get the height of the box.
	*/
	T		get_height() const { return top - bottom; }

	/** insert a point so that the box encloses the box.
	*/
	void	insert( const T x, const T y )
	{
		if ( x < left ) left = x; else if ( x > right )	right = x;
		if ( y < bottom ) bottom = y; else if ( y > top ) top = y;
	} 

	/** move the box.
	*/
	void	translate( const T x, const T y ) { left += x; right += x; bottom += y; top += y; }

	/** clamp this box into another.
	*/
	void	clamp_into( const box< T >& b )
	{
		left = ( left < b.left ) ? b.left : left;
		right = ( right < b.right ) ? right : b.right;
		bottom = ( bottom < b.bottom ) ? b.bottom : bottom;
		top = ( top < b.top ) ? top : b.top;
	}

	/** check whether this box intersects the other one.
	*/
	bool	intersects( const box< T >& b ) const
	{
		return (right > b.left) && ( left < b.right ) && ( top > b.bottom ) && ( bottom < b.top );
	}

	/** expand this box by a margin.
	*/
	box< T >	expanded( const T margin ) const
	{
		return box< T >( left-margin, bottom-margin, right+margin, top+margin );
	}

	/** set the box by constraints.
	*/
	void	set( const T x1, const T y1, const T x2, const T y2 ) { left=x1; right=x2; bottom=y1; top=y2; }
	
	/** set the size of the box.
	*/
	void	set_size( const T w, const T h ) { right = left + w; top = bottom + h; }
	
	/** set the origin of the box.
	*/
	void	set_origin( const T x, const T y ) { this->translate( x - left, y - bottom ); }

	/** check whether a given point is inside the box.
	*/
	bool	is_inside( const T x, const T y ) const { return ( x >= left ) && ( x <= right ) && ( y >= bottom ) && ( y <= top ); }
};

}

template < class stream_type, class data_type > stream_type& operator<<( stream_type& stream, const replay::box< data_type >& x )
{
	return ( stream << x.left << x.bottom << x.right << x.top );
}


template < class stream_type, class data_type > stream_type& operator>>( stream_type& stream, replay::box< data_type >& x )
{
	return ( stream >> x.left >> x.bottom >> x.right >> x.top );
}

#endif // replay_box_hpp
