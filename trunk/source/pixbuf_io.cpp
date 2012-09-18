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

#include <replay/pixbuf_io.hpp>
#include <boost/scoped_array.hpp>
#include <replay/bstream.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/fstream.hpp>

#ifdef REPLAY_USE_LIBPNG
#  include <png.h>
#  define PNG_SIGNATURE_BYTES 8
#endif

#ifdef REPLAY_USE_STBIMAGE
#  define STBI_HEADER_FILE_ONLY
#  include <stb_image.c>
#endif

namespace { // BEGIN PRIVATE NAMESPACE

typedef unsigned char uint8;
typedef unsigned short uint16;

class tga_header
{
	uint8		id_length;
	uint8		colormap_type;
	uint8		image_type;
	uint8		colormap[ 5 ];
	uint16		origin[ 2 ];
	uint16		width;
	uint16		height;
	uint8		pixeldepth;
	uint8		image_descriptor;

	replay::shared_pixbuf
				load_type2( replay::ibstream<std::istream>& file );
					
public:
	tga_header() : id_length( 0 ), colormap_type( 0 ), image_type( 0 ),
		width( 0 ), height( 0 ), pixeldepth( 0 ), image_descriptor( 0 )
	{
		colormap[ 0 ] = colormap[ 1 ] = colormap[ 2 ] = colormap[ 3 ] = colormap[ 4 ] = 0;
		origin[ 0 ] = origin[ 1 ] = 0;
	}

	replay::shared_pixbuf
				load( replay::ibstream<std::istream>& file );
	void		save( replay::obstream< std::ostream >& file, const replay::pixbuf& source );
};

#ifdef REPLAY_USE_LIBPNG
// User read function to use istream instead of FILE*
void png_user_read( png_structp png_ptr, png_bytep data, png_size_t length )
{
	std::istream* file = reinterpret_cast< std::istream* >( png_get_io_ptr( png_ptr ) );
	file->read( (char*)data, static_cast< std::streamsize >( length ) );
}


// User write function to use ostream instead of FILE*
void png_user_write( png_structp png_ptr, png_bytep data, png_size_t length )
{
	std::ostream* file = reinterpret_cast< std::ostream* >( png_get_io_ptr( png_ptr ) );
	file->write( (char*)data, static_cast< std::streamsize >( length ) );
}

// User read function to use ostream instead of FILE*
void png_user_flush( png_structp png_ptr )
{
	std::ostream* file = reinterpret_cast< std::ostream* >( png_get_io_ptr( png_ptr ) );
	file->flush();
	//*file << std::flush;
}

replay::shared_pixbuf
png_convert_paletted(png_structp png_ptr, png_infop info_ptr,
	png_bytepp row_pointers, const png_uint_32 width, const png_uint_32 height)
{
	using namespace replay;
	png_colorp	palette=0;
	int			palette_size=0;

	// Attempt to load the palette
	if (png_get_PLTE(png_ptr, info_ptr, &palette, &palette_size) != PNG_INFO_PLTE)
		throw pixbuf_io::read_error("Unable to read png-palette, file is corrupt");

	png_bytep		trans=0;
	int				trans_size=0;
	png_color_16p	trans_values=0;
	bool has_transparency = (png_get_tRNS(png_ptr, info_ptr, &trans, &trans_size, &trans_values) == PNG_INFO_tRNS);

	replay::shared_pixbuf result;
	// Take this path for palettes with transparency data
	if (has_transparency)
	{
		// Need to access both, so find the min for an upper bound
		palette_size = std::min(trans_size, palette_size);

		result = pixbuf::create(width, height, pixbuf::rgba);
		unsigned char* pixel = result->get_data();

		// Flip image during load
		for (png_uint_32 y = 0; y < height; ++y)
		for (png_uint_32 x = 0; x < width; ++x, pixel+=4)
		{
			int offset = row_pointers[height-y-1][x];

			// File data corrupt
			if (offset >= palette_size)
				throw pixbuf_io::read_error("Palette index out-of-range, file is corrupt");

			pixel[0] = palette[offset].red;
			pixel[1] = palette[offset].green;
			pixel[2] = palette[offset].blue;
			pixel[3] = trans[offset];
		}
	}
	else
	{
		result = pixbuf::create(width, height, pixbuf::rgb);
		unsigned char* pixel = result->get_data();

		// Flip image during load
		for (png_uint_32 y = 0; y < height; ++y)
		for (png_uint_32 x = 0; x < width; ++x, pixel+=3)
		{
			int offset = row_pointers[height-y-1][x];

			// File data corrupt
			if (offset >= palette_size)
				throw pixbuf_io::read_error("Palette index out-of-range, file is corrupt");

			pixel[0] = palette[offset].red;
			pixel[1] = palette[offset].green;
			pixel[2] = palette[offset].blue;
		}
	}

	return result;
}
#endif

} // END PRIVATE NAMESPACE

#ifdef REPLAY_USE_LIBPNG
/** Deserialize a PNG encoded file.
	\params file A standard-stream to the file to decode.
	\returns A shared_pixbuf containing the loaded image
	\ingroup Imaging
	\note Throws pixbuf_io::unrecognized_format or pixbuf_io::read_error when the image cannot be loaded
*/
replay::shared_pixbuf
replay::pixbuf_io::load_from_png_file(std::istream& file)
{
	shared_pixbuf		result;
	unsigned char		header[PNG_SIGNATURE_BYTES];

	// Read the signature
	const std::istream::pos_type startpos = file.tellg();
	file.read( (char*)header, PNG_SIGNATURE_BYTES );
	
	if (png_sig_cmp(header, 0, PNG_SIGNATURE_BYTES))
		throw pixbuf_io::unrecognized_format();

	file.seekg(startpos);

	// Create additional read data structures
	png_structp png_ptr = png_create_read_struct( PNG_LIBPNG_VER_STRING, 0, 0, 0 );
	if ( !png_ptr )
	{
		throw pixbuf_io::read_error( "Unable to create PNG read structure." );
	}

	png_infop info_ptr = png_create_info_struct(png_ptr);

	if ( !info_ptr )
	{
		png_destroy_read_struct( &png_ptr, 0, 0 );
		throw pixbuf_io::read_error( "Unable to create PNG info structure." );
	}

	png_infop end_info = png_create_info_struct(png_ptr);
	if ( !end_info )
	{
		png_destroy_read_struct( &png_ptr, &info_ptr, 0 );
		throw pixbuf_io::read_error( "Unable to create PNG end info structure.");
	}

	// Jump here on errors
	if ( setjmp( png_jmpbuf( png_ptr ) ) )
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		throw pixbuf_io::read_error( "Undefined read error." );
	}

	png_set_read_fn( png_ptr, &file, png_user_read );

	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY | PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING, NULL);

	png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);

	// Copy the contents
	png_uint_32 width=0;
	png_uint_32 height=0;
	int			bit_depth=0;
	int			color_type=0;

	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, 0, 0, 0);

	// Create an RGB image (3 channels)
	if (color_type == PNG_COLOR_TYPE_RGB)
	{
		result = pixbuf::create(width,height,pixbuf::rgb);
		unsigned char* pixel = result->get_data();

		// Flip image during load
		for (png_uint_32 y = 0; y < height; ++y)
		for (png_uint_32 x = 0; x < width; ++x, pixel+=3)
		{
			pixel[0] = row_pointers[height-y-1][x*3];
			pixel[1] = row_pointers[height-y-1][x*3+1];
			pixel[2] = row_pointers[height-y-1][x*3+2];
		}
	}
	// Create an RGBA image (4 channels)
	else if (color_type == PNG_COLOR_TYPE_RGB_ALPHA)
	{
		result = pixbuf::create(width,height,pixbuf::rgba);
		unsigned char* pixel = result->get_data();

		// Flip image during load
		for (png_uint_32 y = 0; y < height; ++y)
		for (png_uint_32 x = 0; x < width; ++x, pixel+=4)
		{
			pixel[0] = row_pointers[height-y-1][x*4];
			pixel[1] = row_pointers[height-y-1][x*4+1];
			pixel[2] = row_pointers[height-y-1][x*4+2];
			pixel[3] = row_pointers[height-y-1][x*4+3];
		}
	}
	// Read a paletted image
	else if (color_type == PNG_COLOR_TYPE_PALETTE)
	{
		try {
			result = png_convert_paletted(png_ptr, info_ptr, row_pointers, width, height);
		}
		catch (std::exception&)
		{
			// Make sure we don't leak memory
			png_destroy_read_struct(&png_ptr, &info_ptr,
				&end_info);

			// Rethrow
			throw;
		}
	}
	else
	{
		png_destroy_read_struct(&png_ptr, &info_ptr,
			&end_info);

		throw pixbuf_io::unrecognized_format();
	}

	png_destroy_read_struct(&png_ptr, &info_ptr,
	   &end_info);

	return result;
}

/** Serialize by encoding a PNG file via libpng.
	\param file The file to serialize to.
	\param source The image to serialize.
	\param level Compression level to be used. 0 means no compression, 9 means full compression.
	\ingroup Imaging
*/
void
replay::pixbuf_io::save_to_png_file( std::ostream& file, const pixbuf& source, int level )
{
	level = std::max( 0, std::min( level, 9 ) );

	png_structp png_ptr = png_create_write_struct( PNG_LIBPNG_VER_STRING, 0, 0, 0 );

	if ( !png_ptr )
		throw pixbuf_io::write_error();

	png_infop info_ptr = png_create_info_struct( png_ptr );

	if ( !info_ptr )
	{
		png_destroy_write_struct( &png_ptr, (png_infopp)NULL );
		throw pixbuf_io::write_error();
	}

	if ( setjmp( png_jmpbuf( png_ptr ) ) )
	{
		png_destroy_write_struct( &png_ptr, &info_ptr );
		throw pixbuf_io::write_error();
	}

	png_set_write_fn( png_ptr, &file,
		&png_user_write, &png_user_flush );

	png_set_compression_level( png_ptr,
		level );

	int color_type;

	switch( source.get_channels() )
	{
	default:
	case 1: color_type = PNG_COLOR_TYPE_GRAY; break;
	case 2: color_type = PNG_COLOR_TYPE_GRAY_ALPHA; break;
	case 3: color_type = PNG_COLOR_TYPE_RGB; break;
	case 4: color_type = PNG_COLOR_TYPE_RGB_ALPHA; break;
	};

	const png_uint_32 width = source.get_width();
	const png_uint_32 height = source.get_height();

	png_set_IHDR( png_ptr, info_ptr, width, height, 8, color_type, 
		PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT );

	// Get pointers to the rows
	boost::scoped_array<unsigned char*> row_pointer( new unsigned char*[ height ] );

	// const_cast is safe since we never modify the contents
	for ( png_uint_32 i = 0; i < height; ++i )
		row_pointer[ i ] = const_cast<unsigned char*>(source.get_pixel( 0, height-i-1 ));

	png_set_rows( png_ptr, info_ptr, row_pointer.get() );

	png_write_png( png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, 0 );

	png_destroy_write_struct( &png_ptr, &info_ptr );
}
#endif

replay::shared_pixbuf
tga_header::load_type2( replay::ibstream<std::istream>& file )
{
	using namespace replay;

	// read in colormap dummy data
	file >> colormap[ 0 ] >> colormap[ 1 ] >> colormap[ 2 ] >> colormap[ 3 ] >> colormap[ 4 ];

	// read in image specification
	file >> origin[ 0 ] >> origin[ 1 ] >> width >> height >> pixeldepth >> image_descriptor;

	// only TARGA-24 and TARGA-32 are supported
	if ( ( pixeldepth != 24 ) && ( pixeldepth != 32 ) )
		throw pixbuf_io::unrecognized_format();

	// read the freeform id
	uint8 dummy;
	for ( int i = 0; i < id_length; ++i )
		file >> dummy;

	// now read the image data
	replay::shared_pixbuf result = pixbuf::create( width, height, pixeldepth==24 ? pixbuf::rgb : pixbuf::rgba );
	unsigned int	pixelcount = width * height;
	uint8*			pixel = result->get_data();
	uint8			buffer[ 4 ];

	if ( pixeldepth == 24 )
	{
		for ( unsigned int i = 0; i < pixelcount; ++i )
		{
			file.read( buffer, 3 );
			*(pixel++) = buffer[ 2 ];
			*(pixel++) = buffer[ 1 ];
			*(pixel++) = buffer[ 0 ];
		}
	}
	else // pixeldepth == 32
	{
		for ( unsigned int i = 0; i < pixelcount; ++i )
		{
			file.read( buffer, 4 );
			*(pixel++) = buffer[ 2 ];
			*(pixel++) = buffer[ 1 ];
			*(pixel++) = buffer[ 0 ];
			*(pixel++) = buffer[ 3 ];
		}
	}

	return result;
}

/** Deserialize a TGA encoded file.
	\ingroup Imaging
*/
replay::shared_pixbuf
replay::pixbuf_io::load_from_tga_file( std::istream& file )
{
	tga_header				header;
	ibstream<std::istream>	binary_file( file );

	return header.load( binary_file );
}

/** Serialize by encoding a TGA file.
	\ingroup Imaging
*/
void
replay::pixbuf_io::save_to_tga_file( std::ostream& file, const pixbuf& source )
{
	tga_header					header;
	obstream<std::ostream>	binary_file( file );

	header.save( binary_file, source );
}


/** Save an image.
	\note Only TGA and PNG are supported right now.
	\param filename Path of the file to be saved.
	\param source The image to be saved.
	\ingroup Imaging
*/
void
replay::pixbuf_io::save_to_file( const boost::filesystem::path& filename, const pixbuf& source )
{
#if BOOST_FILESYSTEM_VERSION < 3
	const std::string ext = boost::filesystem::extension(filename);
#else
	const std::string ext = filename.extension().string();
#endif

	boost::filesystem::ofstream file;

	file.exceptions (
		std::ifstream::badbit |
		std::ifstream::eofbit |
		std::ifstream::failbit
	);

	file.open( filename, std::ios_base::out | std::ios_base::binary );

	if ( ext == ".tga" )
	{
		save_to_tga_file( file, source );
	}
#ifdef REPLAY_USE_LIBPNG
	else if ( ext == ".png" )
	{
		save_to_png_file( file, source );
	}
#endif
	else
	{
		throw pixbuf_io::unrecognized_format();
	}
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS

replay::shared_pixbuf
tga_header::load( replay::ibstream<std::istream>& file )
{
	using namespace replay;

	file >> id_length >> colormap_type >> image_type;

	// colormaps are not supported
	if ( colormap_type != 0 )
		throw pixbuf_io::unrecognized_format();

	replay::shared_pixbuf result;

	switch ( image_type )
	{
	// uncompressed, unmapped BGR image
	case 2: result=load_type2( file ); break;

	// FIXME: add this
	// compressed, unmapped 
	//case 10: load_type10( dst, file ); break;

	default:
		throw pixbuf_io::unrecognized_format();
	};

	if ( image_descriptor & (1 << 5 ) )
		result->flip();

	return result;
}

void
tga_header::save( replay::obstream<std::ostream>& file, const replay::pixbuf& source )
{
	if ( source.get_width() && source.get_height() &&
		((source.get_channels()==3) || (source.get_channels()==4)))
	{
		image_type = 2;
		width = source.get_width();
		height = source.get_height();
		pixeldepth = source.get_channels() * 8;
	}
	else
	{
		throw replay::pixbuf_io::write_error();
	}

	file << id_length << colormap_type << image_type;

	// write colormap dummy data
	file << colormap[0] << colormap[1] << colormap[2] << colormap[3] << colormap[4];

	// write image specification
	file << origin[0] << origin[1] << width << height << pixeldepth << image_descriptor;

	unsigned int	pixelcount = width * height;
	const uint8*	pixel = source.get_data();
	uint8			buffer[ 4 ];
	if ( pixeldepth == 24 )
	{
		for (unsigned int i = 0; i < pixelcount; ++i)
		{
			buffer[2] = *(pixel++);
			buffer[1] = *(pixel++);
			buffer[0] = *(pixel++);
			
			file.write( buffer, 3 );
		}
	}
	else // pixeldepth == 32
	{
		for (unsigned int i = 0; i < pixelcount; ++i)
		{
			buffer[2] = *(pixel++);
			buffer[1] = *(pixel++);
			buffer[0] = *(pixel++);
			buffer[3] = *(pixel++);
			
			file.write( buffer, 4 );
		}
	}
}

namespace 
{
	int stb_read_callback(void* user, char* data, int size)
	{
		boost::filesystem::ifstream* file(reinterpret_cast<boost::filesystem::ifstream*>(user));
		file->read(data, size);
		return file->gcount();
	}

	void stb_skip_callback(void* user, unsigned n)
	{
		
		boost::filesystem::ifstream* file(reinterpret_cast<boost::filesystem::ifstream*>(user));
		file->ignore(n);
	}

	int stb_eof_callback(void* user)
	{
		boost::filesystem::ifstream* file(reinterpret_cast<boost::filesystem::ifstream*>(user));
		return file->eof() ? 1 : 0;
	}
}

/** Load an image.
	The format is guessed from the filename's extension.
	\note Only TGA and PNG are supported right now.
	\param filename Path of the file to be loaded.
	\ingroup Imaging
*/
replay::shared_pixbuf
replay::pixbuf_io::load_from_file( const boost::filesystem::path& filename )
{
#if BOOST_FILESYSTEM_VERSION < 3
	const std::string ext = boost::filesystem::extension(filename);
#else
	const std::string ext = filename.extension().string();
#endif

	boost::filesystem::ifstream file;
	file.open(filename, std::ios_base::in | std::ios_base::binary);

	if (!file.good())
		return shared_pixbuf();


#ifdef REPLAY_USE_STBIMAGE
	stbi_io_callbacks callbacks;
	callbacks.read=&stb_read_callback;
	callbacks.skip=&stb_skip_callback;
	callbacks.eof=&stb_eof_callback;

	int rx=0, ry=0, comp=0;
	unsigned char* data=stbi_load_from_callbacks(&callbacks, &file, &rx, &ry, &comp, 0);

	if (!data)
	{
		throw pixbuf_io::unrecognized_format();
	}

	replay::pixbuf::color_format format=pixbuf::rgba;
	if (comp!=4)
	{
		if (comp==3)
			format=replay::pixbuf::rgb;
		else if (comp==1)
			format=replay::pixbuf::greyscale;
		else
		{
			stbi_image_free(data);
			throw pixbuf_io::unrecognized_format();
		}
	}
	replay::shared_pixbuf result=replay::pixbuf::create(rx, ry, format);
	unsigned char* target=result->get_data();
	
	std::size_t byte_count=rx*ry*comp;
	for (std::size_t i=0; i<byte_count; ++i)
		target[i]=data[i];
	
	stbi_image_free(data);
	result->flip();
	return result;
#else
	file.exceptions (
		std::ifstream::badbit |
		std::ifstream::eofbit |
		std::ifstream::failbit
	);

	if ( ext == ".tga" )
	{
		return load_from_tga_file(file);
	}
#endif
#ifdef REPLAY_USE_LIBPNG
	else if ( ext == ".png" )
	{	
		return load_from_png_file(file);
	}
#endif
	
	throw pixbuf_io::unrecognized_format();
}

#endif
