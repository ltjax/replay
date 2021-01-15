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
#include <replay/byte_rgba.hpp>

replay::byte_rgba& replay::byte_rgba::operator+=(byte_rgba const& rhs)
{
    for (std::size_t i = 0; i < 4; ++i)
        data[i] = static_cast<byte>(std::min(int(rhs.data[i]) + data[i], 255));

    return *this;
}

replay::byte_rgba& replay::byte_rgba::operator-=(byte_rgba const& rhs)
{
    for (std::size_t i = 0; i < 4; ++i)
        data[i] = static_cast<byte>(std::max(int(data[i]) - rhs.data[i], 0));

    return *this;
}

void replay::byte_rgba::set(byte r, byte g, byte b, byte a)
{
    data[0] = r;
    data[1] = g;
    data[2] = b;
    data[3] = a;
}

void replay::byte_rgba::negate()
{
    for (std::size_t i = 0; i < 4; ++i)
        data[i] = 255 - data[i];
}

replay::byte_rgba replay::lerp(byte_rgba lhs, byte_rgba rhs, byte_rgba::byte x)
{
    byte_rgba result;

    for (std::size_t i = 0; i < 4; ++i)
    {
        result[i] = lhs[i] + ((rhs[i] - lhs[i]) * x) / 255;
    }

    return result;
}

replay::byte_rgba replay::lerp(byte_rgba lhs, byte_rgba rhs, int x)
{
    auto alpha = static_cast<byte_rgba::byte>(std::min(std::max(x, 0), 255));
    return lerp(lhs, rhs, alpha);
}

replay::byte_rgba replay::lerp(byte_rgba lhs, byte_rgba rhs, float x)
{
    return lerp(lhs, rhs, static_cast<int>(255 * x));
}

std::string replay::to_rgb_hex_string(replay::byte_rgba Color)
{
    std::string result = "#";
    constexpr const char* digits = "0123456789ABCDEF";
    for (int i = 0; i < 3; ++i)
    {
        result.push_back(digits[Color[i] >> 4U]);
        result.push_back(digits[Color[i] & 0xfU]);
    }
    return result;
}

replay::byte_rgba replay::from_rgb_uint(std::uint32_t rgb)
{
    return from_rgba_uint((rgb << 8U) | 0xFFU);
}

replay::byte_rgba replay::from_rgba_uint(std::uint32_t rgba)
{
    return replay::byte_rgba{rgba};
}

int replay::square_perceptual_difference(replay::byte_rgba lhs, replay::byte_rgba rhs)
{
    // https://en.wikipedia.org/wiki/Color_difference
    auto r = lhs[0] - rhs[0];
    auto g = lhs[1] - rhs[1];
    auto b = lhs[2] - rhs[2];
    return 2 * r * r + 4 * g * g + 3 * b * b;
}
