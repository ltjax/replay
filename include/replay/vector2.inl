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

/** Default constructor. 
	Initializes all components as zero.
*/
template < class type > inline
replay::vector2< type >::vector2()
{
	this->data[ 0 ] = 0;
	this->data[ 1 ] = 0;
}

/** Initialize vector from a c-style array.
*/
template < class type > inline
replay::vector2< type >::vector2( const type* values )
{
	data[ 0 ] = values[ 0 ]; data[ 1 ] = values[ 1 ];
}

/** Initialize vector by setting all elements to a single value.
*/
template < class type > inline
replay::vector2< type >::vector2( const type value )
{
	data[ 0 ] = value; data[ 1 ] = value;
}

/** Initialize vector from given values.
*/
template < class type > inline
replay::vector2< type >::vector2( const type x, const type y )
{
	this->data[ 0 ] = x;
	this->data[ 1 ] = y;
}

/** Set vector from given values.
*/
template < class type > inline void
replay::vector2< type >::set( const type x, const type y )
{
	this->data[ 0 ] = x;
	this->data[ 1 ] = y;
}

/** Vector addition.
*/
template < class type > inline replay::vector2< type >
replay::vector2< type >::operator+( const vector2< type >& operand ) const
{
	return vector2< type > (
		this->data[ 0 ] + operand.data[ 0 ],
		this->data[ 1 ] + operand.data[ 1 ] );
}

/** vector subtraction.
*/
template < class type > inline replay::vector2< type >
replay::vector2< type >::operator-( const vector2< type >& operand ) const
{
	return vector2< type > (
		this->data[ 0 ] - operand.data[ 0 ],
		this->data[ 1 ] - operand.data[ 1 ] );
}

/** Vector scalar multiplication.
*/
template < class type > inline replay::vector2< type >
replay::vector2< type >::operator*( const type value ) const
{
	return vector2< type > (
		this->data[ 0 ] * value,
		this->data[ 1 ] * value );
}

/** Vector dot product.
*/
template < class type > inline type
replay::vector2< type >::operator|( const vector2< type >& operand ) const
{
	return this->data[ 0 ]*operand.data[ 0 ] + this->data[ 1 ]*operand.data[ 1 ];
}

/** Vector scalar division.
*/
template < class type > inline replay::vector2< type >
replay::vector2< type >::operator/( const type value ) const
{
	return vector2< type > (
		this->data[ 0 ] / value,
		this->data[ 1 ] / value );
}

/** Vector negation.
*/
template < class type > inline replay::vector2< type >
replay::vector2< type >::operator-() const
{
	return vector2< type > (
		-this->data[ 0 ],
		-this->data[ 1 ] );
}

/** Vector add-assign.
*/
template < class type > inline replay::vector2< type >&
replay::vector2< type >::operator+=( const vector2< type >& operand )
{
	this->data[ 0 ] += operand.data[ 0 ];
	this->data[ 1 ] += operand.data[ 1 ];

	return (*this);
}

/** Vector subtract-assign.
*/
template < class type > inline replay::vector2< type >&
replay::vector2< type >::operator-=( const vector2< type >& operand )
{
	this->data[ 0 ] -= operand.data[ 0 ];
	this->data[ 1 ] -= operand.data[ 1 ];

	return (*this);
}

/** Vector scalar divide-assign.
*/
template < class type > inline replay::vector2< type >&
replay::vector2< type >::operator/=( const type value )
{
	this->data[ 0 ] /= value;
	this->data[ 1 ] /= value;

	return (*this);
}

/** Vector scalar multiplicate-assign.
*/
template < class type > inline replay::vector2< type >&
replay::vector2< type >::operator*=( const type value )
{
	this->data[ 0 ] *= value;
	this->data[ 1 ] *= value;

	return (*this);
}

/** Vector element wise compare.
*/
template < class type > inline bool
replay::vector2< type >::operator==( const vector2< type >& operand ) const
{
	return this->data[ 0 ] == operand.data[ 0 ] && this->data[ 1 ] == operand.data[ 1 ];
}

/** Vector element wise compare.
*/
template < class type > inline bool
replay::vector2< type >::operator!=( const vector2< type >& operand ) const
{
	return this->data[ 0 ] != operand.data[ 0 ] || this->data[ 1 ] != operand.data[ 1 ];
}

/** Vector assign from C-style array.
*/
template < class type > inline replay::vector2< type >&
replay::vector2< type >::operator=( const type* array )
{
	this->data[ 0 ] = array[ 0 ];
	this->data[ 1 ] = array[ 1 ];

	return (*this);
}

/** Vector negate.
*/
template < class type > inline void
replay::vector2< type >::negate()
{
	this->data[ 0 ] = -this->data[ 0 ];
	this->data[ 1 ] = -this->data[ 1 ];
}

/** Vector clear.
	Sets all elements to 0.
*/
template < class type > inline void
replay::vector2< type >::clear()
{
	this->data[ 0 ] = 0;
	this->data[ 1 ] = 0;
}

/** Vector dot-product square.
*/
template < class type > inline type
replay::vector2< type >::squared() const
{
	return ( this->data[ 0 ]*this->data[ 0 ] ) + ( this->data[ 1 ]*this->data[ 1 ] );
}


