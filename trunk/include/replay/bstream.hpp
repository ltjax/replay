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

#ifndef replay_bstream_hpp
#define replay_bstream_hpp

#include <iostream>
#include <fstream>
#include <string>

namespace replay {
	
/**Binary IStream Wrapper.
	Allows wrapping of standard streams for binary reading.
*/

template < class istream > class ibstream
{
private:
	istream&		stream;
public:

	/** construct the stream wrapper.
	*/
	ibstream( istream& internal ) : stream( internal ) {}
	/**Seek to a relative position in the stream.
	\param offset The offset to seek to, this is relative to the way parameter.
	\param way The seek direction, can be ios_base::beg, ios_base::cur or ios_base::end.
	\return *this*/
	ibstream&		seekg( std::ios::off_type offset, std::ios_base::seekdir way ) { stream.seekg( offset, way ); return *this; }
	/**Seek to an absolute position in the stream.
	\param position The streampos object position to seek to.
	\return *this*/
	ibstream&		seekg( std::ios::pos_type position )	{ stream.seekg( position ); return *this; }

	/**Read the stream.
	\param pointer Pointer to the stream.
	\param s Size
	\return *this*/
	ibstream&		read( void* pointer, std::streamsize s ){ stream.read( reinterpret_cast< char* >( pointer ), s ); return *this; } 
	
	/**Reads the stream, size is calculated from the type.
	\param x The stream to read.
	\return *this*/
	template < class datatype >
	ibstream&		read( datatype& x )						{ stream.read( reinterpret_cast< char* >( &x ), sizeof( datatype ) ); return *this; }

	/**Reading operator for unsigned ints.*/
	ibstream&		operator>>( unsigned int& x )			{ return read( x ); }
	/**Reading operator for signed ints.*/
	ibstream&		operator>>( signed int& x )				{ return read( x ); }

	/**Binary stream reading operator for unsigned chars.*/	
	ibstream&		operator>>( unsigned char& x )			{ return read( x ); }
	/**Reading operator for signed chars.*/
	ibstream&		operator>>( signed char& x )			{ return read( x ); }

	/**Reading operator for unsigned shorts.*/
	ibstream&		operator>>( unsigned short& x )			{ return read( x ); }
	/**Reading operator for signed shorts.*/
	ibstream&		operator>>( signed short& x )			{ return read( x ); }

	/**Reading operator for floats.*/
	ibstream&		operator>>( float& x )					{ return read( x ); }
	/**Reading operator for doubles.*/
	ibstream&		operator>>( double& x )					{ return read( x ); }

	/**Read a single unsigned int.*/
	unsigned int	read_uint()								{ unsigned int result; read( result ); return result; }

	/**Reading operator for strings.
	*/
	ibstream&		operator>>( std::string& x )
	{
		unsigned int length = 0;
		read( length );

		if ( length )
		{
			x.resize( length );
			read( &(x[0]), length );
		}

		return *this;
	}

};

/**Binary OStream Wrapper.
	Allows wrapping of standard streams for binary writing.
*/
template < class ostream > class obstream
{
private:
	ostream&		stream;
public:

	/** Construct the stream wrapper.
	*/
	obstream( ostream& internal ) : stream( internal ) {}

	/**Seek to a relative position in the stream.
	\param offset The offset to seek to, this is relative to the way parameter.
	\param way The seek direction, can be ios_base::beg, ios_base::cur or ios_base::end.
	\return *this*/
	obstream&		seekg( std::ios::off_type offset, std::ios_base::seekdir way ) { stream.seekg( offset, way ); return *this; }
	/**Seek to an absolute position in the stream.
	\param position The streampos object position to seek to.
	\return *this*/
	obstream&		seekg( std::ios::pos_type position )	{ stream.seekg( position ); return *this; }

	/**Write to the stream.
	\param pointer Pointer to the stream.
	\param s Size
	\return *this*/
	obstream&		write( const void* pointer, std::streamsize s ){ stream.write( reinterpret_cast< const char* >( pointer ), s ); return *this; } 
	template < class datatype >
	/**Writes to the stream, size is calculated from the type.
	\param x The stream to write to.
	\return *this*/
	obstream&		write( const datatype& x )					{ stream.write( reinterpret_cast< const char* >( &x ), sizeof( datatype ) ); return *this; }

	/**Writing operator for unsigned ints.*/
	obstream&		operator<<( unsigned int x )			{ return write( x ); }
	/**Writing operator for signed ints.*/
	obstream&		operator<<( signed int x )				{ return write( x ); }
	
	/**Writing operator for unsigned chars.*/
	obstream&		operator<<( unsigned char x )			{ return write( x ); }
	/**Writing operator for signed chars.*/
	obstream&		operator<<( signed char x )				{ return write( x ); }

	/**Writing operator for unsigned shorts.*/
	obstream&		operator<<( unsigned short x )			{ return write( x ); }
	/**Writing operator for signed shorts.*/
	obstream&		operator<<( signed short x )			{ return write( x ); }

	/**Writing operator for floats.*/
	obstream&		operator<<( float x )					{ return write( x ); }
	/**Writing operator for doubles.*/
	obstream&		operator<<( const double& x )			{ return write( x ); }

	/** Writing operator for strings.
	*/
	obstream&		operator<<( const std::string& x )
	{
		unsigned int length = x.length();
		write( length );

		if ( length )
		{
			write( &(x[0]), length );
		}

		return *this;
	}
};

}

#endif // replay_bstream_hpp

