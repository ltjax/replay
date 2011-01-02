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

#include <replay/plane3.hpp>
#include <replay/vector_math.hpp>

replay::plane3&
replay::plane3::set( const vector3f& normal, const float d )
{
	this->normal = normal;
	this->d = d;
	return *this;
}

replay::plane3&
replay::plane3::set( const float a, const float b, const float c, const float d )
{
	this->normal.reset( a, b, c );
	this->d = d;
	return *this;
}

replay::plane3&
replay::plane3::set( const replay::plane3& plane )
{
	return ( *this = plane );
}

void
replay::plane3::flip()
{
	normal.negate();
	d = -d;
}

void
replay::plane3::clear()
{
	normal.reset();
	d = 0.f;
}

replay::plane3::plane3( const vector3f& normal, const float d )
: normal( normal ), d( d )
{
}

replay::plane3::plane3( const float a, const float b, const float c, const float d )
: normal( a, b, c ), d( d )
{
}

void
replay::plane3::scale( const float factor )
{
	this->normal *= factor;
	this->d *= factor;
}

replay::plane3
replay::plane3::get_flipped( const plane3& from )
{
	return plane3( -from.normal, -from.d );
}

replay::plane3
replay::plane3::construct_from_pointnormal( const vector3f& normal, const vector3f& point )
{
	return plane3( normal, -dot( normal, point ) );
}

replay::plane3
replay::plane3::construct_from_points( const vector3f& p0, const vector3f& p1, const vector3f& p2 )
{
	return construct_from_pointnormal( cross( p1-p0, p2-p0 ), p0 );
}

void
replay::plane3::hnf( const float epsilon )
{
	const float m = magnitude( this->normal );

    if ( m < epsilon )
		throw invalid_plane();

	this->scale( 1.0f / m );
}


