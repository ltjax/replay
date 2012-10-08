/*
replay
Software Library

Copyright (c) 2012 Marius Elvert

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

#ifndef replay_pixbuf_io_hpp
#define replay_pixbuf_io_hpp

#include <string>
#include <boost/filesystem/path.hpp>
#include "pixbuf.hpp"

namespace replay {

/** Loading and saving functions for raster images.
*/
namespace pixbuf_io
{
	/** Exception that is thrown on read errors.
		\ingroup Imaging
	*/
	class read_error :
		public std::runtime_error
	{
	public:
		/** Initialize with an error string.
		*/
		explicit read_error(const std::string& str) : std::runtime_error(str) {}
	};

	/** Exception that is thrown on write errors.
		\ingroup Imaging
	*/
	class					write_error : public std::exception {};

	/** Exception that is thrown when trying to load unsupported image formats.
		\ingroup Imaging
	*/
	class					unrecognized_format : public std::exception {};

	shared_pixbuf			load_from_file(std::istream& file);
	shared_pixbuf			load_from_file(const boost::filesystem::path& filename);
	void					save_to_file(const boost::filesystem::path& filename, const pixbuf& source);

	shared_pixbuf			load_from_tga_file(std::istream& file);
	void					save_to_tga_file(std::ostream& file, const pixbuf& source);

#ifdef REPLAY_USE_LIBPNG
	shared_pixbuf			load_from_png_file( std::istream& file );
	void					save_to_png_file( std::ostream& file, const pixbuf& source, int level = 1 );
#endif
}

}

#endif // replay_pixbuf_io_hpp
