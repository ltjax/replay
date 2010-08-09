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

#ifndef replay_byte_color_hpp
#define replay_byte_color_hpp

#include <memory>

namespace replay {

/** Represents a 32-bit color as a 4 byte array in RGBA.
*/
class byte_color4
{
public:
	/** 8-bit unsigned integer.
	*/
	typedef unsigned char	byte;

	/** 32-bit unsigned integer.
	*/
	typedef unsigned int uint32;

	/** Default ctor that does nothing.
		\note this also leaves this color uninitialized.
	*/
							byte_color4() {}
							
	/** Initialize the color from a uint32 code.
	*/
	explicit				byte_color4( const uint32 rgba );
	
	/** Construct the color from the components.
	*/
							byte_color4( const byte r, const byte g, const byte b, const byte a=255 );

	/** Set the color via components.
	*/
	void					set( const byte r, const byte g, const byte b, const byte a=255 );
	
	/** Invert all channels.
	*/
	void					negate();

	/** Linear interpolation.
	*/
	static byte_color4		lerp( const byte_color4& color0, const byte_color4& color1, byte x );

	/** Get a pointer to the raw data.
	*/
	byte*					ptr() { return data; }
	
	/** Get a pointer to the raw data.
	*/
	const byte*				ptr() const { return data; }

	
	/** Access a color element.
	*/
	const byte				operator[]( std::size_t i ) const { return data[i]; }


	/** Access a color element.
	*/
	byte&					operator[]( std::size_t i ) { return data[i]; }



private:
	byte					data[4];

};

/** sample color palette.
*/
namespace palette {
	/** White. */
	const byte_color4 white( 255, 255, 255 );
	/** Black. */
	const byte_color4 black(   0,   0,   0 );
	/** Dark grey. */
	const byte_color4 darkgrey(  96,  96,  96 );
	/** Light grey. */
	const byte_color4 lightgrey( 192, 192, 192 );
	/** Medium grey. */
	const byte_color4 grey( 128, 128, 128 );
	/** Red. */
	const byte_color4 red( 255,   0,   0 );
	/** Green. */
	const byte_color4 green(   0, 255,   0 );
	/** Yellow. */
	const byte_color4 yellow( 255, 255,   0 );
	/** Blue. */
	const byte_color4 blue(   0,   0, 255 );
	/** Light cyan. */
	const byte_color4 lightcyan( 196, 255, 255 );
	/** Medium blue. */
	const byte_color4 mediumblue(   0, 128, 255 );
	/** Dark blue. */
	const byte_color4 darkblue(   0,   0, 128 );
	/** White with 50% alpha. */
	const byte_color4 halfalpha( 255, 255, 255, 128 );
}

}

#endif
