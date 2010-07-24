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

/**Set the values of the vector.
\param x The first component.
\param y The second component.
\param z The third component.
\param w The fourth component.*/

template < class type > inline void
replay::vector4< type >::set( const type x, const type y, const type z, const type w )
{
	this->data[ 0 ] = x;
	this->data[ 1 ] = y;
	this->data[ 2 ] = z;
	this->data[ 3 ] = w;
}

/**Addition*/
template < class type > inline replay::vector4< type >
replay::vector4< type >::operator+( const vector4< type >& operand ) const
{
	return vector4< type >(
		this->data[ 0 ] + operand.data[ 0 ],
		this->data[ 1 ] + operand.data[ 1 ],
		this->data[ 2 ] + operand.data[ 2 ],
		this->data[ 3 ] + operand.data[ 3 ] );
}

/**Subtraction*/
template < class type > inline replay::vector4< type >
replay::vector4< type >::operator-( const vector4< type >& operand ) const
{
	return vector4< type >(
		this->data[ 0 ] - operand.data[ 0 ],
		this->data[ 1 ] - operand.data[ 1 ],
		this->data[ 2 ] - operand.data[ 2 ],
		this->data[ 3 ] - operand.data[ 3 ] );
}

/**Multiplication*/
template < class type > inline replay::vector4< type >
replay::vector4< type >::operator*( const type value ) const
{
	return vector4< type >(
		this->data[ 0 ] * value,
		this->data[ 1 ] * value,
		this->data[ 2 ] * value,
		this->data[ 3 ] * value );
}

/**Division*/
template < class type > inline replay::vector4< type >
replay::vector4< type >::operator/( const type value ) const
{
	return vector4< type >(
		this->data[ 0 ] / value,
		this->data[ 1 ] / value,
		this->data[ 2 ] / value,
		this->data[ 3 ] / value );
}

/**Dot product*/
template < class type > inline type
replay::vector4< type >::operator|( const vector4< type >& operand ) const
{
	return  this->data[ 0 ] * operand.data[ 0 ] +
			this->data[ 1 ] * operand.data[ 1 ] +
			this->data[ 2 ] * operand.data[ 2 ] +
			this->data[ 3 ] * operand.data[ 3 ];
}

/**Addition*/
template < class type > inline replay::vector4< type >&
replay::vector4< type >::operator+=( const vector4< type >& rhs )
{
	data[0]+=rhs.data[0];
	data[1]+=rhs.data[1];
	data[2]+=rhs.data[2];
	data[3]+=rhs.data[3];

	return *this;
}

/**Subtraction*/
template < class type > inline replay::vector4< type >&
replay::vector4< type >::operator-=( const vector4< type >& rhs )
{
	data[0]-=rhs.data[0];
	data[1]-=rhs.data[1];
	data[2]-=rhs.data[2];
	data[3]-=rhs.data[3];

	return *this;
}

/**Multiplication*/
template < class type > inline replay::vector4< type >&
replay::vector4< type >::operator*=( const type value )
{
	this->data[ 0 ] *= value;
	this->data[ 1 ] *= value;
	this->data[ 2 ] *= value;
	this->data[ 3 ] *= value;

	return *this;
}

/**Division*/
template < class type > inline replay::vector4< type >&
replay::vector4< type >::operator/=( const type value )
{
	this->data[ 0 ] /= value;
	this->data[ 1 ] /= value;
	this->data[ 2 ] /= value;
	this->data[ 3 ] /= value;

	return *this;
}


/** Clear. Reset all values to zero. */
template < class type > inline void
replay::vector4< type >::clear()
{
	this->data[ 0 ] = this->data[ 1 ] = this->data[ 2 ] = this->data[ 3 ] = 0;
}

/**Default Constructor.*/
template < class type > inline
replay::vector4< type >::vector4()
{
	this->data[ 0 ] = this->data[ 1 ] = this->data[ 2 ] = this->data[ 3 ] = 0;
}

/** Convert an array into a vector.
	\param array Array to copy the values from. The values will be copied from the first four elements.
*/
template < class type > inline
replay::vector4< type >::vector4( const type* array )
{
	data[ 0 ] = array[ 0 ]; data[ 1 ] = array[ 1 ];
	data[ 2 ] = array[ 2 ]; data[ 3 ] = array[ 3 ];
}

/** Initialize the vector by setting all elements to a single value.
*/
template < class type > inline
replay::vector4< type >::vector4( const type value )
{
	data[ 0 ] = value; data[ 1 ] = value;
	data[ 2 ] = value; data[ 3 ] = value;
}

/** Convert a 3-tuple and a 4th element into a vector.
*/
template < class type > inline
replay::vector4< type >::vector4( const vector3< type >& abc, const type d )
{
	this->data[ 0 ] = abc[ 0 ]; this->data[ 1 ] = abc[ 1 ]; this->data[ 2 ] = abc[ 2 ]; this->data[ 3 ] = d;
}

/** Convert two 2-tuples into a vector,
*/
template < class type > inline
replay::vector4< type >::vector4( const vector2< type >& ab, const vector2< type >& cd )
{
	this->data[ 0 ] = ab[ 0 ]; this->data[ 1 ] = ab[ 1 ]; this->data[ 2 ] = cd[ 0 ]; this->data[ 3 ] = cd[ 1 ];
}

/**Create a new vector from seperate values.
\param x The first component.
\param y The second component.
\param z The third component.
\param w The fourth component.*/
template < class type > inline
replay::vector4< type >::vector4( const type x, const type y, const type z, const type w )
{
	set( x, y, z, w );
}

/** Negate. Negate each component of this vector. */
template < class type > inline replay::vector4< type >&
replay::vector4< type >::negate()
{
	this->data[ 0 ] = -this->data[ 0 ];
	this->data[ 1 ] = -this->data[ 1 ];
	this->data[ 2 ] = -this->data[ 2 ];
	this->data[ 3 ] = -this->data[ 3 ];
}

/** Compute the sum of all elements. */
template < class type > inline type
replay::vector4< type >::sum()
{
	return this->data[ 0 ] + this->data[ 1 ]
		+ this->data[ 2 ] + this->data[ 3 ];
}

/**Negated.*/
template < class type > inline replay::vector4< type >
replay::vector4< type >::operator-() const
{
	return vector4<type>(
		-this->data[ 0 ],
		-this->data[ 1 ],
		-this->data[ 2 ],
		-this->data[ 3 ] );
}


