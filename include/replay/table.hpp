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

#include <algorithm>

namespace replay {

	
/** A dynamicly sized two dimensional array class.
	Tables of size \f$0x0\f$ are called invalid - they do not maintain
	any additional memory and no elements can be accessed.
	\note Consider using \ref fixed_table instead when the size is known at compile time.
	\ingroup Container
*/
template <class T> class table
{
public:
	typedef	T					value_type;
	typedef std::size_t 		size_type;


	/** An iterator to use with this type.
		\note This is currently implemented as a raw pointer.
	*/
	typedef value_type*			iterator;

	/** An immutable iterator to use with this type.
		\note This is currently implemented as a const raw pointer.
	*/
	typedef const value_type*	const_iterator;

	/** Fill the table with the given value.
	*/
	void						fill(const value_type& value)
	{
		const size_type num_elements = m_width*m_height;
		for (size_type i=0; i<num_elements; ++i)
			m_buffer[i] = value;
	}

	/** Construct a table of given width and height.
	*/
	table(size_type w, size_type h)
	: m_buffer(new value_type[w*h]), m_width(w), m_height(h)
	{
	}

	/** Construct a table of given width and height and fill it with the given value.
	*/
	table(size_type w, size_type h, const value_type& value)
	: m_buffer(new value_type[w*h]), m_width(w), m_height(h)
	{
		try {
			fill(value);
		}
		catch (...)
		{
			delete[] m_buffer;
			throw;
		}
	}

	/** Default constructor. 
		Creates an invalid table.
	*/ 
	table() : m_buffer(0), m_width(0), m_height(0) {}


	/** Copy constructor.
		Will create a table of equal size and copy all elements over.
	*/
	table(const table<value_type>& rhs)
	: m_buffer(0), m_width(rhs.m_width), m_height(rhs.m_height)
	{
		if (m_width && m_height)
		{
			const size_type num_elements = m_width*m_height;
			m_buffer=new value_type[num_elements];
			
			try {
				for (size_type i=0; i<num_elements; ++i)
					m_buffer[i] = rhs.m_buffer[i];
			}
			catch (...)
			{
				delete[] m_buffer;
				throw;
			}
		}
	}

	/** Destructor.
		Will clean up all memory automatically.
	*/
	~table()
	{
		delete[] m_buffer;
	}
	

	/** Get an iterator to the beginning of the table.
	*/
	iterator				begin() {return iterator(m_buffer);}
	
	
	/** Get an iterator to the beginning of the table.
	*/
	const_iterator			begin() const {return const_iterator(m_buffer); }


	/** Get an iterator the end of the table.
	*/
	iterator				end() {return iterator(m_buffer+width()*height());}

	/** Get an iterator the end of the table.
	*/
	const_iterator			end() const {return const_iterator(m_buffer+width()*height());}

	/** Invalidate the table and free the memory.
	*/
	void					clear()
	{delete[] m_buffer; m_width=0; m_height=0; m_buffer=0;}

	/** Assign a table and copy the contained data.
	*/
	table&					operator=(table rhs)
	{
		rhs.swap(*this);
		return *this;
	}

	/** Resize the table and write the value to all elements.
	*/
	void					resize(size_type w, size_type h, const value_type& value)
	{table rhs(w, h, value); swap(*this, rhs);}

	/** Resize the table and invalidate all contents.
	*/
	void					resize(size_type w, size_type h)
	{table rhs(w, h); swap(*this, rhs);}

	/** Compute the linear memory offset of an element.
	*/
	size_type				element_offset(size_type x, size_type y) const {return (m_width*y)+x;}
	
	/** Access the table.
		\param x The column in the table.
		\param y The row in the table.
		\returns A mutable reference to an element.
	*/
	value_type&				operator()(size_type x, size_type y) {return m_buffer[element_offset(x, y)];}
	
	/** Access the table.
		\param x The column in the table.
		\param y The row in the table.
		\returns A const reference to an element.
	*/
	const value_type&		operator()(size_type x, size_type y) const {return m_buffer[element_offset(x, y)];}

	/** Access the table.
		\param rhs Array-like (x,y) coordinate into the table.
		\returns A mutable reference to an element.
	*/
	template <class T>
	value_type&				operator()(T const& rhs) {return m_buffer[element_offset(rhs[0], rhs[1])];}
	
	/** Access the table.
		\param rhs Array-like (x,y) coordinate into the table.
		\param y The row in the table.
		\returns A const reference to an element.
	*/
	template <class T>
	const value_type&		operator()(T const& rhs) const {return m_buffer[element_offset(rhs[0], rhs[1])];}


	/** Checks whether the matrix is empty.
	*/
	bool					empty() const {return m_buffer==0;}

	/** Get an element via its linear offset.
		\see get_index
	*/
	value_type&				get(size_type i) {return m_buffer[i];}
	
	/** Get an element via its linear offset.
		\see get_index
	*/
	const value_type&		get(size_type i) const {return m_buffer[i];}

	/** Get the width of the table, i.e. the number of columns.
	*/
	size_type				width() const {return m_width;}
	
	/** Get the height of the table, i.e. the number of rows.
	*/
	size_type				height() const {return m_height;}

	/** Get a pointer to the raw %buffer.
		\returns A const pointer to the internal %buffer.
		\note The internal memory layout is a continguous and row-major.
	*/
	const value_type*		ptr() const {return m_buffer;}
	
	/** Get a pointer to the raw %buffer.
		\returns A mutable pointer to the internal %buffer.
		\note The internal memory layout is a continguous and row-major.
	*/
	value_type*				ptr() {return m_buffer;}

	/** Swap the contents of two tables.
		\note This is a constant-time operation.
	*/
	static void				swap(table& lhs, table& rhs)
	{
		value_type* const data=lhs.m_buffer;
		lhs.m_buffer=rhs.m_buffer;
		rhs.m_buffer=data;

		const size_type w = lhs.m_width;
		lhs.m_width = rhs.m_width;
		rhs.m_width = w;

		const size_type h = lhs.m_height;
		lhs.m_height = rhs.m_height;
		rhs.m_height = h;
	};

	/** Swap the contents of this and another table.
		\note This is a constant-time operation.
	*/
	void					swap(table& rhs) {swap(*this, rhs);}

private:
	value_type*				m_buffer;
	size_type				m_width;
	size_type				m_height;
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
