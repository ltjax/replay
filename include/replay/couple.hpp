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

#ifndef replay_couple_hpp
#define replay_couple_hpp

/**Replay namespace*/

namespace replay {
	
/** A homogenous binary tuple type.
	\ingroup Container
*/
template < class T > class couple
{
private:
	T			array[ 2 ];
public:

	/** A lexical less comparision functor type.
	*/
	struct less
	{
		/** Evaluate the predicate.
			\param lhs The left-hand side of the operator.
			\param rhs The right-hand side of the operator.
			\returns The result of the comparison.
		*/
		bool operator()( const couple<T>& lhs, const couple<T>& rhs ) const
		{
			return lhs[0]<rhs[0] || (lhs[0]==rhs[0] && lhs[1]<rhs[1]);
		}
	};

	/** Empty constructor.
		Leave the elements default-initialized.
	*/
	inline couple() {}

	/**	Constructor.
		\param a first value.
		\param b second value.
		\see replay::make_couple
	*/
	inline couple( const T& a, const T& b ) { array[ 0 ] = a; array[ 1 ] = b; }

	/** Get an element from the couple.
	*/
	inline T& operator[]( const unsigned int i ) { return array[ i ]; }

	/** Get an element from a couple.
	*/
	inline const T& operator[]( const unsigned int i ) const { return array[ i ]; }

	/** Get the first element.
	*/
	inline const T& get0() const { return array[ 0 ]; }

	/** Get the first element.
	*/
	inline T& get0() { return array[ 0 ]; }

	/** Get the second element.
	*/
	inline const T& get1() const { return array[ 1 ]; }

	/** Get the second element.
	*/
	inline T& get1() { return array[ 1 ]; }

	/** Get a pointer to the array.
	*/
	inline const T* ptr() const { return array; }

	/** Get a pointer to the array.
	*/
	inline T* ptr() { return array; }

	/** Updates the values in the couple.
		\param a the first value.
		\param b the seconf value.
	*/
	inline couple< T >& set( const T& a, const T& b ) { array[ 0 ] = a; array[ 1 ] = b; return *this; }

	/** Swap the values in the couple.
	*/
	inline couple< T >& swap() { const T x = array[ 0 ]; array[ 0 ] = array[ 1 ]; array[ 1 ] = x; return *this; }

	/** Inequantity.
	*/
	bool operator!=( const couple< T >& other ) const { return array[ 0 ] != other.array[ 0 ] || array[ 1 ] != other.array[ 1 ]; }
	
	/** Equantity.
	*/
	bool operator==( const couple< T >& other ) const { return array[ 0 ] == other.array[ 0 ] && array[ 1 ] == other.array[ 1 ]; }
	
};

/** Couple of floats.
*/
typedef couple< float > fcouple;

/** Make a couple without providing template parameters
	\param a The first value.
	\param b The second value.
	\return The new couple object.
*/
template < class T > couple< T > make_couple( const T& a, const T& b )
{
	return couple< T >( a, b );
}

}

#endif // replay_couple_hpp
