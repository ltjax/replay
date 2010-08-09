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

#ifndef replay_vector2_hpp
#define replay_vector2_hpp

namespace replay {

/** 2-dimensional vector.
	\ingroup Math
*/
template < class type > class vector2
{
	/** the actual data.
	*/
	type						data[ 2 ];
public:

	/** Get a pointer to the internal array.
	*/
	inline type*		ptr() { return data; }

	/** Get a pointer to the internal array.
	*/
	inline const type*	ptr() const { return data; }

	/** Index access operator.
	*/
	template < class index_type >
	inline type&		operator[]( const index_type i ) { return data[i]; }

	/** Index access operator.
	*/
	template < class index_type >
	inline const type&	operator[]( const index_type i ) const { return data[i]; }

						vector2();
	explicit			vector2( const type* values );
	explicit			vector2( const type value );
						vector2( const type x, const type y );

	void				set( const type x, const type y );

	vector2< type >		operator+( const vector2< type >& operand ) const;	
	vector2< type >		operator-( const vector2< type >& operand ) const;
	type				operator|( const vector2< type >& operand ) const;
	vector2< type >		operator*( const type value ) const;
	vector2< type >		operator/( const type value ) const;
	vector2< type >		operator-() const;


	vector2< type >&	operator+=( const vector2< type >& operand );
	vector2< type >&	operator-=( const vector2< type >& operand );
	vector2< type >&	operator*=( const type value );
	vector2< type >&	operator/=( const type value );
	vector2< type >&	operator=( const type* array );

	bool				operator==( const vector2< type >& operand ) const;
	bool				operator!=( const vector2< type >& operand ) const;

	void				clear();
	void				negate();

	type				squared() const;

	/** Element wise static typecast.
	*/
	template < class x > static vector2< type > cast( const vector2< x >& from )
	{ return vector2< type >( static_cast< type >( from[ 0 ] ), static_cast< type >( from[ 1 ] ) ); }
};


#include "vector2.inl"

/** Scalar product.
	\ingroup Math
*/
template < class type > vector2< type >
operator*( const type& a, const vector2< type >& b )
{
	return b * a;
}

/** Shorthandle for a 2D \c float vector.
	\ingroup Math	
*/
typedef vector2< float > vector2f;

/** Shorthandle for a 2D \c int vector.
	\ingroup Math
*/
typedef vector2< int > vector2i;

}

#endif // replay_vector2_hpp

