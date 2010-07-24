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

#ifndef replay_matrix2_hpp
#define replay_matrix2_hpp

#include <replay/vector2.hpp>

namespace replay {

/** 2x2 float matrix.
	\ingroup Math
*/
class matrix2
{
	private:
		float				data[ 4 ];

	public:
		/** Create a new uninitialized matrix.
		*/
		matrix2() {}
		
		/** Create a matrix from four values.
		*/
		matrix2(	const float m11, const float m21,
					const float m12, const float m22 );

		/** Set the matrix from four values.
		*/
		matrix2&			set( const float m11, const float m21,
								 const float m12, const float m22 );

		/** Set the matrix to the identity transformation.
		*/
		matrix2&			set_identity();
		
		/** Set the matrix to a rotational transformation.
		*/
		matrix2&			set_rotation( const float angle );
		
		/** Set the matrix to a scale transformation.
		*/
		matrix2&			set_scale( const vector2f& scale );
		
		/** Concaternate the given matrix with a rotational transformation.
		*/
		static matrix2&		rotate( matrix2& m, const float angle );
		
		/** Concaternate the given matrix with a scale transformation.
		*/
		static matrix2&		scale( matrix2& m, const vector2f& scale );

		/** Get a matrix element by two indices.
		*/
		float&				operator()( const unsigned int row, const unsigned int column ) { return data[ row + (column<<1) ]; }
		
		/** Get a matrix element by two indices.
		*/
		const float			operator()( const unsigned int row, const unsigned int column ) const { return data[ row + (column<<1) ]; }
		
		/** Get a matrix element by its column major index.
		*/
		float				operator[]( const unsigned int i ) const { return data[i]; }

		/** Multiply a vector by this matrix.
		*/
		const vector2f		operator*( const vector2f& v ) const;
		
		/** Multiply two matrices.
		*/
		const matrix2		operator*( const matrix2& m ) const;
		
		/** Multiply-assign a matrix.
		*/
		matrix2&			operator*=( const matrix2& m );

		/** Get a pointer to the data.
		*/
		const float*		ptr() const { return data; }
								
		/** Get a pointer to the data.
		*/
		float*				ptr() { return data; }

		/** Multiply two matrices.
		*/
		static matrix2&		multiply( const matrix2& a, const matrix2& b, matrix2& result );

		/** Compute the determinant of the matrix.
		*/
		float				determinant() const;

		/** Invert the matrix.
			\returns false if the matrix is singular, true otherwise.
		*/
		bool				invert();
};

}

#endif // replay_matrix2_hpp

