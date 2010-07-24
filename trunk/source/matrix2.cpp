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

#include <cmath>
#include <replay/matrix2.hpp>

replay::matrix2::matrix2( const float m11, const float m21,
						  const float m12, const float m22 )
{
	data[ 0 ] = m11; data[ 2 ] = m21;
	data[ 1 ] = m12; data[ 3 ] = m22;
}

replay::matrix2&
replay::matrix2::set( const float m11, const float m21,
					  const float m12, const float m22 )
{
	data[ 0 ] = m11; data[ 2 ] = m21;
	data[ 1 ] = m12; data[ 3 ] = m22;

	return *this;
}

replay::matrix2&
replay::matrix2::set_identity()
{
	return set( 1.f, 0.f, 0.f, 1.f );
}

replay::matrix2&
replay::matrix2::set_rotation( const float angle )
{
	const float sin = std::sin( angle );
	const float cos = std::cos( angle );

	return set( cos,sin, -sin, cos );
}

replay::matrix2&
replay::matrix2::set_scale( const vector2f& scale )
{
	return set( scale[ 0 ], 0.f, 0.f, scale[ 1 ] );
}

const replay::vector2f
replay::matrix2::operator*( const vector2f& v ) const
{
	return vector2f(  data[ 0 ] * v[ 0 ] + data[ 2 ] * v[ 1 ],
					  data[ 1 ] * v[ 0 ] + data[ 3 ] * v[ 1 ] );
}

const replay::matrix2
replay::matrix2::operator*( const matrix2& other ) const
{
    matrix2 result;
	return multiply( *this, other, result );
}

replay::matrix2&
replay::matrix2::multiply( const matrix2& a, const matrix2& b, matrix2& result )
{
	result.data[ 0 ] = a.data[ 0 ] * b.data[ 0 ] + a.data[ 2 ] * b.data[ 1 ];
	result.data[ 1 ] = a.data[ 1 ] * b.data[ 0 ] + a.data[ 3 ] * b.data[ 1 ];
	result.data[ 2 ] = a.data[ 0 ] * b.data[ 2 ] + a.data[ 2 ] * b.data[ 3 ];
	result.data[ 3 ] = a.data[ 1 ] * b.data[ 2 ] + a.data[ 3 ] * b.data[ 3 ];

	return result;
}


replay::matrix2&
replay::matrix2::operator*=( const matrix2& Matrix )
{
	return ( (*this) = ( (*this) * Matrix ) );
}

float
replay::matrix2::determinant() const
{
	return data[0]*data[3]-data[1]*data[2];
}

bool
replay::matrix2::invert()
{
	const float d = this->determinant();

	if ( d==0.f )
		return false;

	const float t = d*data[0];
	data[0] = d*data[3];
	data[3] = t;

	data[2]=-d*data[1];
	data[3]=-d*data[2];

	return true;
}
