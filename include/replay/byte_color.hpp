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

#ifndef replay_byte_color_hpp
#define replay_byte_color_hpp

#include <cstdint>
#include <memory>
#include <replay/vector4.hpp>

namespace replay
{

/** Represents a 32-bit color as a 4 byte array in RGBA.
    \ingroup Imaging
*/
class byte_color4
{
public:
    /** 8-bit unsigned integer.
     */
    typedef std::uint8_t byte;

    /** Initialized to a specific grey-value. Alpha is fixed at 255.
     */
    byte_color4(byte greyvalue = 0);

    /** Initialize the color from a 32-bit unsigned integer.
     */
    explicit byte_color4(std::uint32_t rgba);

    /** Construct the color from the components.
     */
    byte_color4(byte r, byte g, byte b, byte a = 255);

    /** Set the color via components.
     */
    void set(byte r, byte g, byte b, byte a = 255);

    /** Invert all channels.
     */
    void negate();

    /** Get a pointer to the raw data.
     */
    byte* ptr()
    {
        return data;
    }

    /** Get a pointer to the raw data.
     */
    const byte* ptr() const
    {
        return data;
    }

    /** Access a color element.
     */
    byte operator[](std::size_t i) const
    {
        return data[i];
    }

    /** Access a color element.
     */
    byte& operator[](std::size_t i)
    {
        return data[i];
    }

    /** In-place add a color.
        Clamps all channels.
    */
    byte_color4& operator+=(byte_color4 const& rhs);

    /** In-place subtract a color.
        Clamps all channels.
    */
    byte_color4& operator-=(byte_color4 const& rhs);

private:
    byte data[4];
};

/** Add two colors, while clamping all channels.
 */
inline byte_color4 operator+(byte_color4 lhs, byte_color4 const& rhs)
{
    return (lhs += rhs);
}

/** Add two colors, while clamping all channels.
 */
inline byte_color4 operator-(byte_color4 lhs, byte_color4 const& rhs)
{
    return (lhs -= rhs);
}

/** Check if two colors are unequal.
 */
inline bool operator!=(byte_color4 lhs, byte_color4 rhs)
{
    for (int i = 0; i < 4; ++i)
        if (lhs[i] != rhs[i])
            return true;
    return false;
}

/** Check if two colors are equal.
 */
inline bool operator==(byte_color4 lhs, byte_color4 rhs)
{
    return !(lhs != rhs);
}

/** Create a color from a 4D vector.
 */
byte_color4 from_float(vector4f const& rhs);

/** Create a 4D vector from this color.
 */
vector4f to_float(byte_color4 rhs);

/** Linear interpolation of byte_color4 objects.
 */
byte_color4 lerp(byte_color4 lhs, byte_color4 rhs, byte_color4::byte x);

/** Format the RGB part as a hex color, like #FFFFFF for white.
*/
std::string to_rgb_hex(byte_color4 rhs);

/** sample color palette.
 */
namespace palette
{
/** White. */
const byte_color4 white(255, 255, 255);
/** Black. */
const byte_color4 black(0, 0, 0);
/** Dark grey. */
const byte_color4 darkgrey(96, 96, 96);
/** Light grey. */
const byte_color4 lightgrey(192, 192, 192);
/** Medium grey. */
const byte_color4 grey(128, 128, 128);
/** Red. */
const byte_color4 red(255, 0, 0);
/** Green. */
const byte_color4 green(0, 255, 0);
/** Yellow. */
const byte_color4 yellow(255, 255, 0);
/** Blue. */
const byte_color4 blue(0, 0, 255);
/** Light cyan. */
const byte_color4 lightcyan(196, 255, 255);
/** Medium blue. */
const byte_color4 mediumblue(0, 128, 255);
/** Dark blue. */
const byte_color4 darkblue(0, 0, 128);
/** White with 50% alpha. */
const byte_color4 halfalpha(255, 255, 255, 128);
}
}

#endif
