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

#ifndef replay_table_hpp
#define replay_table_hpp

namespace replay {

	
/** A dynamicly sized two dimensional array class.
	Tables of size \f$0x0\f$ are called invalid - they do not maintain
	any additional memory and no elements can be accessed.
	\note Consider using \ref fixed_table instead when the size is known at compile time.
	\ingroup Container
*/
template < class T > class table
{
private:
	typedef	T				object_type;
	typedef unsigned int	size_type;
	typedef table< T >		self_type;

	object_type*			buffer;
	size_type				width;
	size_type				height;

public:

	typedef object_type*		iterator;
	typedef const object_type*	const_iterator;

	/** Fill the table with the given value.
	*/
	void				fill( const object_type& value )
	{
		const size_type num_elements = width*height;
		for ( size_type i = 0; i < num_elements; ++i )
			buffer[ i ] = value;
	}

	/** Construct a table of given width and height.
	*/
	table( const size_type width, const size_type height )
	: buffer( new object_type[ width*height ] ), width( width ), height( height ) {}

	/** Construct a table of given width and height and fill it with the given value.
	*/
	table( const size_type width, const size_type height, const object_type& value )
	: buffer( new object_type[ width*height ] ), width( width ), height( height )
	{
		fill( value );
	}
	/** Default constructor. 
		Creates an invalid table.
	*/ 
	table() : buffer( 0 ), width( 0 ), height( 0 ) {}


	/** Copy constructor.
		Will create a table of equal size and copy all elements over.
	*/
	table( const table< object_type >& other )
	: buffer( 0 ), width( other.width ), height( other.height )
	{
		if ( width && height )
		{
			const size_type num_elements = width*height;

			buffer = new object_type[ num_elements ];
			
			for ( size_type i = 0; i < num_elements; ++i )
				buffer[ i ] = other.buffer[ i ];
		}
	}

	/** Destructor.
		Will clean up all memory automatically.
	*/
	~table()
	{
		clear();
	}
	

	/** Get an iterator to the beginning of the table.
	*/
	iterator			begin() { return iterator(buffer); }
	
	
	/** Get an iterator to the beginning of the table.
	*/
	const_iterator		begin() const { return const_iterator(buffer); }


	/** Get an iterator the end of the table.
	*/
	iterator			end() { return iterator(buffer+width*height); }

	/** Get an iterator the end of the table.
	*/
	const_iterator		end() const { return const_iterator(buffer+width*height); }

	/** Invalidate the table and free the memory.
	*/
	void				clear()
	{ delete[] buffer; this->width = 0; this->height = 0; this->buffer = 0; }

	/** Assign a table and copy the contained data.
	*/
	self_type&			operator=( const self_type& other )
	{
		if ( (this->width == other.width) && (this->height == other.height) )
		{
			const size_type num_elements = width*height;
				
			for ( size_type i = 0; i < num_elements; ++i )
				buffer[ i ] = other.buffer[ i ];
		}
		else
		{
			clear();
			this->width = other.width;
			this->height = other.height;

			if ( width && height )
			{
				const size_type num_elements = width*height;

				buffer = new object_type[ num_elements ];
				
				for ( size_type i = 0; i < num_elements; ++i )
					buffer[ i ] = other.buffer[ i ];
			}
		}

		return *this;
	}

	/** Resize the table and write the value to all elements.
	*/
	void				resize( const size_type width, const size_type height, const object_type& value )
	{ this->width = width; this->height = height; delete[] buffer; buffer = new object_type[ width*height ]; fill( value ); }

	/** Resize the table and invalidate all contents.
	*/
	void				resize( const size_type width, const size_type height )
	{ this->width = width; this->height = height; delete[] buffer; buffer = new object_type[ width*height ]; }

	/** Access the table.
		\param x The column in the table.
		\param y The row in the table.
		\returns A mutable reference to an element.
		\note The order of the parameters is reversed from typical mathematical notation.
	*/
	object_type&		operator()( const size_type x, const size_type y ) { return buffer[ (width*y)+x ]; }
	
	/** Access the matrix.
		\param x The column in the table.
		\param y The row in the table.
		\returns A const reference to an element.
		\note The order of the parameters is reversed from typical mathematical notation.
	*/
	const object_type&	operator()( const size_type x, const size_type y ) const { return buffer[ (width*y)+x ]; }

	/** Compute the linear memory offset of an element.
	*/
	size_type			get_index( const size_type x, const size_type y ) const { return (width*y)+x; }

	/** Checks whether the matrix is valid.
	*/
	bool				is_valid() const { return buffer != 0; }

	/** Get an element via its linear offset.
		\see get_index
	*/
	object_type&		get( const size_type i ) { return buffer[ i ]; }
	
	/** Get an element via its linear offset.
		\see get_index
	*/
	const object_type&	get( const size_type i ) const { return buffer[ i ]; }

	/** Get the width of the table, i.e. the number of columns.
	*/
	size_type			get_width() const { return width; }
	
	/** Get the height of the table, i.e. the number of rows.
	*/
	size_type			get_height() const { return height; }

	/** Get a pointer to the raw %buffer.
		\returns A const pointer to the internal %buffer.
		\note The internal memory layout is a continguous and row-major.
	*/
	const object_type*	ptr() const { return buffer; }
	
	/** Get a pointer to the raw %buffer.
		\returns A mutable pointer to the internal %buffer.
		\note The internal memory layout is a continguous and row-major.
	*/
	object_type*		ptr() { return buffer; }

	/** Swap the contents of two tables.
		\note This is a constant-time operation.
	*/
	static void			swap( self_type& a, self_type& b )
	{
		object_type* temp_pointer = a.buffer;
		a.buffer = b.buffer;
		b.buffer = temp_pointer;

		size_type temp = a.width;
		a.width = b.width;
		b.width = temp;

		temp = a.height;
		a.height = b.height;
		b.height = temp;
	};

	/** Swap the contents of this and another table.
		\note This is a constant-time operation.
	*/
	void				swap( self_type& rhs ) { swap( *this, rhs ); }
};

/** A fixed size two dimensional array.
	This is the fixed size sibling of \ref table.
	\ingroup Container
*/
template < class T, unsigned int w, unsigned int h > class fixed_table
{
	typedef unsigned int	size_type;

	T						data[ w*h ];
public:
	/** Initialize all elements with their default constructor.
	*/
	fixed_table() {}

	/** Copy the given value to all elements.
	*/
	fixed_table( T v ) { std::fill_n( data, w*h, v ); }

	/** Fill the table with the given value.
	*/
	inline void				fill( T v ) { std::fill_n( data, w*h, v ); }

	/** Access the array elements using two indices.
	*/
	inline T&				operator()( unsigned int x, unsigned int y ) { return data[ x*h + y ]; }
	
	/** Access the array elements using two indices.
	*/
	inline const T&			operator()( unsigned int x, unsigned int y ) const { return data[ x*h + y ]; }
	
	
	/** Access the array elements using the array offset operator.
		\see get
	*/
	inline T*				operator[]( unsigned int x ) { return data + x*h; }
	
	
	/** Access the array elements using the array offset operator.
		\see get
	*/
	inline const T*			operator[]( unsigned int x ) const {return data + x*h; }

	/** Get an element via its linear offset.
	*/
	T&						get( const size_type i ) { return data[ i ]; }
	
	/** Get an element via its linear offset.
	*/
	const T&				get( const size_type i ) const { return data[ i ]; }

	/** Get width.
	*/
	size_type				get_width() const { return w; }
	
	/** Get height.
	*/
	size_type				get_height() const { return h; }

	/** Return the raw %buffer.
	*/
	inline const T*			ptr() const { return data; }

	/** Return the raw %buffer.
	*/
	inline T*				ptr() { return data; }
};

};

#endif
