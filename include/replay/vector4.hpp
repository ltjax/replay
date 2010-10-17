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

#ifndef replay_vector4_hpp
#define replay_vector4_hpp

#include <replay/vector2.hpp>
#include <replay/vector3.hpp>

namespace replay {
/** 4-dimensional vector.
	\ingroup Math
*/
template < class type > class vector4
{
	/** the actual data.
	*/
	type						data[ 4 ];
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

	/** Slice the first three elements into a 3 element vector.
	*/
	inline vector3< type >	slice3() const { return vector3< type >( data ); }

	void				set( const type a, const type b, const type c, const type d );

	vector4< type >		operator+( const vector4< type >& operand ) const;	
	vector4< type >		operator-( const vector4< type >& operand ) const;
	vector4< type >		operator*( const type value ) const;
	vector4< type >		operator/( const type value ) const;
	type				operator|( const vector4< type >& operand ) const;

	vector4< type >		operator-() const;

	vector4< type >&	operator+=( const vector4< type >& rhs );
	vector4< type >&	operator-=( const vector4< type >& rhs );
	vector4< type >&	operator*=( const type value );
	vector4< type >&	operator/=( const type value );

	void				clear();
	vector4< type >&	negate();

	type				sum() const;
	type				squared() const;

						vector4();
	explicit			vector4( const type* array );
	explicit			vector4( const type value );
						vector4( const vector3< type >& abc, const type d );
						vector4( const vector2< type >& ab, const vector2< type >& cd );
						vector4( const type a, const type b, const type c, const type d );
};

/** Scalar product.
	\ingroup Math
*/
template < class type > vector4< type >
operator*( const type& a, const vector4< type >& b )
{
	return b * a;
}

/** A convenience typedef for a 4d floating-point vector.
	\ingroup Math
*/
typedef vector4< float > vector4f;


/** A convenience typedef for a 4d double-precision floating-point vector.
	\ingroup Math
*/
typedef vector4< double > vector4d;

}

#include "vector4.inl"

#endif // replay_vector4_hpp
