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

#include <replay/box_packer.hpp>

#ifndef DOXYGEN_SHOULD_SKIP_THIS

class replay::box_packer::node
{
	node*			child;
	box< int >		rectangle;
	bool			in_use;

public:
	node( const box< int >& rect ) : child( 0 ), rectangle( rect ), in_use( false ) {}
	node() : child( 0 ), in_use( false ) {}
	~node() { delete[] child; }

	const box< int >&	get_rectangle() const throw() { return rectangle; }
	const node* insert( const couple< int >& size, int padding );
};

const replay::box_packer::node*
replay::box_packer::node::insert( const couple< int >& size, int padding  )
{
	if ( child == 0 )
	{
		if ( in_use )
			return 0;

		const int dw = rectangle.get_width() - ( size[ 0 ] );
		const int dh = rectangle.get_height() - ( size[ 1 ] );

		if ( ( dw < 0 ) || ( dh < 0 ) )
			return 0;

		// perfect fit?
		if ( ( dw == 0 ) &&	( dh == 0 ) )
		{
			this->in_use = true;
			return this;
		}

		child = new node[ 2 ];

		// split
		if ( dw > dh )
		{
			// divide width
			child[ 0 ].rectangle.set( rectangle.left, rectangle.bottom,
				rectangle.left+size[ 0 ], rectangle.top );

			child[ 1 ].rectangle.set( rectangle.left+size[ 0 ]+(padding<<1), rectangle.bottom,
				rectangle.right, rectangle.top );
		}
		else
		{
			// devide height
			child[ 0 ].rectangle.set( rectangle.left, rectangle.bottom,
				rectangle.right, rectangle.bottom+size[ 1 ] );

			child[ 1 ].rectangle.set( rectangle.left, rectangle.bottom+size[ 1 ]+(padding<<1),
				rectangle.right, rectangle.top );
		}

		return child[ 0 ].insert( size, padding );		
	}
	else
	{
		const node* result = child[ 0 ].insert( size, padding );

		return ( result ) ? result : child[ 1 ].insert( size, padding );
	}
}

#endif 


/** Create a new box packer.
	\param width The width of the area to pack in
	\param height The height of the area to pack in
	\param padding Space to be left between rectangles
*/
replay::box_packer::box_packer( int width, int height, int padding )
: root( new node( box< int >( padding, padding, width-padding, height-padding ) ) ), padding( padding )
{
}

/** dtor.
*/
replay::box_packer::~box_packer()
{
	delete root;
}


/** Pack an item of the given size.
	If there is no more space left to pack the given item, the function
	will throw an box_packer::pack_overflow exception.
	\param width The width of the item to pack
	\param height The height of the item to pack
*/
const replay::box< int >&
replay::box_packer::pack( int width, int height )
{
	const node* result = root->insert( replay::make_couple( width, height ), this->padding );

	if ( result == 0 )
		throw pack_overflow();
	
	return result->get_rectangle();
}

/** Pack an item of the given size.
	This is the exception free variant: if there is no more space left to pack the given item, the function
	will return false.
	\param width The width of the item to pack.
	\param height The height of the item to pack.
	\param rect The result rect where the item was placed.
*/
bool
replay::box_packer::pack( int width, int height, replay::box< int >* rect )
{
	const node* result = root->insert( replay::make_couple( width, height ), this->padding );

	if ( result == 0 )
		return false;

	if ( rect != 0 )
		*rect = result->get_rectangle();

	return true;
}

/** get the width (without padding).
*/
int
replay::box_packer::get_width() const
{
	return root->get_rectangle().get_width() + 2*padding;
}


/** get the height (without padding).
*/
int
replay::box_packer::get_height() const
{
	return root->get_rectangle().get_height() + 2*padding;
}


/** get the padding between boxes.
*/
int
replay::box_packer::get_padding() const
{
	return padding;
}



