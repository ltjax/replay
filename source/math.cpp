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

#include <cmath>
#include <replay/math.hpp>

unsigned int replay::math::solve_quadratic_eq( float a, float b, float c, fcouple& result, float epsilon )
{
	if ( !math::fuzzy_zero( a, epsilon ) )
	{
		float r = b*b - 4*a*c;

		// do we have non-complex solutions
		if ( r > -epsilon )
		{
            // one or two solutions?
			if ( r > epsilon ) 
			{
				// r is positive -> 2 solutions
				r = std::sqrt( r );
	            
				result.set( (-b+r)/(2*a), (-b-r)/(2*a) );

				if ( result[ 0 ] > result[ 1 ] )
					result.swap();

				return 2;
			}
			else // r is null -> 1 solution
			{
				result[ 0 ] = result[ 1 ] = (-b)/(2*a);
				return 1;		
			}
		}
	}
	else if ( !math::fuzzy_zero( b, epsilon ) )
	{
		result[ 0 ] = result[ 1 ] = -c/b;
		return 1;
	}

	return 0;
}

/** \defgroup Math Mathematical classes and functions
*/

/** \mainpage Replay
	\section section_about About
	Replay is a bootstrap library for game development. It mostly provides game development
	oriented mathematical tools, such as \ref replay::vector3, \ref replay::quaternion and,
	built on those, \ref replay::affinity.

	The library is currently hosted on google code: http://code.google.com/p/replay/

	\section section_dependencies Dependencies
	Replay depends on libpng (and therefore libpng) and boost.


*/


