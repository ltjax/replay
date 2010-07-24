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

#ifndef replay_buffer_hpp
#define replay_buffer_hpp

#include <cstddef>

namespace replay {

/** \defgroup Container Container classes
*/
	
/** A dynamic sized one dimensional array class that comforms to a range interface.
	This is more lightweight than std::vector, since it only every allocated space
	once, but doesn't support resizing or insertion because of that.
	\ingroup Container
*/
template < class T >
class buffer
{
private:
	typedef	T				object_type;
	typedef std::size_t		size_type;
	typedef buffer< T >		self_type;

	object_type*			_data;
	size_type				_size;

public:

	/** Iterator type to reference the objects in the sequence for reading and writing.
	*/
	typedef object_type*		iterator;

	/** Const iterator type to reference the objects in the sequence for reading.
	*/
	typedef const object_type*	const_iterator;

	/** Get an iterator to the beginning of the sequence.
	*/
	iterator			begin() { return _data; }

	/** Get an iterator to the beginning of the sequence.
	*/
	const_iterator		begin() const { return _data; }

	/** Get an iterator past the end of the sequence.
	*/
	iterator			end() { return _data+_size; }

	/** Get an iterator past the end of the sequence.
	*/
	const_iterator		end() const { return _data+_size; }

	/** Fill the buffer with the given value.
		\param value Value to be copied into all elements.
	*/
	void				fill( const object_type& value )
	{
		const size_type num_elements = _size;
		for ( size_type i = 0; i < num_elements; ++i )
			_data[ i ] = value;
	}

	/** Construct a buffer of given size.
	*/
	buffer( const size_type num_elements )
	: _data( new object_type[ num_elements ] ), _size( num_elements ) {}

	/** Construct a buffer of given size and fill it with the given value.
	*/
	buffer( const size_type num_elements, const object_type& value )
	: _data( new object_type[ num_elements ] ), _size( num_elements )
	{
		fill( value );
	}

	/** Construct a buffer from a given iterator range.
	*/
	template < class IteratorType >
	buffer( IteratorType bi, IteratorType ei ) 
		: _data( 0 ), _size( 0 )
	{
		alloc<IteratorType>( bi, ei );
	}

	/** Default constructor. Creates an empty buffer.
	*/ 
	buffer() : _data( 0 ), _size( 0 ) {}

	/** Copy constructor.
		Creates a per-value copy, all elements are copied.
	*/
	buffer( const buffer< object_type >& other )
	: _data( 0 ), _size( other.size )
	{
		if ( _size )
		{
			const size_type num_elements = _size;

			_data = new object_type[ num_elements ];
			
			for ( size_type i = 0; i < num_elements; ++i )
				_data[ i ] = other._data[ i ];
		}
	}

	/** dtor.
	*/
	~buffer()
	{
		clear();
	}

	/** Invalidate the buffer and free the memory.
	*/
	void				clear()
	{ delete[] _data; _size = 0; _data = 0; }

	/** Assign a buffer and copy the contained data.
	*/
	self_type&			operator=( const self_type& other )
	{
		if ( _size == other._size )
		{
			if ( &other == this )
				return;

			const size_type num_elements = _size;
				
			for ( size_type i = 0; i < num_elements; ++i )
				_data[ i ] = other._data[ i ];
		}
		else
		{
			clear();
			this->size = other.size;

			if ( this->size )
			{
				const size_type num_elements = size;

				_data = new object_type[ num_elements ];
				
				for ( size_type i = 0; i < num_elements; ++i )
					_data[ i ] = other._data[ i ];
			}
		}

		return *this;
	}

	/** Allocate memory and write a value to all elements.
		Completely discards previous contents.
		\param size Number of elements to allocate.
		\param value Value the objects are initialized with.
	*/
	void				alloc( const size_type size, const object_type& value )
	{ _size = size; delete[] _data; if ( _size ) { _data = new object_type[ size ]; fill( value ); } else _data = 0; }

	/** Allocate memory.
		This will invalidate all previous contents.
		\param size Number of elements to allocate. They are initialized with the default contructor.
	*/
	void				alloc( const size_type size )
	{ _size = size; delete[] _data; if ( _size ) _data = new object_type[ size ]; else _data = 0; }
	
	/** Allocate the buffer and set contents to those given in the iterator range.
	*/
	template < class IteratorType >
	void				alloc( IteratorType bi, IteratorType ei )
	{ _size = (ei-bi); delete[] _data; if ( _size ) { _data = new object_type[ _size ]; std::copy( bi, ei, _data ); } else _data = 0; }
	
	/** Get an element via its offset.
		\see get
	*/
	const object_type&	operator[]( const size_type i ) const { return _data[ i ]; }

	/** access the buffer.
	*/
	object_type&		operator[]( const size_type i ) { return _data[ i ]; }

	/** checks whether there is valid data.
	*/
	bool				is_valid() const { return _data != 0; }

	/** checks whether the buffer is empty.
	*/
	bool				empty() const { return _data == 0; }

	/** Get an element via its offset.
	*/
	object_type&		get( const size_type i ) { return _data[ i ]; }
	
	/** Get an element via its offset.
	*/
	const object_type&	get( const size_type i ) const { return _data[ i ]; }

	/** Get the number of elements in this buffer.
	*/
	size_type			size() const { return _size; }
	
	/** Return the raw buffer.
	*/
	const object_type*	ptr() const { return _data; }
	
	/** Return the raw buffer.
	*/
	object_type*		ptr() { return _data; }

	/** Swap the contents of two matrices.
	*/
	static void			swap( self_type& a, self_type& b )
	{
		object_type* temp_pointer = a._data;
		a._data = b._data;
		b._data = temp_pointer;

		size_type temp = a._size;
		a._size = b._size;
		b._size = temp;
	};

	/** Swap the contents of this and another matrix.
	*/
	void				swap( self_type& rhs ) { swap( *this, rhs ); }
};


};

#endif
