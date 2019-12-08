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
#include <cassert>
#include <cstdint>
#include <replay/pixbuf.hpp>
#include <stdexcept>
#include <vector>

// Move some types into this unit's namespace
using index_type = replay::pixbuf::index_type;
using byte = replay::pixbuf::byte;
using color_format = replay::pixbuf::color_format;
using iterator = replay::pixbuf::iterator;
using const_iterator = replay::pixbuf::const_iterator;

namespace
{

inline index_type channel_count_for(color_format format)
{
    switch (format)
    {
    case color_format::rgb:
        return 3;

    case color_format::rgba:
        return 4;

    default:
        assert(format == color_format::greyscale);
        return 1;
    }
}

inline color_format color_format_for(index_type channel_count)
{
    switch (channel_count)
    {
    default:
    case 1:
        return color_format::greyscale;
    case 3:
        return color_format::rgb;
    case 4:
        return color_format::rgba;
    }
}

} // namespace


/** Create a new invalid pixbuf.
 */
replay::pixbuf::pixbuf()
: data_(nullptr)
, width_(0)
, height_(0)
, channel_count_(0)
{
}

/** Create an image with the given parameters.
 */
replay::pixbuf::pixbuf(index_type w, index_type h, index_type channel_count)
: data_(0)
, width_(w)
, height_(h)
, channel_count_(channel_count)
{
    if (channel_count != 1 && channel_count != 3 && channel_count != 4)
        throw std::invalid_argument("Unsupported channel count");

    data_ = new byte[w * h * channel_count];
}

/** Create an image with the given parameters.
 */
replay::pixbuf::pixbuf(index_type w, index_type h, color_format format)
: pixbuf(w, h, channel_count_for(format))
{
}

/** Move-construct the pixbuf. The original image is as-if default-constructed.
 */
replay::pixbuf::pixbuf(pixbuf&& rhs) noexcept
: data_(rhs.data_)
, width_(rhs.width_)
, height_(rhs.height_)
, channel_count_(rhs.channel_count_)
{
    rhs.data_ = nullptr;
    rhs.width_ = 0;
    rhs.height_ = 0;
    rhs.channel_count_ = 0;
}

/** Copy-construct the pixbuf. The original image is as-if default-constructed.
 */
replay::pixbuf::pixbuf(pixbuf const& rhs)
: data_(nullptr)
, width_(rhs.width_)
, height_(rhs.height_)
, channel_count_(rhs.channel_count_)
{
    if (!rhs.data_)
        return;

    data_ = new byte[width_ * height_ * channel_count_];
    std::copy(rhs.begin(), rhs.end(), begin());
}

/** Free the image data.
 */
replay::pixbuf::~pixbuf()
{
    delete[] data_;
}

replay::pixbuf& replay::pixbuf::operator=(replay::pixbuf const& rhs)
{
    *this = std::move(replay::pixbuf(rhs));
    return *this;
}

replay::pixbuf& replay::pixbuf::operator=(replay::pixbuf&& rhs) noexcept
{
    // Allow x = std::move(x)
    if (&rhs == this)
        return *this;

    data_ = rhs.data_;
    width_ = rhs.width_;
    height_ = rhs.height_;
    channel_count_ = rhs.channel_count_;

    rhs.data_ = 0;
    rhs.width_ = 0;
    rhs.height_ = 0;
    rhs.channel_count_ = 0;

    return *this;
}

/** Get the image width.
 */
index_type replay::pixbuf::width() const
{
    return width_;
}

/** Get the image height.
 */
index_type replay::pixbuf::height() const
{
    return height_;
}

/** Get the image number of channels.
 */
index_type replay::pixbuf::channel_count() const
{
    return channel_count_;
}

color_format replay::pixbuf::pixel_format() const
{
    return color_format_for(channel_count_);
}

/** Get a pointer to the pixel data.
 */
byte* replay::pixbuf::ptr()
{
    return data_;
}

/** Get a constant pointer to the pixel data.
 */
byte const* replay::pixbuf::ptr() const
{
    return data_;
}

/** Get a const pointer to a specific pixel.
    \param i Index of the pixel to get.
*/
byte const* replay::pixbuf::ptr(index_type i) const
{
    return data_ + channel_count_ * i;
}

/** Get a pointer to a specific pixel.
    \param i Index of the pixel to get.
*/
byte* replay::pixbuf::ptr(index_type i)
{
    return data_ + channel_count_ * i;
}

/** Get a const pointer to a specific pixel.
    \param x Column of the pixel to get.
    \param y Column of the pixel to get.
*/
byte const* replay::pixbuf::ptr(index_type x, index_type y) const
{
    return ptr(y * width_ + x);
}

/** Get a pointer to a specific pixel.
    \param x Column of the pixel to get.
    \param y Column of the pixel to get.
*/
byte* replay::pixbuf::ptr(index_type x, index_type y)
{
    return ptr(y * width_ + x);
}

/** Access the pixbuf as a const range.
 */
const_iterator replay::pixbuf::begin() const
{
    return data_;
}

/** Access the pixbuf as a range.
 */
iterator replay::pixbuf::begin()
{
    return data_;
}

/** Access the pixbuf as a const range.
 */
const_iterator replay::pixbuf::end() const
{
    return data_ + size();
}

/** Access the pixbuf as a range.
 */
iterator replay::pixbuf::end()
{
    return data_ + size();
}

index_type replay::pixbuf::size() const
{
    return width_ * height_ * channel_count_;
}

bool replay::pixbuf::empty() const
{
    return width_ == 0 || height_ == 0;
}

/** Set the pixel.
 */
void replay::pixbuf::assign_pixel(index_type x, index_type y, byte_rgba rgba)
{
    auto target = ptr(x, y);
    for (index_type i = 0; i < channel_count_; ++i)
        target[i] = rgba[i];
}

/** Set the pixel.
 */
void replay::pixbuf::assign_pixel(index_type x, index_type y, byte r, byte g, byte b, byte a)
{
    assign_pixel(x, y, { r, g, b, a });
}

/** Set the pixel.
 */
void replay::pixbuf::assign_pixel(index_type x, index_type y, byte grey)
{
    assign_pixel(x, y, byte_rgba{ grey });
}

replay::byte_rgba replay::pixbuf::read_pixel(index_type x, index_type y) const
{
    auto src = ptr(x, y);
    switch (channel_count_)
    {
    case 4:
        return byte_rgba{ src[0], src[1], src[2], src[3] };
    case 3:
        return byte_rgba{ src[0], src[1], src[2], 255 };
    case 1:
    default:
        return byte_rgba{ *src };
    }
}

/** Return a section of this image.
    \param x Source offset x-coordinate.
    \param y Source offset y-coordinate.
    \param w Width of the part to copy.
    \param h Height of the part to copy.
*/
replay::pixbuf replay::pixbuf::crop(index_type x, index_type y, index_type w, index_type h) const
{
    replay::pixbuf result(w, h, channel_count_);
    result.blit_from(0, 0, *this, w, h, x, y);
    return result;
}

/**	Copy a part of one image to another.
    \param dx Destination x-coordinate.
    \param dy Destination y-coordinate.
    \param w Width of the part to copy.
    \param h Height of the part to copy.
    \param sx Source offset x-coordinate.
    \param sy Source offset y-coordinate.
    \param source Image to source the data from.
*/

void replay::pixbuf::blit_from(index_type dx,
                               index_type dy,
                               replay::pixbuf const& source,
                               index_type w,
                               index_type h,
                               index_type sx,
                               index_type sy)
{
    byte* dst_pixel = 0;
    byte const* src_pixel = 0;

    if (channel_count_ != source.channel_count_)
        throw std::invalid_argument("Incompatible channel counts");

    auto ex = std::min(sx + w, source.width_);
    auto ey = std::min(sy + h, source.height_);

    for (; (sx < ex) && (dx < width_); ++sx, ++dx)
    {
        for (auto y0 = sy, y1 = dy; (y0 < ey) && (y1 < height_); ++y0, ++y1)
        {
            dst_pixel = ptr(dx, y1);
            src_pixel = source.ptr(sx, y0);

            for (index_type i = 0; i < channel_count_; ++i)
                dst_pixel[i] = src_pixel[i];
        }
    }
}

/** Simplified blit. Copies the whole source image to the given coordinates.
    \param dx Destination x-coordinate.
    \param dy Destination y-coordinate.
    \param source Image to source the data from.
*/
void replay::pixbuf::blit_from(index_type dx, index_type dy, pixbuf const& source)
{
    blit_from(dx, dy, source, source.width(), source.height(), 0, 0);
}

/** Fill the whole image with the given pixel value.
 */
void replay::pixbuf::fill(byte_rgba rgba)
{
    auto pixel_count = width_ * height_;

    for (index_type i = 0; i < pixel_count; ++i)
    {
        auto pixel = ptr(i);
        for (index_type c = 0; c < channel_count_; ++c)
            pixel[c] = rgba[c];
    }
}

/** Fill the whole image with the given pixel value.
 */
void replay::pixbuf::fill(byte r, byte g, byte b, byte a)
{
    fill({ r, g, b, a });
}

/** Fill the whole image with the given greyscale value.
 */
void replay::pixbuf::fill(const byte grey)
{
    fill({ grey, grey, grey, grey });
}

/** Flip the image vertically.
 */
void replay::pixbuf::flip()
{
    auto const half_height = height_ / 2;

    // Temporary swap space
    auto row_byte_count = width_ * channel_count_;
    std::vector<byte> buffer(row_byte_count);

    for (index_type y = 0; y < half_height; ++y)
    {
        auto row0 = ptr(0, y);
        auto row1 = ptr(0, height_ - 1 - y);

        // Swap rows
        std::copy(row0, row0 + row_byte_count, buffer.begin());
        std::copy(row1, row1 + row_byte_count, row0);
        std::copy(buffer.begin(), buffer.end(), row1);
    }
}

/** Convert this image to 4-channel RGBA format.
 */
void replay::pixbuf::convert_to_rgba()
{
    // Nothing to do
    if (channel_count_ == 4)
        return;

    pixbuf result(width_, height_, 4);

    auto pixel_count = width_ * height_;
    auto* src = ptr();
    auto* dst = result.ptr();

    if (channel_count_ == 3)
    {
        for (index_type i = 0; i < pixel_count; ++i)
        {
            dst[0] = src[0];
            dst[1] = src[1];
            dst[2] = src[2];
            dst[3] = 255;

            src += 3;
            dst += 4;
        }
    }
    else if (channel_count_ == 1)
    {
        for (index_type i = 0; i < pixel_count; ++i)
        {

            dst[0] = src[0];
            dst[1] = src[0];
            dst[2] = src[0];
            dst[3] = 255;

            ++src;
            dst += 4;
        }
    }

    *this = std::move(result);
}

/** \defgroup Imaging Image manipulation, loading and saving.
 */
