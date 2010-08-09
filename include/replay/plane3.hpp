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

#ifndef replay_plane3_hpp
#define replay_plane3_hpp

#include <replay/math.hpp>
#include <replay/vector3.hpp>

namespace replay {

/** 3-dimensional plane.
	All \f$\vec{x} \in \mathbb{R}^3\f$ that satisfy the equation \f$\langle\vec{\mathsf{normal}},\vec{x}\rangle + \mathsf{d} = 0\f$.
	The distance to a plane can be computed by \ref replay::distance().
	\ingroup Math
*/
class plane3
{
	public:
		/** Normal of the plane.
		*/
		vector3f			normal;
		
		/** Distance of the plane to the origin.
		*/
		float				d;

		/** Set the plane via normal and distance.
		*/
		plane3&				set( const vector3f& normal, const float d );
		
		/** Set the plane by setting all components individually.
		*/
		plane3&				set( const float a, const float b, const float c, const float d );
		
		/** Set this plane to another plane.
		*/
		plane3&				set( const plane3& plane );

		/** Multiply the equation by -1.
		*/
		void				flip();
		
		/** Clear all components to 0, effectively degenerating the plane into the \f$\mathbb{R}^3\f$.
		*/
		void				clear();

		/** Create a plane with all components set to 0.
			Represents the \f$R^3\f$.
		*/
							plane3() : d( 0.f ) {}
							
		/** Create the plane by normal and distance to the origin.
		*/
							plane3( const vector3f& normal, const float d );
							
		/** Create the plane by setting the individual components.
		*/
							plane3( const float a, const float b, const float c, const float d );

		/** Multiply the equation by a factor.
		*/
		void				scale( const float factor );

		/** Get a flipped plane.
		*/
		static plane3		get_flipped( const plane3& from );
		
		/** Construct a plane from a point and a normal.
		*/
		static plane3		construct_from_pointnormal( const vector3f& normal, const vector3f& point );
		
		/** Construct a plane from 3 CCW ordered points.
		*/
		static plane3		construct_from_points( const vector3f& p0, const vector3f& p1, const vector3f& p2 );

		/** Exception class.
		*/
		class invalid_plane : std::exception {};

		/** Convert the equation into hessian normal form.
		*/
		void				hnf( const float epsilon = math::default_epsilon );
};

}

#endif // replay_plane3_hpp

