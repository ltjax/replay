/*
replay
Software Library

Copyright (c) 2010-2019 Marius Elvert

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
#include <string>
#include <replay/vector4.hpp>

namespace replay
{

/** Represents a 32-bit color as a 4 byte array in RGBA.
    \ingroup Imaging
*/
class byte_rgba
{
public:
    /** 8-bit unsigned integer.
     */
    using byte = std::uint8_t;

    /** Initialized to a specific grey-value. Alpha is fixed at 255.
     */
    explicit constexpr byte_rgba(byte grey = 0) noexcept
    : data{grey, grey, grey, grey}
    {
    }

    /** Initialize the color from a 32-bit unsigned integer.
     */
    explicit constexpr byte_rgba(std::uint32_t rgba) noexcept
    : data{static_cast<byte>(rgba >> 24U), static_cast<byte>((rgba >> 16U) & 0xFFU),
           static_cast<byte>((rgba >> 8U) & 0xFFU), static_cast<byte>(rgba & 0xFFU)}
    {
    }

    /** Construct the color from the components.
     */
    constexpr byte_rgba(byte r, byte g, byte b, byte a = 255) noexcept
    : data{r, g, b, a}
    {
    }

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
    byte_rgba& operator+=(byte_rgba const& rhs);

    /** In-place subtract a color.
        Clamps all channels.
    */
    byte_rgba& operator-=(byte_rgba const& rhs);

private:
    byte data[4];
};

/** Add two colors, while clamping all channels.
 */
inline byte_rgba operator+(byte_rgba lhs, byte_rgba const& rhs)
{
    return (lhs += rhs);
}

/** Add two colors, while clamping all channels.
 */
inline byte_rgba operator-(byte_rgba lhs, byte_rgba const& rhs)
{
    return (lhs -= rhs);
}

/** Check if two colors are unequal.
 */
inline bool operator!=(byte_rgba lhs, byte_rgba rhs)
{
    for (int i = 0; i < 4; ++i)
        if (lhs[i] != rhs[i])
            return true;
    return false;
}

/** Check if two colors are equal.
 */
inline bool operator==(byte_rgba lhs, byte_rgba rhs)
{
    return !(lhs != rhs);
}

/** Compute the square perceptual difference of the RGB parts of the given colors.
    \see https://en.wikipedia.org/wiki/Color_difference
*/
int square_perceptual_difference(replay::byte_rgba lhs, replay::byte_rgba rhs);

/** Decode a 24-bit rgb unsigned integer color, e.g. decode 0xFF0000 as red.
*/
byte_rgba from_rgb_uint(std::uint32_t rgb);

/** Decode a 32-bit rgba unsigned integer color, e.g. decode 0xFF0000FF as red.
*/
byte_rgba from_rgba_uint(std::uint32_t rgba);

/** Create a color from a 4D vector.
 */
byte_rgba from_float(vector4f const& rhs);

/** Create a 4D vector from this color.
 */
vector4f to_float(byte_rgba rhs);

/** Linear interpolation of byte_color4 objects using a byte from 0 to 255.
 */
byte_rgba lerp(byte_rgba lhs, byte_rgba rhs, byte_rgba::byte x);

/** Linear interpolation of byte_color4 objects using an int from 0 to 255.
 */
byte_rgba lerp(byte_rgba lhs, byte_rgba rhs, int x);

/** Linear interpolation of byte_color4 objects using a float from 0.0f to 1.0f.
 */
byte_rgba lerp(byte_rgba lhs, byte_rgba rhs, float x);

/** Format the RGB part as a hex color, like #FFFFFF for white.
*/
std::string to_rgb_hex_string(byte_rgba rhs);

/** sample color palette.
 */
namespace palette
{
/** White. */
byte_rgba const white(255, 255, 255);
/** Black. */
byte_rgba const black(0, 0, 0);
/** Dark grey. */
byte_rgba const darkgrey(96, 96, 96);
/** Light grey. */
byte_rgba const lightgrey(192, 192, 192);
/** Medium grey. */
byte_rgba const grey(128, 128, 128);
/** Red. */
byte_rgba const red(255, 0, 0);
/** Green. */
byte_rgba const green(0, 255, 0);
/** Yellow. */
byte_rgba const yellow(255, 255, 0);
/** Blue. */
byte_rgba const blue(0, 0, 255);
/** Light cyan. */
byte_rgba const lightcyan(196, 255, 255);
/** Medium blue. */
byte_rgba const mediumblue(0, 128, 255);
/** Dark blue. */
byte_rgba const darkblue(0, 0, 128);
/** White with 50% alpha. */
byte_rgba const halfalpha(255, 255, 255, 128);
}
}

#endif
