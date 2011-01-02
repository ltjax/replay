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

#ifndef replay_vector3_hpp
#define replay_vector3_hpp

#include <iosfwd>
#include <replay/common.hpp>

namespace replay {

/** 3-dimensional vector.
	\note The element type is supposed to be a mathematical group.
	\ingroup Math
*/
template <class type> class vector3
{
public:
	typedef type						value_type;
	
	/** Get a pointer to the internal array.
	*/
	inline
	type*								ptr() {return data;}

	/** Get a pointer to the internal array.
	*/
	inline
	const type*							ptr() const {return data;}

	/** Index access operator.
	*/
	template <class index_type>	inline
	value_type&							operator[](const index_type i) {return data[i];}

	/** Index access operator.
	*/
	template <class index_type>	inline
	const value_type					operator[](const index_type i) const {return data[i];}

	// Access
	vector3<type>&						reset(value_type x, value_type y, value_type z);
	vector3<type>&						reset(value_type value = value_type(0));

	// Linear Algebra
	vector3<type>						operator-() const;									// Negation
	
	/** Create a new vector.
		This constructor will leave all values uninitialized.
	*/
	explicit							vector3(uninitialized_tag) {}
	
	/** Create a vector with all elements the same value.
	*/
	explicit							vector3(value_type value = value_type(0)) {reset(value);}
	
	/** Create a new vector from seperate values.
		\param x The first component.
		\param y The second component.
		\param z The third component.
	*/
										vector3(value_type x, value_type y, value_type z) {reset(x,y,z);}

	vector3<type>&						operator+=( const vector3<type>& operand );
	vector3<type>&						operator-=( const vector3<type>& operand );
	vector3<type>&						operator*=( const type& operand );
	vector3<type>&						operator/=( const type& operand );

	bool								operator==( const vector3< type >& operand ) const;
	bool								operator!=( const vector3< type >& operand ) const;
	
	/** In-place negate.
		Negates each element of this vector.
	*/
	vector3<type>&						negate();

	value_type							squared() const;
	value_type							sum() const;


	/** Static element wise type cast.
		This can be used on all indexable array-like types.
	*/
	template <class array_type> inline static
	vector3<type>						cast(const array_type& src)
	{
		return vector3<type>( static_cast<type>(src[0]), static_cast<type>(src[1]), static_cast<type>(src[2]) );
	}

private:
	/** the actual data.
	*/
	type								data[3];
};

/** Cross product.
	Also referred to as vector-product.
	\ingroup Math
*/
template <class type> inline 
vector3<type> cross( const vector3<type>& a, const vector3<type>& b );
	
/** Dot product of two 3D vectors.
	\ingroup Math
*/
template <class type> inline 
type dot( const vector3<type>& a, const vector3<type>& b );

/** Component wise product of two 3D vectors..
	\ingroup Math
*/
template <class type> inline 
vector3<type> comp( const vector3<type>& a, const vector3<type>& b );

/** Addition.
	\ingroup Math
*/
template <class type> inline
vector3<type> operator+(vector3<type> lhs, const vector3<type>& rhs)
{
	return lhs += rhs;
}

/** Subtraction.
	\ingroup Math
*/
template <class type> inline
vector3<type> operator-(vector3<type> lhs, const vector3<type>& rhs)
{
	return lhs -= rhs;
}

/** Scalar product.
	\ingroup Math
*/
template <class type> inline
vector3<type> operator*(vector3<type> lhs, const type rhs)
{
	return lhs *= rhs;
}

/** Scalar product.
	\ingroup Math
*/
template <class type> inline
vector3<type> operator*(const type lhs, vector3<type> rhs)
{
	return rhs *= lhs;
}

/** Scalar division.
	\ingroup Math
*/
template <class type> inline
vector3<type> operator/(vector3<type> lhs, const type& rhs)
{
	return lhs /= rhs;
}

/** Stream-out a vector in human-readable form.
	This streams a vector with elements x,y and z as "(x y z)".
	\note The element type needs to be streamable.
	\ingroup Math
*/
template <class type>
std::ostream& operator<<(std::ostream& lhs, const replay::vector3<type>& rhs)
{
	return lhs << '(' << rhs[0] << ' ' << rhs[1] << ' ' << rhs[2] << ')';
}

/** A convenience typedef for a 3d floating-point vector.
	\ingroup Math
*/
typedef vector3<float>					vector3f;

/** A convenience typedef for a 2d double-precision floating-point vector.
	\ingroup Math
*/
typedef vector3<double>					vector3d;

}

#include "vector3.inl"

#endif // replay_vector3_hpp




