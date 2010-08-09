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

#ifndef replay_math_hpp
#define replay_math_hpp

#include <boost/math/special_functions/sign.hpp>
#include <replay/couple.hpp>

namespace replay {

/** Math related functions.
*/
namespace math {

	/** default numerical error tolerance.
		\ingroup Math
	*/
	const float default_epsilon = 0.000001f;
	
	/** the ratio of a circle's circumference to its diameter.
		\ingroup Math
	*/
	const float m_pi = 3.14159265358979323846f;

	/** get the absolute of a value.
		\ingroup Math
	*/
	template < class type > inline type
	abs( type value )
	{
		return ( value >= 0 ) ? value : -value;
	}
	
	/** get the absolute of a value.
		\ingroup Math
	*/
	
#ifdef replay_unsafe_bit_magic
	template <> inline float
	abs< float >( float value )
	{
		*reinterpret_cast< int* >( &value ) &= 0x7FFFFFFF;
		return value;
	}
#endif

	/** multiply a only by the sign of b.
		\ingroup Math
	*/
	inline void mult_ref_by_sign( float& a, float b )
	{
#ifdef replay_unsafe_bit_magic
		reinterpret_cast< unsigned int& >( a ) ^= (reinterpret_cast< unsigned int& >(b)&0x80000000);
#else
		a = boost::math::copysign( a, a*b );
#endif
	}
	
	/** multiply a only by the sign of b.
		\ingroup Math
	*/
	inline void mult_by_sign( float a, float b, float& result )
	{
#ifdef replay_unsafe_bit_magic
		reinterpret_cast< unsigned int& >( result ) = reinterpret_cast< unsigned int& >( a ) 
			^ (reinterpret_cast< unsigned int& >(b)&0x80000000);
#else
		result = boost::math::copysign( a, a*b );
#endif
	}
	
	/** get the sign of an integer.
		\return 1 for negative numbers and 0 for others.
		\ingroup Math
	*/
	/*inline int sign( int a )
	{
		return a >> 31;
	}*/
	
	/** copies the sign.
		\ingroup Math
	*/
	inline float copy_sign( float value, const float sign )
	{
#ifdef replay_unsafe_bit_magic
		*reinterpret_cast< int* >( &value ) &= 0x7FFFFFFF;
		*reinterpret_cast< int* >( &value ) |= (*reinterpret_cast< const int* >( &sign ) & 0x80000000);
        return value;
#else
        return boost::math::copysign( value, sign );
#endif
	}

	/** return true if the value is within a treshold of zero.
		\ingroup Math
	*/
	inline bool fuzzy_zero( const float value, const float epsilon )
	{
		return abs(value) < epsilon;
	}

	/** return true if the value is within a treshold of zero.
		\ingroup Math
	*/
	inline bool fuzzy_zero( const float value )
	{
		return abs(value) < default_epsilon;
	}

	/** Return true if a is within a treshold of b.
		This is used to compare floating point numbers.
		\ingroup Math
	*/
	inline bool fuzzy_equals( const float a, const float b, const float epsilon=default_epsilon )
	{
		return abs(a-b) < epsilon;
	}

	/** check if the value is in the range. borders count as in.
		\ingroup Math
	*/
	template < class T > inline bool in_range( const T value, const couple< T >& range )
	{
		return (range[ 0 ] <= value) && (value <= range[ 1 ]);
	}

	/** check if the value is in the range. borders count as in.
		\ingroup Math
	*/
	template < class T > inline bool in_range( const T value, const T left, const T right )
	{
		return (left <= value) && (value <= right);
	}

	/** check whether two intervals intersect.
		\ingroup Math
	*/
	template < class T > inline bool intervals_intersect( const couple< T >& a, const couple< T >& b )
	{
		return a[ 1 ] > b[ 0 ] && a[ 0 ] < b[ 1 ];
	}

	/** clamp a value into the range [-abs,abs]
		\ingroup Math
	*/
	template < class T > inline T clampabs( const T value, const T abs )
	{
		if ( value < -abs )
			return -abs;
		else if ( value > abs )
			return abs;
		else
			return value;
	}

	/** Clamp a value into a range.
		\ingroup Math
	*/
	template < class T > inline T clamp( const T value, const couple< T >& range )
	{
		if ( value < range[ 0 ] )
			return range[ 0 ];
		else if ( value > range[ 1 ] )
			return range[ 1 ];
		else
			return value;
	}

	/** Clamp a value into a range.
		\ingroup Math
	*/
	template < class T > inline T clamp( const T value, const T a, const T b )
	{
		if ( value < a )
			return a;
		else if ( value > b )
			return b;
		else
			return value;
	}

	/** Saturate the value, i.e., clamp it into the [0..1] range.
		\param x Value to be saturated.
		\ingroup Math
	*/
	inline float saturate( float x )
	{
		if ( x < 0.f )	return 0.f;
		else return std::min(x,1.f);
	}

	/** Perform a smooth hermite blend between two edge values.
		Returns 0 for values smaller than edge0 and 1 for values greater than edge1.
		Values in between are interpolated by the polynomial x*x*(3-2*x).
		\ingroup Math
	*/
	inline float smoothstep( float edge0, float edge1, float x )
	{
		x = saturate( (x-edge0)/(edge1-edge0) );
		return x*x*(3.0f-2.f*x);
	}

	/** find the sign.
		\ingroup Math
	*/
	inline unsigned int
	sign( float value ) // returns 1 for - and 0 for +
	{
#ifdef replay_unsafe_bit_magic
		return (*reinterpret_cast< unsigned int* >( &value )) >> 31;
#else
		return value < 0.f ? 1 : 0;
#endif
	}

	/** compare signs.
		\ingroup Math
	*/
	inline unsigned int
	same_sign( float a, float b )
	{
#ifdef replay_unsafe_bit_magic
		return ((
			( *reinterpret_cast< unsigned int* >( &a ) ) ^
			~( *reinterpret_cast< unsigned int* >( &b ) )
			) >> 31 );
#else
		return a*b < 0.f ? 0 : 1;		
#endif
	}

	/** convert radians to degrees.
		\ingroup Math
	*/
    inline float
	convert_to_degrees( const float radians )
	{
		static float factor = 180.f / 3.14159265358979323846f;
		return radians * factor; 
	}

	/** convert degrees to radians.
		\ingroup Math
	*/
	inline float
	convert_to_radians( const float degrees )
	{
		static float factor = 3.14159265358979323846f / 180.f;
		return degrees * factor;
	}

	/** returns true if the given integer is a power of two.
		\ingroup Math
	*/
	inline bool
	is_pow2( unsigned int Number )
	{
		return ( Number & ( Number - 1 )) == 0;
	}

	/** returns true if the given integer is a power of two.
		\ingroup Math
	*/
	inline bool
	is_pow2( int Number )
	{
		return ( Number & ( Number - 1 )) == 0;
	}

	/** compute the square.
		\ingroup Math
	*/
	template < class T > T
	square( T p ) { return p * p; }

	/** Solve a quadratic equation of the form: a*x^2+b*x+c=0
		\ingroup Math
	*/
	unsigned int
	solve_quadratic_eq( float a, float b, float c, fcouple& result, float epsilon );

	/** linear interpolation.
		\ingroup Math
	*/
	template < class type, class delta_type >
	type lerp( const type a, const type b, const delta_type x ) { return a + x*(b-a); }

	/** interpolation functions.
		\ingroup Math
	*/
	namespace interpolate {

		/** linear
		*/
		template < class type, class delta_type >
		inline type linear( const type a, const type b, const delta_type x )
		{
			return a + x*(b-a);
		}

		/** cubic
		*/
		template < class type, class delta_type >
		inline type cubic( const type& a, const type& b,
			const type& c, const type& d, const delta_type x )
		{
			// 6 mults, 8 adds

			const type P = (d-c) - (a-b);
			const type Q = (a-b) - P;

			return ( ( P*x + Q ) * x + (c-a) ) * x + b;
		}

		/** bicubic
		*/
		template < class type, class delta_type >
		inline type bicubic(
			const type& v11, const type& v21, const type& v31, const type& v41,
			const type& v12, const type& v22, const type& v32, const type& v42,
			const type& v13, const type& v23, const type& v33, const type& v43,
			const type& v14, const type& v24, const type& v34, const type& v44,
			const delta_type x, const delta_type y )
		{
			return cubic(
				cubic( v14, v13, v12, v11, y), 
				cubic( v24, v23, v22, v21, y), 
				cubic( v34, v33, v32, v31, y), 
				cubic( v44, v43, v42, v41, y),
				x );
		}
	}

}

}

#endif
