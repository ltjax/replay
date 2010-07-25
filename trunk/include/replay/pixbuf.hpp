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

#ifndef replay_pixbuf_hpp
#define replay_pixbuf_hpp

#include <string>
#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>

namespace replay {

/** pixel based image.
*/
class pixbuf :
	public boost::noncopyable
{
	class internal;
	internal*		data;

	pixbuf();

	void							replace_data( internal* data );
	
public:
	friend class pixbuf::internal;
	typedef boost::shared_ptr<pixbuf>	shared_pixbuf;

	/** Color-Format.
	*/
	enum color_format {
		greyscale, /**< greyscale (8-bit). */
		rgb, /**< Red,Green,Blue (24-bit). */
		rgba /**< Red,Green,Blue and Alpha (32-bit). */
	};

	/** 8-bit unsigned int.
	*/
	typedef unsigned char byte;

	~pixbuf();

	unsigned int			get_width() const;
	unsigned int			get_height() const;
	unsigned int			get_channels() const;
	const unsigned char*	get_data() const;
	unsigned char*			get_data();

	const unsigned char*	get_pixel( unsigned int i ) const;

	unsigned char*			get_pixel( unsigned int x, unsigned int y );
	const unsigned char*	get_pixel( unsigned int x, unsigned int y ) const;

	bool					blit( unsigned int dx, unsigned int dy,
								  unsigned int w, unsigned int h,
								  unsigned int sx, unsigned int sy,
								  const pixbuf& source ); 

	bool					blit( unsigned int dx, unsigned int dy,
								  const pixbuf& source );

	void					fill( const byte r, const byte g, const byte b, const byte a = 255 );
	void					fill( const byte grey );

	void					flip();

	void					set_pixel( const unsigned int x, const unsigned int y,
								const byte r, const byte g, const byte b, const byte a );

	void					set_pixel( const unsigned int x, const unsigned int y,
								const byte grey );

	void					convert_to_rgba();
	
	static
	shared_pixbuf			create( unsigned int x, unsigned int y, color_format format );
	shared_pixbuf			get_sub_image( unsigned int x, unsigned int y, unsigned int w, unsigned int h );

};

typedef pixbuf::shared_pixbuf shared_pixbuf;

}

#endif // replay_pixbuf_hpp
