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

#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <replay/math.hpp>
#include <replay/scalar.hpp>
#include <cctype>

/** Gives the value of this scalar as a double.
	If the value cannot be converted to double, this will return 0.
*/
double replay::scalar::as_double() const
{
	std::istringstream str( *this );
	double result = 0.0;
	str >> result;
	return result;
}

/** Gives the value of this scalar as a float.
	If the value cannot be converted to float, this will return 0.
*/
float replay::scalar::as_float() const
{
	std::istringstream str( *this );
	float result = 0.0;
	str >> result;
	return result;
}

/** Gives the value of this scalar as a string.
*/
const std::string& replay::scalar::as_string() const
{
	return *this;
}


/** Gives the value of this scalar as an unsigned int.
	If the value cannot be converted to unsigned int, this will return 0.
*/
unsigned int replay::scalar::as_uint() const
{
	std::istringstream str( *this );
	unsigned int result = 0;
	str >> result;

	if ( result == 0 && str.peek() == 'x' )
	{
		char x;
		str >> x >> x;

		while ( str.good() )
		{
			if ( std::isdigit( x ) )
				result = 16*result + (x - '0');
			else if ( math::in_range( x, 'a', 'f' ) )
				result = 16*result + (x - 'a') + 10;
			else if ( math::in_range( x, 'A', 'F' ) )
				result = 16*result + (x - 'A') + 10;
			else
				break;

			str >> x;
		}
		/*str.str( *this );
		str.setf( std::ios::hex );
		str >> result;*/
	}

	return result;
}

/** Gives the value of this scalar as an int.
	If the value cannot be converted to int, this will return 0.
*/
int replay::scalar::as_int() const
{
	std::istringstream str( *this );
	int result = 0;
	str >> result;
	return result;
}

/** Gives the value of this scalar as bool.
	"true" and "false" strings are also considered valid boolean sequences.
*/
bool replay::scalar::as_bool() const
{
	if ( boost::algorithm::all( static_cast< const std::string& >( *this ),
		boost::algorithm::is_digit() ) )
	{
		return this->as_uint() > 0;
	}
	else
	{
		std::istringstream str( *this );
		bool result = false;
		str.setf( std::ios::boolalpha );
		str >> result;
		return result;
	}
}

/** Assign a C-style string.
*/
replay::scalar& replay::scalar::operator=( const char* x )
{
	static_cast< std::string& >( *this ) = x;
	return *this;
}

/** Assign a string.
*/
replay::scalar&	replay::scalar::operator=( const std::string& x )
{
	static_cast< std::string& >( *this ) = x;
	return *this;
}

/** Assign another scalar.
*/
replay::scalar&	replay::scalar::operator=( const scalar& x )
{
	static_cast< std::string& >( *this ) = x;
	return *this;
}

/** Assign a boolean value.
*/
replay::scalar& replay::scalar::operator =( const bool x )
{
	std::ostringstream str;
	str << x;
	return (*this = str.str()); 
}

/** Assign a float value.
*/
replay::scalar&	replay::scalar::operator=( const float x )
{
	std::ostringstream str;
	str << x;
	return (*this = str.str());
}

/** Assign an unsigned int.
*/
replay::scalar&	replay::scalar::operator=( const unsigned int x )
{
	std::ostringstream str;
	str << x;
	return (*this = str.str());
}

/** Assign an int.
*/
replay::scalar&	replay::scalar::operator=( const int x )
{
	std::ostringstream str;
	str << x;
	return (*this = str.str());
}

/** Assign a double.
*/
replay::scalar&	replay::scalar::operator=( const double x )
{
	std::ostringstream str;
	str << x;
	return (*this = str.str());
}

/** Clear the value.
*/
void replay::scalar::clear()
{
	std::string::clear();
}

