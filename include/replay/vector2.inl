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

/** Set vector elements from individual values.
*/
template <class type> inline
replay::vector2<type>& replay::vector2<type>::reset(const value_type x, const value_type y)
{
	data[0] = x;
	data[1] = y;
	return *this;
}

/** Vector reset.
	Sets all elements to a given value, which defaults to 0.
*/
template <class type> inline
replay::vector2<type>& replay::vector2<type>::reset(const value_type value)
{
	data[0] = value;
	data[1] = value;
	return *this;
}

/** Vector negation.
*/
template <class type> inline replay::vector2<type>
replay::vector2<type>::operator-() const
{
	return vector2<type>(-data[0], -data[1]);
}

/** Vector add-assign.
*/
template <class type> inline replay::vector2<type>&
replay::vector2<type>::operator+=(const vector2<type>& rhs)
{
	data[0] += rhs.data[0];
	data[1] += rhs.data[1];

	return (*this);
}

/** Vector subtract-assign.
*/
template <class type> inline replay::vector2<type>&
replay::vector2<type>::operator-=(const vector2<type>& rhs)
{
	data[0] -= rhs.data[0];
	data[1] -= rhs.data[1];

	return (*this);
}

/** Vector scalar divide-assign.
*/
template <class type> inline
replay::vector2<type>&
replay::vector2<type>::operator/=(const value_type rhs)
{
	data[0] /= rhs;
	data[1] /= rhs;

	return (*this);
}

/** Vector scalar multiplicate-assign.
*/
template <class type> inline
replay::vector2<type>&
replay::vector2<type>::operator*=(const value_type rhs)
{
	data[0] *= rhs;
	data[1] *= rhs;

	return (*this);
}

/** Vector element wise compare.
*/
template <class type> inline
bool replay::vector2<type>::operator==(const vector2<type>& rhs) const
{
	return data[0]==rhs.data[0] && data[1]==rhs.data[1];
}

/** Vector element wise compare.
*/
template <class type> inline
bool replay::vector2<type>::operator!=(const vector2<type>& rhs) const
{
	return data[0]!=rhs.data[0] || data[1]!=rhs.data[1];
}

/** In-place vector negate.
*/
template <class type> inline
replay::vector2<type>&
replay::vector2<type>::negate()
{
	data[0] = -data[0];
	data[1] = -data[1];

	return *this;
}


/** Vector dot-product square.
*/
template <class type> inline
typename replay::vector2<type>::value_type
replay::vector2<type>::squared() const
{
	return data[0]*data[0] + data[1]*data[1];
}

/** Sum of all elements in the vector.
*/
template <class type> inline
typename replay::vector2<type>::value_type
replay::vector2<type>::sum() const
{
	return data[0] + data[1];
}


