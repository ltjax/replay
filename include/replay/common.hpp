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

#ifndef replay_common_hpp
#define replay_common_hpp

namespace replay {

/** Tag type to use 'uninitializing constructors'.
	Types that support this will typically expose an explicit unary constructor
	taking an uninitialized_tag and leave all contents uninitialized.
	Using these constructors is for optimization-reasons only and violates the RAII principle.
	Use with caution!
*/
class uninitialized_tag
{
};


/** Use the SFINAE idiom to selectively enable conversion-constructors.
	Several types have a conversion-constructor that can be enabled
	by specializing convertible_tag for the source and target type and
	adding a typedef named "type" that can accept a zero, for example a void*.
	The actual type does not matter, but smaller types are better in case
	they don't get optimized away properly.
*/
template <class source_type, class target_type>
struct convertible_tag
{
};

}

#endif