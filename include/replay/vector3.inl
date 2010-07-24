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

/** Create a new vector.
	The default constructor will initialise all values as zero.
*/
template< class type > inline
replay::vector3< type >::vector3()
{
	this->data[ 0 ] = this->data[ 1 ] = this->data[ 2 ] = type( 0 );
}

/** Convert an array into a vector.
	\param array Array to copy the values from. The values will be copied from the first three elements.
*/
template< class type > inline
replay::vector3< type >::vector3( const type* array )
{
	data[ 0 ] = array[ 0 ]; data[ 1 ] = array[ 1 ];
	data[ 2 ] = array[ 2 ];
}

/** Create a vector with all elements the same value. */
template< class type > inline
replay::vector3< type >::vector3( const float value )
{
	this->data[ 0 ] = this->data[ 1 ] = this->data[ 2 ] = value;
}

/** Create a new vector from seperate values.
	\param x The first component.
	\param y The second component.
	\param z The third component.
*/
template< class type > inline
replay::vector3< type >::vector3( const type& x, const type& y, const type& z )
{
	this->data[ 0 ] = x;
	this->data[ 1 ] = y;
	this->data[ 2 ] = z;
}



/** Assign an array.
	\param array Array to copy the values from. The values will be copied from the first three elements.
*/
template< class type > inline replay::vector3< type >&
replay::vector3< type >::operator=( const type* array )
{
	this->data[ 0 ] = array[ 0 ];
	this->data[ 1 ] = array[ 1 ];
	this->data[ 2 ] = array[ 2 ];

	return (*this);
}

/** Set this objects components.
	\param x The first component.
	\param y The second component.
	\param z The third component.
*/
template< class type > inline replay::vector3< type >&
replay::vector3< type >::set( const type& x, const type& y, const type& z )
{
	this->data[ 0 ] = x;
	this->data[ 1 ] = y;
	this->data[ 2 ] = z;

	return (*this);
}

/** Set all components.
	\param v Value to set the vector to.
*/
template< class type > inline replay::vector3< type >&
replay::vector3< type >::set( const type& v )
{
	this->data[ 0 ] = this->data[ 1 ] = this->data[ 2 ] = v;

	return (*this);
}

/** Addition. */
template< class type > inline replay::vector3< type >
replay::vector3< type >::operator+( const vector3< type >& operand ) const
{
	return replay::vector3< type >(
		this->data[ 0 ] + operand.data[ 0 ],
		this->data[ 1 ] + operand.data[ 1 ],
		this->data[ 2 ] + operand.data[ 2 ] );
}

/** Subtraction. */
template< class type > inline replay::vector3< type >
replay::vector3< type >::operator-( const vector3< type >& operand ) const
{
	return replay::vector3< type >(
		this->data[ 0 ] - operand.data[ 0 ],
		this->data[ 1 ] - operand.data[ 1 ],
		this->data[ 2 ] - operand.data[ 2 ] );
}

/** Cross product.
	Also referred to as vector-product.
*/
template< class type > inline replay::vector3< type >
replay::vector3< type >::operator*( const vector3< type >& operand ) const
{
	return replay::vector3< type >(
		this->data[ 1 ] * operand.data[ 2 ] - this->data[ 2 ] * operand.data[ 1 ],
		this->data[ 2 ] * operand.data[ 0 ] - this->data[ 0 ] * operand.data[ 2 ],
		this->data[ 0 ] * operand.data[ 1 ] - this->data[ 1 ] * operand.data[ 0 ] );
}

/** Dot product. */
template< class type > inline type
replay::vector3< type >::operator|( const vector3< type >& operand ) const
{
	return	this->data[ 0 ] * operand.data[ 0 ] +
			this->data[ 1 ] * operand.data[ 1 ] +
			this->data[ 2 ] * operand.data[ 2 ];
}

/** Scalar product. */
template< class type > inline replay::vector3< type >
replay::vector3< type >::operator*( const type& operand ) const
{
	return replay::vector3< type >(
		this->data[ 0 ] * operand,
		this->data[ 1 ] * operand,
		this->data[ 2 ] * operand );
}

/** Scalar division. */
template< class type > inline replay::vector3< type >
replay::vector3< type >::operator/( const type& operand ) const
{
	return replay::vector3< type >(
		this->data[ 0 ] / operand,
		this->data[ 1 ] / operand,
		this->data[ 2 ] / operand );
}

/** Negation. */
template< class type > inline replay::vector3< type >
replay::vector3< type >::operator-() const
{
	return replay::vector3< type >(
		-this->data[ 0 ],
		-this->data[ 1 ],
		-this->data[ 2 ] );
}

/** Addition. */
template< class type > inline replay::vector3< type >&
replay::vector3< type >::operator+=( const replay::vector3< type >& operand )
{
	this->data[ 0 ] += operand.data[ 0 ];
	this->data[ 1 ] += operand.data[ 1 ];
	this->data[ 2 ] += operand.data[ 2 ];

	return *this;
}

/** Subtraction. */
template< class type > inline replay::vector3< type >&
replay::vector3< type >::operator-=( const replay::vector3< type >& operand )
{
	this->data[ 0 ] -= operand.data[ 0 ];
	this->data[ 1 ] -= operand.data[ 1 ];
	this->data[ 2 ] -= operand.data[ 2 ];

	return *this;
}

/** Scalar multiplication. */
template< class type > inline replay::vector3< type >&
replay::vector3< type >::operator*=( const type& operand )
{
	this->data[ 0 ] *= operand;
	this->data[ 1 ] *= operand;
	this->data[ 2 ] *= operand;

	return *this;
}

/** Scalar division. */
template< class type > inline replay::vector3< type >&
replay::vector3< type >::operator/=( const type& operand )
{
	this->data[ 0 ] /= operand;
	this->data[ 1 ] /= operand;
	this->data[ 2 ] /= operand;

	return *this;
}

/** Test for equality.
*/
template< class type > inline bool
replay::vector3< type >::operator==( const vector3< type >& operand ) const
{
	return	data[0] == operand[0] &&
			data[1] == operand[1] &&
			data[2] == operand[2];
}

/** Test for unequality.
*/
template< class type > inline bool
replay::vector3< type >::operator!=( const vector3< type >& operand ) const
{
	return	data[0] != operand[0] ||
			data[1] != operand[1] ||
			data[2] != operand[2];
}

/** Negate. Negate each component of this vector. */
template< class type > inline void
replay::vector3< type >::negate()
{
	this->data[ 0 ] = -this->data[ 0 ];
	this->data[ 1 ] = -this->data[ 1 ];
	this->data[ 2 ] = -this->data[ 2 ];
}

/** Square. Square this vector using the dot product. */
template< class type > inline type
replay::vector3< type >::squared() const
{
	return	this->data[ 0 ] * this->data[ 0 ] +
			this->data[ 1 ] * this->data[ 1 ] +
			this->data[ 2 ] * this->data[ 2 ];
}

/** Sum. Return a sum of all elements. */
template< class type > inline type
replay::vector3< type >::sum() const
{
	return	this->data[ 0 ] + this->data[ 1 ] + this->data[ 2 ];
}

/** Clear. Reset all values to zero. */
template< class type > inline void
replay::vector3< type >::clear()
{
	this->data[ 0 ] = this->data[ 1 ] = this->data[ 2 ] = type( 0 );
}




