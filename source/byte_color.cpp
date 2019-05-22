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

#include <algorithm>
#include <replay/byte_color.hpp>

replay::byte_color4::byte_color4(byte greyvalue)
{
    set(greyvalue, greyvalue, greyvalue);
}

replay::byte_color4::byte_color4(byte r, byte g, byte b, byte a)
{
    set(r, g, b, a);
}

replay::byte_color4& replay::byte_color4::operator+=(byte_color4 const& rhs)
{
    for (std::size_t i = 0; i < 4; ++i)
        data[i] = static_cast<byte>(std::min(int(rhs.data[i]) + data[i], 255));

    return *this;
}

replay::byte_color4& replay::byte_color4::operator-=(byte_color4 const& rhs)
{
    for (std::size_t i = 0; i < 4; ++i)
        data[i] = static_cast<byte>(std::max(int(data[i]) - rhs.data[i], 0));

    return *this;
}

replay::byte_color4::byte_color4(std::uint32_t rgba)
{
    data[0] = (rgba >> 24);
    data[1] = (rgba >> 16) & 0xFF;
    data[2] = (rgba >> 8) & 0xFF;
    data[3] = rgba & 0xFF;
}

void replay::byte_color4::set(byte r, byte g, byte b, byte a)
{
    data[0] = r;
    data[1] = g;
    data[2] = b;
    data[3] = a;
}

void replay::byte_color4::negate()
{
    for (std::size_t i = 0; i < 4; ++i)
        data[i] = 255 - data[i];
}

replay::byte_color4 replay::lerp(byte_color4 lhs, byte_color4 rhs, byte_color4::byte x)
{
    byte_color4 result;

    for (std::size_t i = 0; i < 4; ++i)
    {
        result[i] = lhs[i] + ((rhs[i] - lhs[i]) * x) / 255;
    }

    return result;
}

replay::byte_color4 replay::lerp(byte_color4 lhs, byte_color4 rhs, int x)
{
    auto alpha = static_cast<byte_color4::byte>(std::min(std::max(x, 0), 255));
    return lerp(lhs, rhs, alpha);
}

replay::byte_color4 replay::lerp(byte_color4 lhs, byte_color4 rhs, float x)
{
    return lerp(lhs, rhs, static_cast<int>(255 * x));
}

replay::vector4f replay::to_float(byte_color4 rhs)
{
    vector4f result((uninitialized_tag()));

    for (std::size_t i = 0; i < 4; ++i)
        result[i] = rhs[i] / 255.f;

    return result;
}

replay::byte_color4 replay::from_float(vector4f const& rhs)
{
    replay::byte_color4 result;

    for (std::size_t i = 0; i < 4; ++i)
        result[i] =
            static_cast<replay::byte_color4::byte>(std::max(std::min(static_cast<int>(rhs[i] * 255.f), 255), 0));

    return result;
}

std::string replay::to_rgb_hex_string(replay::byte_color4 Color)
{
    std::string result = "#";
    constexpr const char* digits = "0123456789ABCDEF";
    for (int i = 0; i < 3; ++i)
    {
        result.push_back(digits[Color[i] >> 4]);
        result.push_back(digits[Color[i] & 0xf]);
    }
    return result;
}

replay::byte_color4 replay::from_rgb_uint(std::uint32_t rgb)
{
    return from_rgba_uint((rgb << 8U) | 0xFFU);
}

replay::byte_color4 replay::from_rgba_uint(std::uint32_t rgba)
{
    return replay::byte_color4{rgba};
}

int replay::square_perceptual_difference(replay::byte_color4 lhs, replay::byte_color4 rhs)
{
    // https://en.wikipedia.org/wiki/Color_difference
    auto r = lhs[0] - rhs[0];
    auto g = lhs[1] - rhs[1];
    auto b = lhs[2] - rhs[2];
    return 2 * r * r + 4 * g * g + 3 * b * b;
}
