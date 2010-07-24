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

#include <replay/byte_color.hpp>

replay::byte_color4::byte_color4( const byte r, const byte g, const byte b, const byte a )
{
	data[ 0 ] = r; data[ 1 ] = g; data[ 2 ] = b; data[ 3 ] = a;
}

replay::byte_color4::byte_color4( const uint32 rgba )
{
	data[ 0 ] = (rgba >> 24);
	data[ 1 ] = (rgba >> 16) & 0xFF;
	data[ 2 ] = (rgba >> 8) & 0xFF;
	data[ 3 ] = rgba & 0xFF;
}

void replay::byte_color4::set( const byte r, const byte g, const byte b, const byte a )
{
	data[ 0 ] = r; data[ 1 ] = g; data[ 2 ] = b; data[ 3 ] = a;
}

void replay::byte_color4::negate()
{
	for ( unsigned int i = 0; i < 4; ++i )
		data[ i ] = 255 - data[ i ];
}

replay::byte_color4
replay::byte_color4::lerp( const byte_color4& color0, const byte_color4& color1, byte x )
{
	byte_color4 result;

	for ( unsigned int i = 0; i < 4; ++i )
	{
		result.data[ i ] = color0.data[ i ] +
			(( color1.data[ i ] - color0.data[ i ] )*x)/255;
	}

	return result;
}

